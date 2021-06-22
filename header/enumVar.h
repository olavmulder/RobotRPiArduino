#ifndef ENUM_H
#define ENUM_H

    typedef enum{
        LEFT90 = 3, RIGHT90, LEFT , RIGHT, FORWARD, STOP, BACK, TURN360, TURN180=12
    }DirDrive;

    typedef enum{
        NORTH, EAST, SOUTH, WEST
    }DirNouse;
    
    #define WIDTH 5
    #define HEIGHT 5
#endif