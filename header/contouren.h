#ifndef CONTOUREN
#define CONTOUREN

#include <iostream>
#include <cmath>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

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
        
        int getContourSize();
        vector<vector<Point>> getContours();
        vector<vector<Point>> getContoursPoly();
        vector<Moments> getMoments();
        vector<Point2f> getCenters();
        Mat getProcessed();

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