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


#define BLUE 0
#define RED 1

#define AMOUNT_TARGETS 2
#define FINISH_LOCATIONS 4

#define DEVICE_ID 0x08
#define MAX_RES_WIDTH 640
#define MAX_RES_HEIGHT 320

void pathfinding(int*, int*);
void openCV(int*, int*);

volatile bool  Maininterrupt = false;
void  MainSetup();
void MainInterrupt();
bool  MainGetInterrupt();
void  MainResetInterrupt();

int main() {
  MainSetup();
  int id;
  int offset = 0;
  std::thread path(pathfinding, &offset, &id);
  std::thread cv(openCV, &offset, &id);
  path.join();
  cv.join();
  return 0;
}

void pathfinding(int* offset, int *id){
  Route route;
  Motor motor;
  Map map(&motor, AMOUNT_TARGETS);
  

  //location to visit, first three fixed, last 2 = the found targetLocations.
  int finishCoordinates[FINISH_LOCATIONS][2] = {
    {WIDTH-1, 0},//right top corner
    {0,0},//top left corner
    {*map.GetTargetLocation(0),*(map.GetTargetLocation(0)+1)},//first target x,y
    {*map.GetTargetLocation(1),*(map.GetTargetLocation(1)+1)}
  };

  //init map variables
  int mapNew[HEIGHT][WIDTH];
  int mapOld[HEIGHT][WIDTH];
  int *ptrMap;

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

  int turnAngleBack=0;
  for(unsigned char i=0;i<FINISH_LOCATIONS;i++){
    while(route.GetstepInRouteCounter()/2 < route.GetSize()){

      map.SetDistanceArray();//update distance values
      if(*offset != 0 && !map.GetTargetHit(id)){//if target is detected
        if(!map.GetTargetHit(id)){//if detected color is already hit, skip this
          printf("color: %d\n", *id);
          turnAngleBack = map.CalculateTargetLocation(id, *map.motor->GetCurrentLocation(),*(map.motor->GetCurrentLocation()+1),map.motor->GetCurrentDirection(), offset);//front camera = +1

          //printf("turn back is done!\n");
        }else{
          printf("%d is already hit\n", *id);
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
          route.SetstepInRouteCounter(0);
          if(route.SetRoute(&mapOld[0][0], *map.motor->GetCurrentLocation(), *(map.motor->GetCurrentLocation()+1), 
            finishCoordinates[route.GetRouteCounter()][0], finishCoordinates[route.GetRouteCounter()][1]) == 0){
              
          }
        }
        //print route & map
        route.PrintRoute();
        map.PrintMap();
        //drive to next position in route array and set new location
        map.motor->CalculateCurrentLocationWithRoute(route.GetRoute(), route.GetSize(),route.GetstepInRouteCounter());

        //add two(x and y) pos to current stepinRouteCounter  
        route.SetstepInRouteCounter(route.GetstepInRouteCounter()+2);
        
      }
    }
    //at the end of route
    printf("------------end of route---------\n");
    route.SetstepInRouteCounter(0); //reset step in route counter
    printf("get route counter %d", route.GetRouteCounter());
    route.SetRouteCounter(route.GetRouteCounter()+1);//increase route counter
    printf("get route counter after %d", route.GetRouteCounter());
    //set new route to new destination
    printf("make new route\n");
    route.SetRoute(&mapNew[0][0], *map.motor->GetCurrentLocation(), *(map.motor->GetCurrentLocation()+1), finishCoordinates[route.GetRouteCounter()][0], finishCoordinates[route.GetRouteCounter()][1]);
    route.PrintRoute();
  }
}


void  MainSetup(){
	  wiringPiSetup();			// Setup the library
    pinMode(2, INPUT);		// Configure GPIO17 as an input
    wiringPiISR (2, INT_EDGE_RISING,  MainInterrupt) ;

}
void  MainInterrupt(void){
     Maininterrupt = true;
}
bool  MainGetInterrupt(){
    return  Maininterrupt;
}
void  MainResetInterrupt(){
     Maininterrupt = false;
}
