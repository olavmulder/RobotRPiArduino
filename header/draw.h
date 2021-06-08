#ifndef DRAW
#define DRAW

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>
#include "draw.h"
#include "target.h"
#include "contouren.h"

#define WIDTH 640
#define HEIGHT 480

using namespace cv;
using namespace std;

class Draw{
    public:
        // view methods
        void drawTarget();

        // update (controller) methods
        Draw(vector<vector<Point>> contours_poly, vector<Point2f>  targetGroup, Mat processed );
       
    private:
        // state data
        int i;
        Scalar color1 = Scalar(167,151,0); // B G R values
        Scalar color2 = Scalar(0,100,200); // B G R values
        Mat processed;
        Mat drawing = Mat::zeros( HEIGHT, WIDTH, CV_8UC3 );
        Mat targetMat = Mat::zeros( HEIGHT, WIDTH, CV_8UC3 );
        vector<vector<Point>> contours_poly;
        vector<Point2f>  targetGroup;
        vector<Vec4i> hierarchy;
        // view methods
        // update methods
};


#endif