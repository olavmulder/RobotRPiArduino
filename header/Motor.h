#ifndef Motor_H
#define Motor_H
#include "I2c.h"
#include "Location.h"
#include "enumVar.h"
#include <iostream>
#include <wiringPi.h>
#define TILE_SIZE 30 //tile size in cm
class Motor{
    private:
        Location currentLocation;
        //Route route;
        int arr[2];
        DirNouse direction;
        I2C i2c;
        int counter;//size of routeArray
    public:
        Motor(); 
        void Drive(DirDrive dir);
        void CalculateCurrentLocationWithRoute(int* array, int size);
        void CalculateCurrentLocation(DirNouse dir, int *distanceArr);
        int* GetCurrentLocation();
        void SetCurrentLocation(int x, int y);
        DirNouse GetCurrentDirection();
        void SetCurrentDirection(DirNouse dir);

};

#endif