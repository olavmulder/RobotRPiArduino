#include "header/Map.h"
#include "header/Route.h"
#include "header/Motor.h"
#include "header/I2c.h"
#include "header/enumVar.h"

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

typedef enum{

};
Mat src_gray;
Mat img;
int thresh = 100;
RNG rng(12345);

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
  //std::thread cv(openCV);
  path.join();
  //cv.join();
  return 0;
}
int openCV(){
    // string path1 = "/home/jeroen/Downloads/targetBYR.jpg";
	// string path2 = "/home/jeroen/Downloads/multi_targets.jpg";
    // Mat src = imread( path1 );

    // Setup I2C communication
    int fd = wiringPiI2CSetup(DEVICE_ID);
    uint8_t data_to_send;
    uint8_t lastData;
    
    if (fd == -1) {
        std::cout << "Failed to init I2C communication.\n";
        return -1;
    }
    std::cout << "I2C communication successfully setup.\n";

	VideoCapture cap(0);

	while(true){
		cap.read(img);
		cvtColor( img, src_gray, COLOR_BGR2GRAY );
    	blur( src_gray, src_gray, Size(3,3) );
		Mat canny_output;
		Canny( src_gray, canny_output, thresh, thresh*2 );
		vector<vector<Point>> contours;

		findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );

		//recievePackage = thresh_callback();
		Point2f target, prePreviousTarget, previousTarget;
		previousTarget.x = target.x;
		target = thresh_callback(contours, canny_output);	
		
		if(target.x != 0 && (target.x < (previousTarget.x + 5) || target.x > (previousTarget.x - 5))){
			cout << "Target confirmed! Center position = "<< target << endl;
		}

		if(target.x != 0){
			if(target.x < 300 && target.x != 0){
				cout << "Go left" << endl;
				data_to_send = 6; // Go right because mirrored	
				wiringPiI2CWrite(fd, data_to_send);		
			}else if(target.x > 340){
				cout << "Go right" << endl;
				data_to_send = 5; // Go left because mirrored
				wiringPiI2CWrite(fd, data_to_send);
			}else{
				cout << "Fire!!!!!" << endl;
				data_to_send = 8; // stop driving
				wiringPiI2CWrite(fd, data_to_send);								 
			}
		}
//		if(data_to_send != lastData){
//			wiringPiI2CWrite(fd, data_to_send);
//		}
		imshow( "Source", img );

		cout << "********" << endl;

    	waitKey(1);
	}
}
void pathfinding(){
  Motor motor;
  Map map(&motor,AMOUNT_LOCATIONS);
  Route route;
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
  DirNouse dirNouse = WEST;

  //init maps
  int mapNew[HEIGHT][WIDTH];
  int mapOld[HEIGHT][WIDTH];
  int *ptrMap;
  //current location
  int *curLocationX = motor.GetCurrentLocation();
  int *curLocationY = curLocationX+1;
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
    dirNouse = motor.GetCurrentDirection();
    std::cout << "dirNouse: " << dirNouse << std::endl;
    distanceSensorArray = map.GetDistanceArray();
    
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

      if(route.SetRoute(&mapOld[0][0], *curLocationX, *curLocationY, 
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
      motor.CalculateCurrentLocationWithRoute( route.GetRoute(), route.GetSize());
      std::cout << "cur direction" << motor.GetCurrentDirection()<<std::endl;
      std::cout << "cur location" << *motor.GetCurrentLocation() << "," << *(motor.GetCurrentLocation()+1) << std::endl;
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
}

Point2f thresh_callback(vector<vector<Point>> contourVar, Mat cannyVar)
{
	vector<vector<Point>> contours = contourVar;
	Mat canny_output = cannyVar;
	vector<vector<Point>> contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>centers( contours.size() );
	vector<float>radius( contours.size() );
	vector<Vec4i> hierarchy;
	vector<Point2f> mc(contours.size());
	vector<Moments> mu(contours.size());


	// Approximate contours, when contours are closed
    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( contours[i], contours_poly[i], 3, true );
    //    boundRect[i] = boundingRect( contours_poly[i] );
    //    minEnclosingCircle( contours_poly[i], centers[i], radius[i] );
    }
	// Black Mat to show user cirles found
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	
    
	// get the moments of closed contours
	for( int i = 0; i<contours.size(); i++ ){ 
		mu[i] = moments( contours_poly[i], false ); 
	}
	//!!!!!!!!!!!!! check matchShapes() -> https://docs.opencv.org/master/d3/dc0/group__imgproc__shape.html#ga556a180f43cab22649c23ada36a8a139


	// get the centroid of closed contours.
	for( int i = 0; i<contours.size(); i++){ 
		mc[i] = Point2f(round(mu[i].m10/mu[i].m00), round(mu[i].m01/mu[i].m00)); 
		if(isnan(mc[i].x)){
		//	cout << mc[i] << endl;
			mc[i].x = 0;
			mc[i].y = 0;
		}
	}

	//Sort mc for grouping
	int nCounter = contours.size();
	selectionSort(mc, nCounter);

	// Grouping circles with matching centers with 2 pixel buffer
	Point2f mcGroup[contours.size()];
	int j[contours.size()];
	int groupCounter = 0;
	for(int i = 0; i < contours.size(); i++){
		if(mc[i].x < (mc[i + 1].x + 2) && mc[i].x > (mc[i + 1].x - 2) && mc[i].x != 0){
			mcGroup[groupCounter] += mc[i + 1];
			j[groupCounter]+= 1;
		} else {
			groupCounter++;
		}
	}
	
	// To Do: make buffer 5 pixels wide
	Point2f totalGroup[contours.size()];
	for (int i = 0; i < groupCounter; i++) {
		if(j[i] > 10 && j[i] < 20){
			totalGroup[i].x = 0;
			totalGroup[i].y = 0;
			if(mcGroup[i].x != 0 || mcGroup[i].y != 0){
				totalGroup[i].x = round(mcGroup[i].x / j[i]);
				totalGroup[i].y = round(mcGroup[i].y / j[i]);
		//		cout << "totalGroup = " << totalGroup[i] << "j[i] = "  << j[i]  << endl;
			}
		}
	}

	//bool target;
	Point2f targetGroup[contours.size()];
	int targetCounter = 0;
	for(int i = 0; i < groupCounter; i++){
		if(j[i] > 6 && totalGroup[i].y > 50){ // if group is > 6 and ignore top 50 rows pixels
			//target = true;
			targetGroup[targetCounter] = totalGroup[i];
			targetCounter++;
	//		boundRect[i] = boundingRect( contours_poly[i] );
		} else{
			//target = false;
		}
	}

	static int sendPackage[4];

	for(int i = 0; i < (targetCounter +1); i++){
		if(targetGroup[i].x > 50){
			//cout << "Target confirmed! Center position = "<< targetGroup[i] << "Target [" << (i+1) <<"]" << endl;
		}
	}
	int target1, target2;
	if(targetCounter > 1){
		if(targetGroup[0].x < 320){
			target1 = 320 - targetGroup[0].x;
		}
		else{
			target1 = (-1*(320 - targetGroup[0].x));
		}
		if(targetGroup[1].x < 320){
			target2 = 320 - targetGroup[1].x;
		}else{
			target1 = (-1*(320 - targetGroup[1].x));
		}
	}else if(targetCounter == 1){
		if(targetGroup[0].x < 320){
			target1 = 320 - targetGroup[0].x;
		}else{
			target1 = (-1*(320 - targetGroup[0].x));
		}

	}

	// draw contours
	for( int i = 0; i<contours.size(); i++ )
	{
		Scalar color = Scalar(167,151,0); // B G R values
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
//		rectangle( img, boundRect[i].tl(), boundRect[i].br(), color, 2 );
		circle( img, targetGroup[i], 4, color, -1, 8, 0 );
	}

    imshow( "Contours", drawing );

	if(target2 == 0){
		return targetGroup[0];
	}else if(target2 < target1){
		return targetGroup[1];
	}else{
		return targetGroup[0];
	}
}


//Swap function for arrays
void swap(float *xp, float *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
}

//sorting vector<point2f> on x value
void selectionSort(vector<Point2f> arr, int n) 
{ 
    int i, j, min_idx; 
  
    // One by one move boundary of unsorted subarray 
    for (i = 0; i < n-1; i++) 
    { 
        // Find the minimum element in unsorted array 
        min_idx = i; 
        for (j = i+1; j < n; j++) 
        if (arr[j].x < arr[min_idx].x) 
            min_idx = j; 
  
        // Swap the found minimum element with the first element 
        swap(&arr[min_idx].x, &arr[i].x); 
		swap(&arr[min_idx].y, &arr[i].y); 
    } 
}
