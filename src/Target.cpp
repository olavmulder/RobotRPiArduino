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
    printf("offset before while = %d\n", *offset);

    while(*offset != 0 && GetHit() == false){
        //printf("while loop calculateTargetlocation\n");
        if(*offset < -10){   
            motor.Drive(RIGHT);
            printf("beetje rechts\n");
            angle+=5;
            *offset = *offset +1;
            printf("offset = %d\n", *offset);
        }
        if(*offset > 10){
            
            motor.Drive(LEFT);
            angle-=5;
            *offset = *offset -1;
            printf("beetje links\n");
            printf("offset = %d\n", *offset);
        }
        if(*offset >= -10 && *offset <= 10 ){
            //fire
            
            printf("angle at fire = %d degree\n", angle);
            I2C i2c;
            i2c.OpenBus();
            i2c.WriteBytes(0x1);
            sleep(1);
            distance = i2c.ReadBytes();
            i2c.CloseBus();
            
            
            printf("distance %d\n", distance);
         
            if(dirNouse == NORTH){
                tilesXas =  sin(angle*3.14159/180)*distance/tileSize;//amount tile x as away from cur x 
                tilesYas =  cos(angle*3.14159/180)*distance/tileSize;
                targetLocationArray.SetLocation(curX+tilesXas, curY-tilesYas);
                /*if(( angle >= 0 && angle <= 90) || (angle < 0 && angle >= -90) ){//V
                    tilesXas =  sin(angle*3.14159/180)*distance/tileSize;//amount tile x as away from cur x 
                    tilesYas =  cos(angle*3.14159/180)*distance/tileSize;
                    targetLocationArray.SetLocation(curX+tilesXas, curY-tilesYas);
                }
                if((angle > 90 && angle < 180) || (angle < -90 && angle >= -180) ){//V
                    tilesXas =  sin(angle*3.14159/180)*distance/tileSize;//amount tile x as away from cur x 
                    tilesYas =  cos(angle*3.14159/180)*distance/tileSize;
                    targetLocationArray.SetLocation(curX+tilesXas, curY+tilesYas);
                }*/
            }
            else if(dirNouse == EAST){
                tilesXas = sin((angle-90)*3.14159/180)*distance/tileSize;//amount tile x as away from cur x 
                tilesYas = cos((angle-90)*3.14159/180)*distance/tileSize;
                targetLocationArray.SetLocation(curX-tilesXas, curY+tilesYas);
                /*if(angle >= 0 && angle <= 90){//V
                    
                    tilesXas = sin((angle-90)*3.14159/180)*distance/tileSize;//amount tile x as away from cur x 
                    tilesYas = cos((angle-90)*3.14159/180)*distance/tileSize;
                    targetLocationArray.SetLocation(curX-tilesXas, curY+tilesYas);
                }
                if(angle > 90 && angle < 180){//V
                    
                    tilesXas =  sin((angle-90)*3.14159/180)*distance/tileSize;//amount tile x as away from cur x 
                    tilesYas =  cos((angle-90)*3.14159/180)*distance/tileSize;
                    targetLocationArray.SetLocation(curX-tilesXas, curY+tilesYas);
                }
                if(angle < 0 && angle >= -90){//V
                    
                    tilesXas =  sin((angle-90)*3.14159/180)*distance/tileSize;//amount tile x as away from cur x 
                    tilesYas =  cos((angle-90)*3.14159/180)*distance/tileSize;
                    targetLocationArray.SetLocation(curX-tilesXas, curY+tilesYas);
                }
                if(angle < -90 && angle >= -180){//V
                    
                    tilesXas =  sin((angle-90)*3.14159/180)*distance/tileSize;//amount tile x as away from cur x 
                    tilesYas =  cos((angle-90)*3.14159/180)*distance/tileSize;
                    targetLocationArray.SetLocation(curX-tilesXas, curY+tilesYas);
                }*/
            }
            else if(dirNouse == SOUTH){
                tilesXas =  (sin((angle-180)*3.14159/180)*distance)/tileSize;//amount tile x as away from cur x 
                tilesYas =  (cos((angle-180)*3.14159/180)*distance)/tileSize;
                targetLocationArray.SetLocation(curX-tilesXas, curY-tilesYas);
                /*if(angle >= 0 && angle <= 90){//V
                    tilesXas =  (sin((angle-180)*3.14159/180)*distance)/tileSize;//amount tile x as away from cur x 
                    tilesYas =  (cos((angle-180)*3.14159/180)*distance)/tileSize;
                    targetLocationArray.SetLocation(curX-tilesXas, curY-tilesYas);
                }
                if(angle > 90 && angle < 180){//V
                    
                    tilesXas =  (sin((angle-180)*3.14159/180)*distance)/tileSize;//amount tile x as away from cur x 
                    tilesYas =  (cos((angle-180)*3.14159/180)*distance)/tileSize;
                    targetLocationArray.SetLocation(curX-tilesXas, curY-tilesYas);
                }
                if(angle < 0 && angle >= -90){//V
                    tilesXas =  (sin((angle-180)*3.14159/180)*distance)/tileSize;//amount tile x as away from cur x 
                    tilesYas =  (cos((angle-180)*3.14159/180)*distance)/tileSize;
                    targetLocationArray.SetLocation(curX-tilesXas, curY-tilesYas);
                }
                if(angle < -90 && angle >= -180){//V
                    
                    tilesXas =  (sin((angle-180)*3.14159/180)*distance)/tileSize;//amount tile x as away from cur x 
                    tilesYas =  (cos((angle-180)*3.14159/180)*distance)/tileSize;
                    targetLocationArray.SetLocation(curX-tilesXas, curY-tilesYas);
                }*/
            }
            else if(dirNouse == WEST){
                tilesXas =  (sin((angle-270)*3.14159/180)*distance)/tileSize;//amount tile x as away from cur x 
                tilesYas =  (cos((angle-270)*3.14159/180)*distance)/tileSize;
                if(curX-tilesXas >0 && curX-tilesXas < WIDTH && curY-tilesYas > 0 && curY-tilesYas < WIDTH){
                    targetLocationArray.SetLocation(curX-tilesXas, curY-tilesYas);
                }
                /*if(angle >= 0 && angle <= 90){//V
                    tilesXas =  (sin((angle-270)*3.14159/180)*distance)/tileSize;//amount tile x as away from cur x 
                    tilesYas =  (cos((angle-270)*3.14159/180)*distance)/tileSize;
                    targetLocationArray.SetLocation(curX-tilesXas, curY-tilesYas);
                }
                if(angle > 90 && angle < 180){//V
                    // angle -=90;
                    tilesXas =  (sin((angle-270)*3.14159/180)*distance)/tileSize;//amount tile x as away from cur x 
                    tilesYas =  (cos((angle-270)*3.14159/180)*distance)/tileSize;
                    targetLocationArray.SetLocation(curX-tilesXas, curY-tilesYas);
                }
                if(angle < 0 && angle >= -90){//V
                    // angle *=-1;
                    tilesXas =  (sin((angle-270)*3.14159/180)*distance)/tileSize;//amount tile x as away from cur x 
                    tilesYas =  (cos((angle-270)*3.14159/180)*distance)/tileSize;
                    targetLocationArray.SetLocation(curX-tilesXas, curY-tilesYas);
                }
                if(angle < -90 && angle >= -180){//x++ y++
                    // angle +=90;
                    // angle*=-1;
                    tilesXas =  (sin((angle-270)*3.14159/180)*distance)/tileSize;//amount tile x as away from cur x 
                    tilesYas =  (cos((angle-270)*3.14159/180)*distance)/tileSize;
                    targetLocationArray.SetLocation(curX-tilesXas, curY-tilesYas);
                }*/

            }                    
            printf("tilesY as: %d, tilesX as: %d\n", tilesYas, tilesXas);
            printf("dir is %d\n", dirNouse);
            printf("cur x %d, cur y %d\n", curX, curY);
            printf("X = %d, Y = %d\n", targetLocationArray.GetLocationX(),targetLocationArray.GetLocationY());
            SetHit();//target is found
        }  
    }    
    return angle;
}