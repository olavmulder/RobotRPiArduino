#ifndef MAP_H
#define MAP_H


#include <vector>
#include "Target.h"
#include "DistanceSensor.h"
#include "Motor.h"
#include <stdint.h>
#include <iostream>
#include <fstream>
#define TILE_SIZE 30 //tile size in cm

#define UNKNOWN 0//value in map is tile is UNKNONW
#define CLOSED 1
#define OPEN 2//value in map is tile is opn
#define TARGET 3

class Map{
    private:
        int *map;
        std::vector <Target> target;  
        std::vector <DistanceSensor> ds;  
        int arrayDistanceValues[3];
        int locArr[2];
        int amountTargets;
        bool changed;
    public:
        
        Motor *motor;
        Map(Motor *motor,int amountTarget);
        //map
        int* GetMap();
        void SetMap( );
        void SetDistanceArray();
        void CheckDifference(int*oldMap , int*newMap);
        //target
        int* GetTargetLocation(Color id);
        bool GetTargetHit(Color id);
        void SetTargetHit(Color id);
        void SetTargetOffset(Color id, int offset);
        void CalculateTargetLocation(Color id, int x, int y, DirNouse dir);
        void SetChanged(bool status);
        bool GetChanged();
        void PrintMap();
};

#endif