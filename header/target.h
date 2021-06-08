#ifndef TARGET
#define TARGET

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>
#include "image.h"
#include "contouren.h"
#include "draw.h"

using namespace cv;
using namespace std;

class Target{
    public:
        // view methods
        void setTarget();
        void selectTarget();
       // void drawTarget();

        // update (controller) methods
        Target(vector<Point2f> groepje, int groepjesTeller, vector<int> lijst, int contoursGrootte, vector<vector<Point>> contours_poly, Mat processed);
       
    private:
        // state data
        int i, targetCounter, groepjesTeller, contoursGrootte, target1, target2;
        vector<Point2f> totalGroup;
        vector<Point2f>  targetGroup;
        vector<Point2f>  groepje;
        vector<int> lijst;
        vector<Vec4i> hierarchy;
        vector<vector<Point>> contours_poly;
        Mat processed;
        // view methods
        // update methods
};


#endif