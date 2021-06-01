#include "../header/DataCamera.h"

void DataCamera::SetOffset(int s){
    offset = s;
}
int DataCamera::GetOffset(){
    return offset;
}
void DataCamera::SetHit(bool h){
    hit = h;
}
bool DataCamera::GetHit(){
    return hit;
}
