#ifndef IMAGE
#define IMAGE

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "../header/contouren.h"

#include <iostream>
#include <cmath>
using namespace cv;
using namespace std;

class Image{
    public:
        // view methods
        void showImage();
        void showProcessed();
        void showRed();
        void showBlue();
        void showBlueTarget();
        void showRedTarget();
        Mat getImage();
        Mat getBlueImage();
        Mat getRedImage();
        Mat getHSVImage();
        float getBluePercentage();
        float getRedPercentage();
        void redFilter();
        void blueFilter();
        
        Image(Mat img);

        // update (controller) methods
        void setProcess();
        void setColorProcess();
        void setRoi(int xPos, int yPos);
        void setPercentageFound();
        
    private:
        // state data
        Mat image, processedImage, redOnly, blueOnly, frame_HSV, mask1, mask2, roiImageRed, roiImageBlue;
        vector<Point> black_pixelsBlue, black_pixelsRed;
        
        int thresh = 100;
        int xPos;
        int yPos;
        int startPosX = 0;
        int startPosY = 0;
        float totalWhitePixelsBlue, totalWhitePixelsRed, totalPixels ,totalPixelsRed;
        float amountWhiteBlue = 0;
        float amountWhiteRed = 0;
      
        Rect roi;
        // view methods
        // update methods
       
};


#endif
