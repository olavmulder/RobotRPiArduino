#include "header/Map.h"
#include "header/Route.h"
#include "header/Motor.h"
#include "header/I2c.h"
#include "header/enumVar.h"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "header/image.h"
#include "header/contouren.h"
#include "header/grouping.h"
#include "header/target_cv.h"
#include "header/draw.h"

#include <wiringPiI2C.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <thread>
//#define MANUAL
#define AUTOMATIC
#define TURN

#define AMOUNT_LOCATIONS 3

using namespace cv;
using namespace std;

#define DEVICE_ID 0x08
#define MAX_RES_WIDTH 640
#define MAX_RES_HEIGHT 320

/*
 V  draaien om de 3 tegels
 V   if destenation cell is block pak een cell eerder als destanation 
    - cell onderscheiden van onbekent, vrij, blocked en target
    - iets met states-> als target wordt herkend dan stop met rijden.
    - 2 threads -> global var voor state, dan state checken
    - angel vragen ?? -180/180 kan niet, toch??. 
        twee waardes sturen? 0 & 1 daarna waarde, 1 positief, 0 negatief
*/

void pathfinding();
void openCV();

int main() {
  Color idTarget;
  Color *id = &idTarget;
  int offsetTarget;
  int *offset = &offsetTarget;

  std::thread path(pathfinding, offset, id);
  std::thread cv(openCV, offset, id);
  path.join();
  cv.join();
  return 0;
}
void openCV(int* offset, Color *id){

	Mat img;
	VideoCapture cap(0);
	
	while(true){
    
		/*cap.read(img);
		Image Image(img);
		Contouren Contouren(Image.getImage());
		Grouping Grouping(Contouren.getCenters(), Contouren.getContoursPoly(), Contouren.getContourSize(), Contouren.getProcessed());
	  TargetCV TargetCV(Grouping.getTotalGroup(), Grouping.getGroupCounter(), Grouping.getArray(), Contouren.getContourSize(), Contouren.getContoursPoly());
		Draw Draw(Contouren.getContoursPoly(), TargetCV.getTargetGroups(), Contouren.getProcessed());*/
		
		*offset = 345;//TargetCV.getOffset();
    *id = (Color)1;//TargetCV.getColor();
		waitKey(1);
  }

}
void pathfinding(int* offset, Color *id){
  Route route;
  Motor motor;
  Map map(&motor, AMOUNT_LOCATIONS);
  
  //variable to count down for a 360 turn
  //start x, start y, dest x, dest y; 
  //x,y
  int finishCoordinates[][2] = {
    {WIDTH-1, 0},//right top corner
    {WIDTH-1,HEIGHT-1},//right under corner
    {0, HEIGHT-1},//left under corner
    {0,0}//top left corner
  };
  
  //init map variables
  int mapNew[HEIGHT][WIDTH];
  int mapOld[HEIGHT][WIDTH];
  int *ptrMap;
  //current location
  
  //test function 
  // int *x= map.motor->GetCurrentLocation();
  // int *y= x+1;
  // map.SetDistanceArray();//calculate distance
  // map.CalculateTargetLocation(0, *x,*y, map.motor->GetCurrentDirection(), offset);
  // map.SetMap();
  // map.PrintMap();
  

  //set map
  map.SetDistanceArray();//update distance values
  map.SetMap();
  ptrMap = map.GetMap();
  for(int i =0;i<HEIGHT;i++){
    for(int j=0;j<WIDTH;j++){
      mapNew[i][j] = mapOld[i][j] = *(ptrMap+(WIDTH*i)+j);
    }
  }
  //calculate route
  route.SetRoute(&mapNew[0][0], 0,0, finishCoordinates[0][0], finishCoordinates[0][1]);
  route.PrintRoute();
  #ifdef TURN
    int turnCounter = 2;
  #endif

  //for(unsigned char i=0;i<AMOUNT_LOCATIONS;i++){
    while(route.GetstepInRouteCounter()/2 < route.GetSize()){

      map.SetDistanceArray();//update distance values

      if(*offset != 0){//if target is detected get cur location + front distanc sensor
        //check color/id
        printf("color: %d\n", *id);
        if(!map.GetTargetHit(*id)){//if detected color is already hit, skip this
          map.SetTargetOffset(*id,targetOffset);
          map.CalculateTargetLocation(*id, *map.motor->GetCurrentLocation(),*(map.motor->GetCurrentLocation()+1),map.motor->GetCurrentDirection());//front camera = +1

        }else{
          printf("current id is already hit\n");
        }
      }else{
        //update map
        map.SetMap();
        ptrMap = map.GetMap();
        for(int i =0;i<HEIGHT;i++){
          for(int j=0;j<WIDTH;j++){
            mapNew[i][j]= *(ptrMap+(WIDTH*i)+j);
          }
        }
        //check if map is changed
        map.CheckDifference(&mapNew[0][0], &mapOld[0][0]);
        if(map.GetChanged()){
          std::cout << "new route" << std::endl;
          for(int i =0;i<HEIGHT;i++){
            for(int j=0;j<WIDTH;j++){
              mapOld[i][j]= mapNew[i][j];
            }
          }

          if(route.SetRoute(&mapOld[0][0], *map.motor->GetCurrentLocation(), *(map.motor->GetCurrentLocation()+1), 
            finishCoordinates[route.GetRouteCounter()][0], finishCoordinates[route.GetRouteCounter()][1]) == 0){
          }
        }else{
          std::cout << "same route" << std::endl;
        }
        route.PrintRoute();
        //set manual location:
        #ifdef MANUAL
          int x,y, ownDir;
          std::cout << "\nType x:";
          std::cin >> x;
          std::cout << "\nType y: ";
          std::cin >> y;
          std::cout << "\nType direction: ";
          std::cin >> ownDir;
          map.motor->SetCurrentLocation(x,y);
          map.motor->SetCurrentDirection((DirNouse)ownDir);
          sleep(2);
        #endif
        #ifdef AUTOMATIC
        
        map.motor->CalculateCurrentLocationWithRoute(route.GetRoute(), route.GetSize(),route.GetstepInRouteCounter());

        route.SetstepInRouteCounter(route.GetstepInRouteCounter()+2);
        route.PrintstepInRouteCounter();
        //turn 360 after three tiles
          #ifdef TURN
            turnCounter --;
            std::cout << "turnCounter = ";
            std::cout << turnCounter << std::endl;
            if(turnCounter==0){
              motor.Drive(TURN360);
              turnCounter = 2;
            }
          #endif
        #endif
      }
    }
  //}
}
