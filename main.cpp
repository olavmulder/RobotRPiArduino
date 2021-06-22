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

//#define TURN
#define BLUE 0
#define RED 1

#define AMOUNT_TARGETS 2
#define FINISH_LOCATIONS 4
using namespace cv;
using namespace std;

#define DEVICE_ID 0x08
#define MAX_RES_WIDTH 640
#define MAX_RES_HEIGHT 320

void pathfinding(int*, int*);
void openCV(int*, int*);
const int max_value_H = 360/2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;
static void on_low_H_thresh_trackbar(int, void *){
    low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *){
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *){
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *){
    high_S = max(high_S, low_S+1);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *){
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *){
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V", window_detection_name, high_V);
}
int main() {
  
  int id;
  int offset = 0;
  std::thread path(pathfinding, &offset, &id);
  std::thread cv(openCV, &offset, &id);
  path.join();
  cv.join();
  return 0;
}
void openCV(int* offset, int *id){

	Mat img, myHSV, frame_threshold;
	VideoCapture cap(0);
  cap.set(CV_CAP_PROP_AUTO_EXPOSURE, 0.25);
	cap.set(CV_CAP_PROP_BRIGHTNESS,0.5);
  cap.set(CV_CAP_PROP_CONTRAST, 0.5);
  cap.set(CV_CAP_PROP_SATURATION, 0.5);

  namedWindow(window_detection_name);
    // Trackbars to set thresholds for HSV values
    createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
    createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
    createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
    createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
    createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);
    
	while(true){
    
		cap.read(img);
		Image Image(img);
		Contouren Contouren(Image.getImage())/*, ContourenBlue(Image.getBlueImage()), ContourenRed(Image.getRedImage())*/;
		Grouping Grouping(Contouren.getCenters(), Contouren.getContoursPoly(), Contouren.getContourSize(), Contouren.getProcessed()) /*, GroupingBlue(ContourenBlue.getCenters(), ContourenBlue.getContoursPoly(), ContourenBlue.getContourSize(), ContourenBlue.getProcessed()), GroupingRed(ContourenRed.getCenters(), ContourenRed.getContoursPoly(), ContourenRed.getContourSize(), ContourenRed.getProcessed())*/;
	  TargetCV TargetCV(Grouping.getTotalGroup(), Grouping.getGroupCounter(), Grouping.getArray(), Contouren.getContourSize(), Contouren.getContoursPoly())/*, TargetCVBlue(GroupingBlue.getTotalGroup(), GroupingBlue.getGroupCounter(), GroupingBlue.getArray(), ContourenBlue.getContourSize(), ContourenBlue.getContoursPoly()), TargetCVRed(GroupingRed.getTotalGroup(), GroupingRed.getGroupCounter(), GroupingRed.getArray(), ContourenRed.getContourSize(), ContourenRed.getContoursPoly())*/;
		Draw Draw(Contouren.getContoursPoly(), TargetCV.getTargetGroups(), Contouren.getProcessed())/*, DrawBlue(ContourenBlue.getContoursPoly(), TargetCVBlue.getTargetGroups(), ContourenBlue.getProcessed()), DrawRed(ContourenRed.getContoursPoly(), TargetCVRed.getTargetGroups(), ContourenRed.getProcessed())*/;
     
    vector<Point2f> undefinedTarget;
    float amountWhiteBlue = 0;
    float amountWhiteRed = 0;
    
    undefinedTarget = TargetCV.getTargetGroups();
    if(undefinedTarget[0].x > 60 && undefinedTarget[0].y > 60){
      int xPos = undefinedTarget[0].x;
      int yPos = undefinedTarget[0].y;

      Image.setRoi(xPos, yPos);
      Image.setPercentageFound();
      amountWhiteBlue = Image.getBluePercentage();
      amountWhiteRed = Image.getRedPercentage();
      Image.showBlueTarget();
      Image.showRedTarget();
    }
    
          
      if(amountWhiteBlue > 10 && amountWhiteBlue != 0){
        cout << "target is blue" << endl;
        *id = BLUE;
        *offset = TargetCV.getOffset();
      }else if(amountWhiteRed > 10 && amountWhiteRed != 0){
        cout << "target is Red" << endl;
        *id = RED;
        *offset = TargetCV.getOffset();
      }else if(amountWhiteRed < 10 && amountWhiteRed < 10 && amountWhiteRed != 0){
        cout << "target is undefined! " << endl;
      }
    
    myHSV = Image.getHSVImage();
    inRange(myHSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
    imshow(window_detection_name, frame_threshold);
    
		waitKey(1);
  }

}
void pathfinding(int* offset, int *id){
  Route route;
  Motor motor;
  Map map(&motor, AMOUNT_TARGETS);
  
  //variable to count down for a 360 turn
  //start x, start y, dest x, dest y; 
  //x,y
  int finishCoordinates[FINISH_LOCATIONS][2] = {
    {WIDTH-1, 0},//right top corner
    {WIDTH-1,HEIGHT-1},//right under corner
    {0, HEIGHT-1},//left under corner
    {0,0}//top left corner
  };
  
  //init map variables
  int mapNew[HEIGHT][WIDTH];
  int mapOld[HEIGHT][WIDTH];
  int *ptrMap;
  
  
  //test function 
  /*int *x= map.motor->GetCurrentLocation();
  int *y= x+1;
  map.SetDistanceArray();//calculate distance
  map.motor->SetCurrentDirection(NORTH);
  map.CalculateTargetLocation(id, *x,*y, map.motor->GetCurrentDirection(), offset);
  std::cout << *map.GetTargetLocation(*id) << "," << *(map.GetTargetLocation(*id)+1)<<std::endl;
  map.SetMap();
  map.PrintMap();
  */

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

  for(unsigned char i=0;i<FINISH_LOCATIONS;i++){
    while(route.GetstepInRouteCounter()/2 <= route.GetSize()){

      map.SetDistanceArray();//update distance values
      if(*offset != 0){//if target is detected
        //check color/id
        
        if(!map.GetTargetHit(id)){//if detected color is already hit, skip this
          printf("color: %d\n", *id);
          map.CalculateTargetLocation(id, *map.motor->GetCurrentLocation(),*(map.motor->GetCurrentLocation()+1),map.motor->GetCurrentDirection(), offset);//front camera = +1
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

          if(route.SetRoute(&mapOld[0][0], *map.motor->GetCurrentLocation(), *(map.motor->GetCurrentLocation()+1), 
            finishCoordinates[route.GetRouteCounter()][0], finishCoordinates[route.GetRouteCounter()][1]) == 0){
          }
        }/*else{
          std::cout << "same route" << std::endl;
        }*/
        //print route & map
        route.PrintRoute();
        map.PrintMap();
        //drive to next position in route array and set new location
        map.motor->CalculateCurrentLocationWithRoute(route.GetRoute(), route.GetSize(),route.GetstepInRouteCounter());

        //add two(x and y pos) to current stepinRouteCounter  
        route.SetstepInRouteCounter(route.GetstepInRouteCounter()+2);
        
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
      }
    }
    //at the end of route
    printf("------------end of route---------\n");
    route.SetstepInRouteCounter(0); //reset step in route counter
    route.SetRouteCounter(route.GetRouteCounter()+1);//increase route counter
    //set new route to new destination
    printf("make new route\n");
    route.SetRoute(&mapNew[0][0], *map.motor->GetCurrentLocation(), *(map.motor->GetCurrentLocation()+1), finishCoordinates[route.GetRouteCounter()][0], finishCoordinates[route.GetRouteCounter()][1]);
    route.PrintRoute();
  }
}
