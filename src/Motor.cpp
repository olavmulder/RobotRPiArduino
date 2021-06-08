#include "../header/Motor.h"

Motor::Motor(){
    currentLocation.SetLocation(0,0);
    counter = 0;
    direction = EAST;
}

void Motor::Drive(DirDrive dir){
    i2c.OpenBus();
    i2c.WriteBytes(dir);
    std::cout << "dir is " << dir << std::endl;
    std::cout<<"waiting for arduino, he's busy" << std::endl;
    
    while(i2c.GetInterrupt() == false);
    std::cout << "intterupt is true"<<std::endl;
    i2c.ResetInterrupt();
    i2c.CloseBus();
}
void Motor::CalculateCurrentLocation(DirNouse dir, int* distanceArr){
    int newX = *(GetCurrentLocation());
    int newY = *(GetCurrentLocation()+1);
    std::cout << "newX, newY: " << +newX<< +newY << std::endl;
    std::cout << "Dir is: " << dir << std::endl;
    if(dir == NORTH){
        if(*(distanceArr+1) < TILE_SIZE-5 || newY ==  0){
            Drive(RIGHT90);
            direction = WEST;
            
        }else{
            Drive(FORWARD);
            currentLocation.SetLocation(newX, newY-1);
        }
    }
    else if(dir == EAST){
        if(*(distanceArr+1)< TILE_SIZE-5 || newX == WIDTH-1){
            Drive(RIGHT90);
            direction = SOUTH;
        }else{
            Drive(FORWARD);
            currentLocation.SetLocation(newX+1, newY);
        }
    }
    else if(dir == SOUTH){
        if(*(distanceArr+1)< TILE_SIZE-5 || newY == HEIGHT-1){
            Drive(RIGHT90);
            direction = WEST;
        }else{
            Drive(FORWARD);
            currentLocation.SetLocation(newX, newY+1);
        }
    }
    else if(dir == WEST){
        if(*(distanceArr+1)< TILE_SIZE-5 || newX == 0){
            Drive(RIGHT90);
            direction = WEST;
        }else{
            Drive(FORWARD);
            currentLocation.SetLocation(newX-1, newY);
        }
    }
    //small correction v1
    /*if(*distanceArr < 5){
        Drive(RIGHT);
    }
    if(*(distanceArr+2) < 5){
        Drive(LEFT);
    }*/


}
void Motor::CalculateCurrentLocationWithRoute(int *array, int size){

    int newX, newY;
    int oldX;
    int oldY;
    for(counter = 0;counter<size;counter+=2){
        newX = *(array+counter);
        newY = *(array+counter+1);
        oldX = *(GetCurrentLocation());
        oldY = *(GetCurrentLocation()+1);
        /* start::
            ------------------------------------------
            |  >                                      |
            |                                         |
            |   \/                                    |
            |                                         |
            |                                         |
            |                                         |
            |                                         |
            -------------------------------------------
        */

        if(newX > oldX){//robot RIGHT90
            if(direction == NORTH){
                Drive(RIGHT90);
                Drive(FORWARD);
                
            }
            if(direction == EAST){
                Drive(FORWARD);
            }
            if(direction == SOUTH){
                Drive(LEFT90);
                Drive(FORWARD);
            }
            if(direction == WEST){
                Drive(TURN360);
                Drive(FORWARD);
            }
            direction = EAST;
            
        }
        if(newY > oldY){//robot down
            if(direction == NORTH){
                Drive(TURN360);
                Drive(FORWARD);
            }
            if(direction == EAST){
                Drive(RIGHT90);
                Drive(FORWARD);
            }
            if(direction == SOUTH){
                Drive(FORWARD);
            }
            if(direction == WEST){
                Drive(LEFT90);
                Drive(FORWARD);
            }
            direction = SOUTH;

        }
        if(newX < oldX){//robot LEFT
            if(direction == NORTH){
                Drive(LEFT90);
                Drive(FORWARD);
            }
            if(direction == EAST){
                Drive(TURN360);  
                Drive(FORWARD);            
            }
            if(direction == SOUTH){
                Drive(RIGHT90);
                Drive(FORWARD);     
            }
            if(direction == WEST){
                Drive(FORWARD);
            }
            direction = WEST;
        }
        if(newY < oldY){//robot up
            if(direction == NORTH){
                Drive(FORWARD);
            }
            if(direction == EAST){
                Drive(LEFT90);
                Drive(FORWARD); 
            }
            if(direction == SOUTH){
                Drive(BACK);    
            }
            if(direction == WEST){
                Drive(RIGHT90);
                Drive(FORWARD);
        }
            direction = NORTH;
        }
        currentLocation.SetLocation(newX, newY);
        sleep(1);
    }
}

int* Motor::GetCurrentLocation(){ 
    arr[0] = currentLocation.GetLocationX();
    arr[1] = currentLocation.GetLocationY();
    return &arr[0];
}
void Motor::SetCurrentLocation(int x, int y){
    currentLocation.SetLocation(x,y);
}
DirNouse Motor::GetCurrentDirection(){
    return direction;
}
void Motor::SetCurrentDirection(DirNouse dir){
    direction = dir;
}