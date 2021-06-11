#ifndef DRAW
#define DRAW

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>

#define WIDTH_IMG 640
#define HEIGHT_IMG 480

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
        Mat drawing = Mat::zeros( HEIGHT_IMG, WIDTH_IMG, CV_8UC3 );
        Mat targetMat = Mat::zeros( HEIGHT_IMG, WIDTH_IMG, CV_8UC3 );
        vector<vector<Point>> contours_poly;
        vector<Point2f>  targetGroup;
        vector<Vec4i> hierarchy;
        // view methods
        // update methods
};


#endif