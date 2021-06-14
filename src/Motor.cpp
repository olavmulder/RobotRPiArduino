#include "../header/Motor.h"
volatile bool interrupt = false;
void  setup();
void Interrupt();
bool GetInterrupt();
void ResetInterrupt();
void setup(){
	wiringPiSetup();			// Setup the library
    pinMode(0, INPUT);		// Configure GPIO17 as an input
    wiringPiISR (0, INT_EDGE_RISING, Interrupt) ;

}
void Interrupt(void){
    interrupt = true;
}
bool GetInterrupt(){
    return interrupt;
}
void ResetInterrupt(){
    interrupt = false;
}

Motor::Motor(){
    currentLocation.SetLocation(0,0);
    direction = EAST;
    setup();
}

void Motor::Drive(DirDrive dir){
    i2c.OpenBus();
    i2c.WriteBytes(dir);
    //std::cout << "dir is " << dir << std::endl;
    std::cout<< dir <<": waiting for arduino, he's busy" << std::endl;
    while(GetInterrupt() == false);
    std::cout << "intterupt is true"<<std::endl;
    ResetInterrupt();
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
void Motor::CalculateCurrentLocationWithRoute(int *array, int size, int counter){
    //array [x,y]
    int newX, newY;
    int oldX;
    int oldY;
    if(counter < size*2){//coutner = only 1 per pair, 
        newX = *(array+counter);
        newY = *(array+counter+1);
        oldX = *(GetCurrentLocation());
        oldY = *(GetCurrentLocation()+1);
        std::cout << "currentLocation:(" << oldX << "," << oldY << ") -> new location: (" << newX << ","<< newY << ")"<< std::endl;

        if(newX > oldX){//robot RIGHT90
            if(direction == NORTH){
                /*Drive(RIGHT90);
                Drive(FORWARD);*/
                printf("right 90\n");
                printf("forward\n");
            }
            if(direction == EAST){
                //Drive(FORWARD);
                printf("forward\n");

            }
            if(direction == SOUTH){
                /*Drive(LEFT90);
                Drive(FORWARD);*/
                printf("forward\n");

            }
            if(direction == WEST){
                /*Drive(TURN180);
                Drive(FORWARD);*/
                printf("turn 180\n");
                printf("forward\n");
            }
            direction = EAST;
            printf("direction is EAST\n");
            
        }
        if(newY > oldY){//robot down
            if(direction == NORTH){
                /*Drive(TURN180);
                Drive(FORWARD);*/
                printf("TURN 180\n");
                printf("forward\n");
            }
            if(direction == EAST){
                /*Drive(RIGHT90);
                Drive(FORWARD);*/
                printf("right 90\n");
                printf("forward\n");
            }
            if(direction == SOUTH){
               // Drive(FORWARD);
                printf("forward\n");
            }
            if(direction == WEST){
                // Drive(LEFT90);
                // Drive(FORWARD);
                printf("left 90\n");
                printf("forward\n");
            }
            direction = SOUTH;
            printf("direction = south\n");

        }
        if(newX < oldX){//robot LEFT
            if(direction == NORTH){
                // Drive(LEFT90);
                // Drive(FORWARD);
                printf("left 90\n");
                printf("forward\n");
            }
            if(direction == EAST){
                // Drive(TURN180);  
                // Drive(FORWARD);  
                printf("right 180\n");
                printf("forward\n");          
            }
            if(direction == SOUTH){
                // Drive(RIGHT90);
                // Drive(FORWARD); 
                printf("right 90\n");
                printf("forward\n");    
            }
            if(direction == WEST){
                //Drive(FORWARD);
                printf("forward\n");
            }
            direction = WEST;
            printf("direction = west\n");
        }
        if(newY < oldY){//robot up
            if(direction == NORTH){
                //Drive(FORWARD);
                printf("forward\n");
            }
            if(direction == EAST){
                // Drive(LEFT90);
                // Drive(FORWARD); 
                printf("left 90\n");
                printf("forward\n");    
            }
            if(direction == SOUTH){
                // Drive(TURN180);  
                // Drive(FORWARD);  
                printf("right 180\n");
                printf("forward\n");       
            }
            if(direction == WEST){
                // Drive(RIGHT90);
                // Drive(FORWARD);
                printf("right 90\n");
                printf("forward\n");    
            }
            direction = NORTH;
            printf("direction = north\n");
        }
        currentLocation.SetLocation(newX, newY);
        sleep(3);
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