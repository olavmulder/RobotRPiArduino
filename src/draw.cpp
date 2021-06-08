#include "../header/draw.h"

using namespace cv;
using namespace std;



// public view methods


void Draw::drawTarget(){
    	for( int i = 0; i< contours_poly.size(); i++ ){
		drawContours(targetMat, contours_poly, i, color1, 2, 8, hierarchy, 0, Point());
		circle( targetMat, targetGroup[i], 4, color2, -1, 8, 0 );
	}
}

// public update (controller) methods
Draw::Draw(vector<vector<Point>> contours_poly, vector<Point2f>  targetGroup, Mat processed){
    this->contours_poly = contours_poly;
    this->targetGroup = targetGroup;
    this->processed = processed;
    drawTarget();
    imshow( "Contours", targetMat );
}