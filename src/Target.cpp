#include "../header/Target.h"


void Target::SetHit(bool hit){
    targetHit = hit;
}
bool Target::GetHit(){
    return targetHit;
}
int* Target::GetTargetLocation(){
    
    locArr[0] = targetLocationArray.GetLocationX();
    locArr[1] = targetLocationArray.GetLocationY();
    return &locArr[0];
}
void Target::SetTargetLocation(int x, int y){
    targetLocationArray.SetLocation(x,y);
}
void Target::CalculateTargetLocation(int curX, int curY, DirNouse dirNouse, int distance, int tileSize){
    
    I2C i2c;
    //aks for reset angle??

    //turn
    int offset = dataCamera.GetOffset();
    while(offset != 0){ //bigger range??
        offset = dataCamera.GetOffset();
        if(offset < 0){//left
            //turn right;
            i2c.OpenBus();
            i2c.WriteBytes(0x00);
            i2c.CloseBus();
        }
        if(offset > 0){//right
            //turn left
            i2c.OpenBus();
            i2c.WriteBytes(0x00);
            i2c.CloseBus();
        }
    }
    //get turned angle;
    i2c.OpenBus();
    i2c.WriteBytes(11);//ask angle ;
    int angle = i2c.ReadBytes();
    i2c.CloseBus();
    //current location + start direction + 
    //turned angle + distance
    int tilesXas =  ((int)sin(angle)*distance)/tileSize;//amount tile x as away from cur x 
    int tilesYas =  ((int)cos(angle)*distance)/tileSize;
    printf("tilesY as: %d, tilesX as: %d\n", tilesYas, tilesXas);
    printf("angle: %d\n", angle);
    if(dirNouse == NORTH){
        if(angle > 0 && angle <= 90){//x+ y--
            targetLocationArray.SetLocation(curX+tilesXas, curY-tilesYas);
        }
        if(angle > 90 && angle < 180){//x++ y++
            targetLocationArray.SetLocation(curX+tilesXas, curY+tilesXas);
        }
        if(angle < 0 && angle >= -90){//x-- y--
            targetLocationArray.SetLocation(curX-tilesXas, curY-tilesXas);
        }
        if(angle < -90 && angle >= -180){//x-- y++
            targetLocationArray.SetLocation(curX-tilesXas, curY+tilesXas);
        }
    }
    if(dirNouse == EAST){
        if(angle > 0 && angle <= 90){//x+ y++
            targetLocationArray.SetLocation(curX+tilesXas, curY+tilesYas);
        }
        if(angle > 90 && angle < 180){//x-- y++
            targetLocationArray.SetLocation(curX-tilesXas, curY+tilesXas);
        }
        if(angle < 0 && angle >= -90){//x++ y--
            targetLocationArray.SetLocation(curX+tilesXas, curY-tilesXas);
        }
        if(angle < -90 && angle >= -180){//x-- y--
            targetLocationArray.SetLocation(curX-tilesXas, curY-tilesXas);
        }
    }
    if(dirNouse == SOUTH){
        if(angle > 0 && angle <= 90){//x-- y++
            targetLocationArray.SetLocation(curX-tilesXas, curY+tilesYas);
        }
        if(angle > 90 && angle < 180){//x-- y--
            targetLocationArray.SetLocation(curX-tilesXas, curY-tilesXas);
        }
        if(angle < 0 && angle >= -90){//x++ y++
            targetLocationArray.SetLocation(curX+tilesXas, curY+tilesXas);
        }
        if(angle < -90 && angle == -180){//x++ y--
            targetLocationArray.SetLocation(curX+tilesXas, curY-tilesXas);
        }
        
    }
    if(dirNouse == WEST){
        if(angle > 0 && angle <= 90){//x-- y--
            targetLocationArray.SetLocation(curX-tilesXas, curY-tilesYas);
        }
        if(angle > 90 && angle < 180){//x++ y--
            targetLocationArray.SetLocation(curX+tilesXas, curY-tilesXas);
        }
        if(angle < 0 && angle >= -90){//x-- y++
            targetLocationArray.SetLocation(curX-tilesXas, curY+tilesXas);
        }
        if(angle < -90 && angle >= -180){//x++ y++
            targetLocationArray.SetLocation(curX+tilesXas, curY+tilesXas);
        }

    }
}