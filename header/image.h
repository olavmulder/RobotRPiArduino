#ifndef IMAGE
#define IMAGE

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>
using namespace cv;
using namespace std;

class Image{
    public:
        // view methods
        void showImage();
        void showProcessed();
        Mat getImage();
        Image(Mat img);

        // update (controller) methods
        void setProcess();

        
    private:
        // state data
        Mat image, processedImage;
        int thresh = 100;
        // view methods
        // update methods
       
};


#endif