#include "header/Map.h"
#include "header/Route.h"
#include "header/Motor.h"
#include "header/I2c.h"
#include "header/enumVar.h"
#include <iostream>
#include <fstream>

//#define MANUAL
#define AUTOMATIC

#define AMOUNT_LOCATIONS 4
/*
 V  draaien om de 3 tegels
 V   if destenation cell is block pak een cell eerder als destanation 
    - cell onderscheiden van onbekent, vrij, blocked en target
    - iets met states-> als target wordt herkend dan stop met rijden.
    - 2 threads -> global var voor state, dan state checken
    - angel vragen ?? -180/180 kan niet, toch??. 
        twee waardes sturen? 0 & 1 daarna waarde, 1 positief, 0 negatief
*/

//global state:
bool targetDetected =false;
int main() {

  /*2 function for pathfinding & function for computer vision
    -
    -
    //make 2 threads
    //start threads
  */

  //init
  Motor motor;
  Map map(&motor,AMOUNT_LOCATIONS);

  //variable to count down for a 360 turn
  unsigned char turnCounter=2;
  //start x, start y, dest x, dest y; 
  int finishCoordinates[][2] = {
    {WIDTH-1, 0},//right top corner
    {WIDTH-1,HEIGHT-1},//right under corner
    {0, HEIGHT-1},//left under corner
    {0,0}//top left corner
  };
  int routeCounter=0;//counter reached locations f
  Route route;

  DirNouse dirNouse = WEST;

  //init maps
  int mapNew[HEIGHT][WIDTH];
  int mapOld[HEIGHT][WIDTH];
  int *ptrMap;
  //current location
  int *curLocationX = motor.GetCurrentLocation();
  int *curLocationY = curLocationX+1;

  //update distance sensor
  int *distanceSensorArray = map.GetDistanceArray();
  //set map
  map.SetMap(dirNouse, distanceSensorArray, curLocationX, curLocationY);
  ptrMap = map.GetMap();
  for(int i =0;i<HEIGHT;i++){
    for(int j=0;j<WIDTH;j++){
      mapNew[i][j] = mapOld[i][j] = *(ptrMap+(WIDTH*i)+j);
    }
  }
  //calculate route
  route.SetRoute(&mapNew[0][0], 0,0, finishCoordinates[0][0], finishCoordinates[0][1]);
  //print routeArray;
  route.PrintRoute();

  
  while(1){
    //update the distanceSensor//
    dirNouse = motor.GetCurrentDirection();
    std::cout << "dirNouse: " << dirNouse << std::endl;
    distanceSensorArray = map.GetDistanceArray();
    //std::cout << "read:: " << +i2c.ReadBytes() << std::endl;
    //
    
    //update map
    curLocationX = motor.GetCurrentLocation();
    curLocationY = curLocationX+1;
    map.SetMap(dirNouse, distanceSensorArray, curLocationX, curLocationY);
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

      route.SetRoute(&mapOld[0][0], *curLocationX, *curLocationY, 
                    finishCoordinates[routeCounter][0], finishCoordinates[routeCounter][1]);
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
      motor.CalculateCurrentLocationWithRoute( route.GetRoute(), route.GetSize());
    #endif
    //turn 360 after three tiles
    if(turnCounter--==0){
      motor.Drive(TURN360);
      turnCounter = 2;
    }

    if(targetDetected){//if target is detected get cur location + front distanc sensor
        int *distance = map.GetDistanceArray();
        map.CalculateTargetLocation(routeCounter,*curLocationX,*curLocationY,dirNouse,*(distance+1));//front camera = +1

    }
    //ask for target hit
    if(map.GetTargetHit(routeCounter) && routeCounter < AMOUNT_LOCATIONS){
      routeCounter++;
    }
  }

    
  return 0;  // EXIT_SUCCESS
}
    
