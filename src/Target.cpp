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

int Target::CalculateTargetLocation(int curX, int curY, DirNouse dirNouse, int distance, int tileSize, int *offset){
    int angle=0, tilesXas, tilesYas;
    tileSize -=2;
    printf("offset before while = %d\n", *offset);
    I2C i2c;
    i2c.OpenBus();
    i2c.WriteBytes(11);
    int startOffset = *offset;
    while(*offset != 0 && GetHit() == false){
        //printf("while loop calculateTargetlocation\n");
        
        if(*offset < 300){   
            printf("draai 2 graden left\n");
           //motor.Drive(LEFT);
           
            printf("offset = %d\n", *offset);
            //previousOffset = *offset;
        }
        if(*offset > 340){
            printf("draai 2 graden right\n");
            //motor.Drive(RIGHT);
           
            printf("offset = %d\n", *offset);
        }
        if(*offset >= 300 && *offset <= 340){
            unsigned char dummy;
            unsigned int myAngle = 0;
            myAngle = i2c.ReadBytes();
            printf("myAngle from adruino =%d\n", myAngle);
            if(startOffset < 300){
                printf("turn back right\n");
                std::cin >> dummy;
                myAngle *= -1;
            }else if(startOffset > 340){
                printf("turn back left\n");
                std::cin >> dummy;
            }
            printf("myAngle =%d\n", myAngle);
            angle = myAngle;
            i2c.WriteBytes(0x1);
            sleep(1);
            distance = i2c.ReadBytes();
            i2c.CloseBus();
            
            //distance += tileSize;
            printf("distance %d\n", distance);
            if(dirNouse == NORTH){
                tilesXas =  round(sin(angle*3.14159/180)*distance/tileSize);//amount tile x as away from cur x 
                tilesYas =  round(cos(angle*3.14159/180)*distance/tileSize);
                targetLocationArray.SetLocation(curX+tilesXas, curY-tilesYas);
               
            }
            else if(dirNouse == EAST){
                tilesXas = round(sin((angle-90)*3.14159/180)*distance/tileSize);//amount tile x as away from cur x 
                tilesYas = round(cos((angle-90)*3.14159/180)*distance/tileSize);
                targetLocationArray.SetLocation(curX-tilesXas, curY+tilesYas);
            }
            else if(dirNouse == SOUTH){
                tilesXas =  round((sin((angle-180)*3.14159/180)*distance)/tileSize);//amount tile x as away from cur x 
                tilesYas =  round((cos((angle-180)*3.14159/180)*distance)/tileSize);
                targetLocationArray.SetLocation(curX+tilesXas, curY-tilesYas);
            }
            else if(dirNouse == WEST){
                tilesXas =  round((sin((angle-270)*3.14159/180)*distance)/tileSize);//amount tile x as away from cur x 
                tilesYas =  round((cos((angle-270)*3.14159/180)*distance)/tileSize);
                if(curX-tilesXas >0 && curX-tilesXas < WIDTH && curY-tilesYas > 0 && curY-tilesYas < WIDTH){
                    targetLocationArray.SetLocation(curX-tilesXas, curY-tilesYas);
                }
            }                    
            printf("tilesY as: %d, tilesX as: %d\n", tilesYas, tilesXas);
            printf("dir is %d\n", dirNouse);
            printf("cur x %d, cur y %d\n", curX, curY);
            printf("target: X = %d, Y = %d\n", targetLocationArray.GetLocationX(),targetLocationArray.GetLocationY());
            SetHit();//target is found
        }  
    }    
    return angle;
}
