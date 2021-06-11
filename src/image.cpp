#include <iostream>
#include <cmath>
#include "../header/image.h"
#include "../header/contouren.h"

using namespace cv;
using namespace std;


// public view methods

void Image::showImage() {
    imshow( "Source", image );
}
void Image::showProcessed() {
    imshow( "Processed Image", processedImage );
}
Mat Image::getImage(){
    return processedImage;
}
void Image::setProcess(){
    cvtColor( image, processedImage, COLOR_BGR2GRAY );
    blur( processedImage, processedImage, Size(3,3) );
    Canny( processedImage, processedImage, thresh, thresh*2 );
    erode( processedImage, processedImage, 2 );
}
Image::Image(Mat img){

    this->image = img;
    setProcess();
    showImage();
    //showProcessed();
}