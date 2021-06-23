#include <iostream>
#include <cmath>
#include "../header/image.h"

using namespace cv;
using namespace std;


// public view methods

void Image::showImage() {
    imshow( "Source", image );
}
void Image::showProcessed() {
    imshow( "Processed Image", processedImage );
}
void Image::showRed() {
    imshow( "Red Image", redOnly );
}
void Image::showBlue() {
    imshow( "Blue Image", blueOnly );
}
void Image::showBlueTarget() {
    imshow( "Blue Target", roiImageBlue );
}
void Image::showRedTarget() {
    imshow( "Red Target", roiImageRed );
}
Mat Image::getImage(){
    return processedImage;
}
Mat Image::getBlueImage(){
    return blueOnly;
}
Mat Image::getRedImage(){
    return redOnly;
}
Mat Image::getHSVImage(){
    return frame_HSV;
}
float Image::getBluePercentage(){
    return amountWhiteBlue;
}
float Image::getRedPercentage(){
    return amountWhiteRed;
}
void Image::setProcess(){
    cvtColor( image, processedImage, COLOR_BGR2GRAY );
    blur( processedImage, processedImage, Size(3,3) );
    Canny( processedImage, processedImage, thresh, thresh*2 );
    erode( processedImage, processedImage, 2 );
}
void Image::setRoi(int xPos, int yPos){
    startPosX = xPos - 50;
    startPosY = yPos - 50;
    Rect roi(startPosX, startPosY, 100, 100);
    roiImageBlue = blueOnly(roi);
    roiImageRed = redOnly(roi);
}
void Image::setPercentageFound(){
    findNonZero(roiImageBlue, black_pixelsBlue);
    findNonZero(roiImageRed, black_pixelsRed);
    totalWhitePixelsBlue = black_pixelsBlue.size();
    totalWhitePixelsRed = black_pixelsRed.size();
    totalPixels = 100 * 100;
    amountWhiteBlue = totalWhitePixelsBlue / totalPixels * 100;
    amountWhiteRed = totalWhitePixelsRed / totalPixels * 100;
}
void Image::setColorProcess(){
    cvtColor(image, frame_HSV, COLOR_BGR2HSV);
    blur( frame_HSV, frame_HSV, Size(5,5) );
    erode( frame_HSV, frame_HSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( frame_HSV, frame_HSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
}
void Image::redFilter(){
    inRange(frame_HSV, Scalar(103, 60, 85), Scalar(180, 255, 255), redOnly);
    //inRange(frame_HSV, Scalar(160, 0, 0), Scalar(180, 255, 255), mask2);
    //redOnly = mask1 | mask2;
    //Canny( redOnly, redOnly, thresh, thresh*2 );
}
void Image::blueFilter(){
    inRange(frame_HSV, Scalar(78, 120, 83), Scalar(119, 255, 255), blueOnly);
    //Canny( blueOnly, blueOnly, thresh, thresh*2 );
}
Image::Image(Mat img){
    this->image = img;
    setProcess();
    setColorProcess();
    showImage();
    redFilter();
   // showRed();
    blueFilter();
   // showBlue();
}
