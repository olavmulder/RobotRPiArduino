#include "../header/contouren.h"
#include "../header/grouping.h"

using namespace cv;
using namespace std;

// Find al contours 
void Contouren::setContouren(){
    findContours( processed, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
}

//Make lines from connected dots
void Contouren::setApproxPoly(){
    for( int i = 0; i < contours.size(); i++ ){
        approxPolyDP( contours[i], contours_poly[i], 3, true ); //true for closed contours
    }
}

// get the moments -> simular to area()
void Contouren::setMoments(){
    for( int i = 0; i < contours.size(); i++ ){
        mu[i] = moments( contours_poly[i], false );
    }
}
// get center of all moments
void Contouren::setCenters(){
	for( int i = 0; i<contours.size(); i++){ 
		mc[i] = Point2f(round(mu[i].m10/mu[i].m00), round(mu[i].m01/mu[i].m00)); 
		if(isnan(mc[i].x)){//check if mc is real number
			mc[i].x = 0;
			mc[i].y = 0;
		}
	}
}

Contouren::Contouren(Mat img){
    processed = img;
    setContouren();
    contours_poly.resize(contours.size());
    mu.resize(contours.size());
    mc.resize(contours.size());
    setApproxPoly();
    setMoments();
    setCenters();
    Grouping Grouping(mc, contours_poly, contours.size(), processed);
}