#ifndef ENUM_H
#define ENUM_H
    typedef enum {
        green, blue, red
    }TargetID;
    
    typedef enum{
        ONE, TWO, THREE
    }State;

    typedef enum{
        LEFT90 = 3, RIGHT90, LEFT , RIGHT, FORWARD, STOP, BACK, TURN360
    }DirDrive;

    typedef enum{
        NORTH, EAST, SOUTH, WEST
    }DirNouse;
    #define WIDTH 5
    #define HEIGHT 5
#endif