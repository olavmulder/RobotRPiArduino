#include "../header/Location.h"

int Location::GetLocationX(){
    return x;
}
int Location::GetLocationY(){
    return y;
}
void Location::SetLocation(int X, int Y){
    x = X;
    y = Y;
}