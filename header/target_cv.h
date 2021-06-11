#ifndef TARGETCV
#define TARGETCV

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>


using namespace cv;
using namespace std;

class TargetCV{
    public:
        // view methods
        void setTarget();
        void selectTarget();
       // void drawTarget();

        // update (controller) methods
        TargetCV(vector<Point2f> groepje, int groepjesTeller, vector<int> lijst, int contoursGrootte, vector<vector<Point>> contours_poly);
        vector<Point2f> getTargetGroups();   
        int getOffset();  
    private:
        // state data
        int i, targetCounter, groepjesTeller, contoursGrootte, target1, target2, offset;
        vector<Point2f> totalGroup;
        vector<Point2f>  targetGroup;
        vector<Point2f>  groepje;
        vector<int> lijst;
        vector<Vec4i> hierarchy;
        vector<vector<Point>> contours_poly;
        // view methods
        // update methods
};


#endif