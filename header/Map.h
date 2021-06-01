#ifndef MAP_H
#define MAP_H

#define TILE_SIZE 30 //tile size in cm

#define UNKNOWN 0//value in map is tile is UNKNONW
#define CLOSED 1
#define OPEN 2//value in map is tile is opne
#define TARGET 3
#include <vector>
#include "Target.h"
#include "DistanceSensor.h"
#include "Motor.h"
#include <stdint.h>
#include <iostream>
#include <fstream>
class Map{
    private:
        int *map;
        Motor *motor;
        std::vector <Target> target;    
        DistanceSensor ds;
        int arrayDistanceValues[3];
        int locArr[2];
        int amountTargets;
    public:

       
        Map(Motor *motor,int amountTarget);
        //map
        int* GetMap();
        int* SetMap(DirNouse dir, int* addr, int*x, int* y);
        int* GetDistanceArray();
        bool CheckDifference(int*oldMap , int*newMap);
        //target
        int* GetTargetLocation(int id);
        bool GetTargetHit(int id);
        void CalculateTargetLocation(int id, int x, int y, DirNouse dir, int distance);
};

#endif