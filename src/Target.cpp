#include "../header/Target.h"

Target::Target(){
    SetTargetLocation(-1,-1);
}
void Target::SetHit(){
    targetHit = true;
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
void Target::CalculateTargetLocation(int curX, int curY, DirNouse dirNouse, int distance, int tileSize, int *offset){
    I2C i2c;
    i2c.OpenBus();
    i2c.WriteBytes(11);//ask angle ;
    usleep(30);
    int startAngle = i2c.ReadBytes();
    int angle;
    i2c.CloseBus();
    printf("targetOffset in Calculate: %d\n",offset);
    printf("angle in Calculate: %d\n",startAngle);
/*
    while(offset != 0 && !GetHit()){
        printf("while loop calculateTargetlocation");
        if(offset < -10){   
            //motor.Drive(RIGHT);
            printf("beetje rechts\n");
        }
        if(offset > 10){
            //motor.Drive(LEFT);
            printf("beetje links\n");
        }
        if(offset >= -10 && offset <= 10){
            //fire
            i2c.OpenBus();
            i2c.WriteBytes(11);//ask angle ;
            usleep(30);
            int stopAngle = i2c.ReadBytes();
            i2c.CloseBus();
            int tilesXas =  ((int)sin(angle)*distance)/tileSize;//amount tile x as away from cur x 
            int tilesYas =  ((int)cos(angle)*distance)/tileSize;
            printf("tilesY as: %d, tilesX as: %d\n", tilesYas, tilesXas);
            angle = stopAngle-startAngle;
            printf("angle: %d\n", angle);
            SetHit();//target is found

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
    }    */
}