#include "../header/target_cv.h"

using namespace cv;
using namespace std;



// public view methods
void TargetCV::setTarget(){
    targetCounter = 0;
    for(int i = 0; i < groepjesTeller; i++){
        if(lijst.at(i) > 6 && groepje.at(i).x > 15){
            targetGroup.at(targetCounter) = groepje.at(i);
            targetCounter++;
        }
    }
}

void TargetCV::selectTarget(){
    if(targetCounter > 1){
        if(targetGroup.at(0).x < 320){
            target1 = 320 - targetGroup.at(0).x;
        }
        else{
            target1 = (-1*(320 - targetGroup.at(0).x));
        }
        if(targetGroup.at(1).x < 320){
            target2 = 320 - targetGroup.at(1).x;
        }else{
            target1 = (-1*(320 - targetGroup.at(1).x));
        }
    }else if(targetCounter == 1){
        if(targetGroup.at(0).x < 320){
            target1 = 320 - targetGroup.at(0).x;
        }else{
            target1 = (-1*(320 - targetGroup.at(0).x));
        }
    }
    if(targetCounter != 0){
        if(target2 == 0){
            cout << "target confirmed! Center position = " << targetGroup[0] << endl;
            offset = targetGroup[0].x;
        }else if(target2 < target1){
            cout << "Closest target = target 2! Center position =" << targetGroup[1] << endl;
            offset = targetGroup[0].x;
        }else if(target2 > target1){
            cout << "Closest target = target 1! Center position =" << targetGroup[0] << endl;
            offset = targetGroup[0].x;
        }
    }
}
vector<Point2f> TargetCV::getTargetGroups(){
    return targetGroup;
}
int TargetCV::getOffset(){
    return offset;
}


// public update (controller) methods
TargetCV::Target_CV(vector<Point2f> groepje, int groepjesTeller, vector<int> lijst, int contoursGrootte, vector<vector<Point>> contours_poly){
    this->groepje   = groepje;
    this->lijst     = lijst;
    this->groepjesTeller = groepjesTeller;
    this->contoursGrootte = contoursGrootte;
    this->contours_poly = contours_poly;
    offset = 0;

    targetGroup.resize(contoursGrootte);
    groepje.resize(contoursGrootte);
    lijst.resize(contoursGrootte);
    setTarget();
    selectTarget();
}