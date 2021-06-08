#ifndef GROUPING
#define GROUPING

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>
#include "image.h"
#include "contouren.h"
#include "target.h"

using namespace cv;
using namespace std;

class Grouping{
    public:
        // view methods

        void setSwap(float *xp, float *yp);
        void setSelectionSort(vector<Point2f> arr, int n);
        Grouping(vector<Point2f> group, vector<vector<Point>> contours_poly, int contoursSize, Mat processed);
        void setGroup();
        void setTotalGroups();
        Grouping();

        // update (controller) methods

    private:
        // state data
        int i, j, min_idx, temp, ncounter, contoursSize, groupCounter;
        vector<int> array;
        vector<Point2f> group;
        vector<Point2f> mcGroup;
        vector<Point2f> totalGroup;
        vector<vector<Point>> contours_poly;
        Mat processed;
        // view methods
        // update methods
};


#endif