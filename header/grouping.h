#ifndef GROUPING
#define GROUPING

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>


using namespace cv;
using namespace std;

class Grouping{
    public:
        // view methods
        void setGroup();
        void setTotalGroups();
        void setSwap(float *xp, float *yp);
        void setSelectionSort(vector<Point2f> arr, int n);

        Grouping(vector<Point2f> group, vector<vector<Point>> contours_poly, int contoursSize, Mat processed);

        vector<Point2f> getTotalGroup();
        int getGroupCounter();
        vector<int> getArray();
        //Grouping();

        // update (controller) methods

    private:
        // state data
        int i, j, min_idx, temp, ncounter, contoursSize, groupCounter;
        vector<int> array;
        vector<Point2f> group;
        vector<Point2f> mcGroup;
        vector<Point2f> totalGroup;
        vector<vector<Point>> contours_poly;
        // view methods
        // update methods
};


#endif