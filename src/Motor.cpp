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
    //std::cout<< dir <<": waiting for arduino, he's busy" << std::endl;
    while(GetInterrupt() == false);
    std::cout << "DONE, intterupt is true"<<std::endl;
    ResetInterrupt();
    i2c.CloseBus();
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
                printf("right 90\n");
                Drive(RIGHT90);
                printf("forward\n");
                Drive(FORWARD);
            }
            else if(direction == EAST){
                printf("forward\n");
                Drive(FORWARD);
                
            }
            else if(direction == SOUTH){
                printf("left90\n");
                Drive(LEFT90);
                printf("forward\n");
                Drive(FORWARD);

            }
            else if(direction == WEST){
                printf("turn 180\n");
                Drive(TURN180);
                printf("forward\n");
                Drive(FORWARD);
            }
            direction = EAST;
            //printf("direction is EAST\n");
            
        }
        else if(newY > oldY){//robot down
            if(direction == NORTH){
                printf("TURN 180\n");
                Drive(TURN180);
                printf("forward\n");
                Drive(FORWARD);
            }
            else if(direction == EAST){
                printf("right 90\n");
                Drive(RIGHT90);
                printf("forward\n");
                Drive(FORWARD);
            }
            else if(direction == SOUTH){
                printf("forward\n");
                Drive(FORWARD);
            }
            else if(direction == WEST){
                printf("left 90\n");
                Drive(LEFT90);
                printf("forward\n");
                Drive(FORWARD);
            }
            direction = SOUTH;
            //printf("direction = south\n");

        }
        else if(newX < oldX){//robot LEFT
            if(direction == NORTH){
                printf("left 90\n");
                Drive(LEFT90);
                printf("forward\n");
                Drive(FORWARD);
            }
            else if(direction == EAST){
                printf("right 180\n");
                Drive(TURN180);  
                printf("forward\n");          
                Drive(FORWARD);  
            }
            else if(direction == SOUTH){
                printf("right 90\n");
                Drive(RIGHT90);
                printf("forward\n");    
                Drive(FORWARD); 
            }
            else if(direction == WEST){
                printf("forward\n");
                Drive(FORWARD);
            }
            direction = WEST;
            //printf("direction = west\n");
        }
        else if(newY < oldY){//robot up
            if(direction == NORTH){
                printf("forward\n");
                Drive(FORWARD);
            }
            else if(direction == EAST){
                printf("left 90\n");
                Drive(LEFT90);
                printf("forward\n");    
                Drive(FORWARD); 
            }
            else if(direction == SOUTH){
                printf("left 90\n");
                Drive(TURN180);  
                printf("forward\n");       
                Drive(FORWARD);  
            }
            else if(direction == WEST){
                printf("right 90\n");
                Drive(RIGHT90);
                printf("forward\n");    
                Drive(FORWARD);
            }
            direction = NORTH;
            //printf("direction = north\n");
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