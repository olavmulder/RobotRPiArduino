#ifndef CONTOUREN
#define CONTOUREN

#include <iostream>
#include <cmath>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "image.h"
#include "grouping.h"

using namespace cv;
using namespace std;

class Contouren{
    public:
        // view methods
        


        // update (controller) methods
        void setContouren();
        void setApproxPoly();
        void setMoments();
        void setCenters();
        Contouren(Mat img);        

    private:
        // state data
        int check;
        Mat processed;
        vector<vector<Point>> contours;
        vector<vector<Point>> contours_poly; //resize happens later
        vector<Point> approx;
        
        vector<Moments> mu;
        vector<Point2f> mc;
};


#endif