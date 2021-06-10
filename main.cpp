#include "header/Map.h"
#include "header/Route.h"
#include "header/Motor.h"
#include "header/I2c.h"
#include "header/enumVar.h"
#include "header/image.h"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <wiringPiI2C.h>
#include <cmath>

#include <iostream>
#include <fstream>
#include <thread>
//#define MANUAL
#define AUTOMATIC

#define AMOUNT_LOCATIONS 4

using namespace cv;
using namespace std;

#define DEVICE_ID 0x08
#define MAX_RES_WIDTH 640
#define MAX_RES_HEIGHT 320


/*
Mat src_gray;
Mat img;
int thresh = 100;
RNG rng(12345);*/

/*
 V  draaien om de 3 tegels
 V   if destenation cell is block pak een cell eerder als destanation 
    - cell onderscheiden van onbekent, vrij, blocked en target
    - iets met states-> als target wordt herkend dan stop met rijden.
    - 2 threads -> global var voor state, dan state checken
    - angel vragen ?? -180/180 kan niet, toch??. 
        twee waardes sturen? 0 & 1 daarna waarde, 1 positief, 0 negatief
*/

Point2f thresh_callback(vector<vector<Point>> contourVar, Mat cannyVar);
void swap(float *xp, float *yp); 
void selectionSort(vector<Point2f> arr, int n);

void pathfinding();
int openCV();
//global state:
bool targetDetected =false;

int main() {

  std::thread path(pathfinding);
  std::thread cv(openCV);
  path.join();
  cv.join();
  return 0;
}
int openCV(){

	Mat img;
	VideoCapture cap(0);
	
	while(true){
		cap.read(img);
		Image Image(img);
		waitKey(1);
	}
}
void pathfinding(){
  Route route;
  Motor motor;
  Map map(&motor, AMOUNT_LOCATIONS);
  
  //variable to count down for a 360 turn
  unsigned char turnCounter=2;
  //start x, start y, dest x, dest y; 
  int finishCoordinates[][2] = {
    {WIDTH-1, 0},//right top corner
    {WIDTH-1,HEIGHT-1},//right under corner
    {0, HEIGHT-1},//left under corner
    {0,0}//top left corner
  };
  int routeCounter=0;//counter reached locations 
  DirNouse dirNouse/*= EAST*/;

  //init map variables
  int mapNew[HEIGHT][WIDTH];
  int mapOld[HEIGHT][WIDTH];
  int *ptrMap;
  //current location
  
  //int distance sensor
  int *distanceSensorArray = NULL;
  //set map
  /*map.SetMap(dirNouse, distanceSensorArray, curLocationX, curLocationY);
  ptrMap = map.GetMap();
  for(int i =0;i<HEIGHT;i++){
    for(int j=0;j<WIDTH;j++){
      mapNew[i][j] = mapOld[i][j] = *(ptrMap+(WIDTH*i)+j);
    }
  }
  //calculate route
  route.SetRoute(&mapNew[0][0], 0,0, finishCoordinates[0][0], finishCoordinates[0][1]);
  //print routeArray;
  route.PrintRoute();*/

  
  while(1){
    //update the distanceSensor//
/*    dirNouse = map.motor->GetCurrentDirection();
    std::cout << "dirNouse: " << dirNouse << std::endl;*/
    //update map
    map.SetMap();
    ptrMap = map.GetMap();
    for(int i =0;i<HEIGHT;i++){
      for(int j=0;j<WIDTH;j++){
        mapNew[i][j]= *(ptrMap+(WIDTH*i)+j);
      }
    }
    bool mapChanged =  map.CheckDifference(&mapNew[0][0], &mapOld[0][0]);
    //check if map is changed
    if(mapChanged){
      std::cout << "new route" << std::endl;
      for(int i =0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
          mapOld[i][j]= mapNew[i][j];
          
        }
      }

      if(route.SetRoute(&mapOld[0][0], *map.motor->GetCurrentLocation(), *(map.motor->GetCurrentLocation()+1), 
        finishCoordinates[routeCounter][0], finishCoordinates[routeCounter][1]) == 0){
        exit(0);
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
      motor.SetCurrentLocation(x,y);
      motor.SetCurrentDirection((DirNouse)ownDir);
      sleep(2);
    #endif
    #ifdef AUTOMATIC
      map.motor->CalculateCurrentLocationWithRoute( route.GetRoute(), route.GetSize());
      std::cout << "cur direction" << map.motor->GetCurrentDirection()<<std::endl;
      std::cout << "cur location" << *map.motor->GetCurrentLocation() << "," << *(map.motor->GetCurrentLocation()+1) << std::endl;
    #endif
    //turn 360 after three tiles
	turnCounter --;
    if(turnCounter==0){
      motor.Drive(TURN360);
      turnCounter = 2;
    }
    if(targetDetected){//if target is detected get cur location + front distanc sensor 
        map.CalculateTargetLocation(routeCounter, *map.motor->GetCurrentLocation(),*(map.motor->GetCurrentLocation()+1),dirNouse);//front camera = +1
    }
    //ask for target hit
    if(map.GetTargetHit(routeCounter) && routeCounter < AMOUNT_LOCATIONS){
      routeCounter++;
    }
    
  }
}
