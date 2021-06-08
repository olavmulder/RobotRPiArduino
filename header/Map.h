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
    public:

        Motor *motor;
        Map(Motor *motor,int amountTarget);
        //map
        int* GetMap();
        void SetMap();
        void SetDistanceArray();
        bool CheckDifference(int*oldMap , int*newMap);
        //target
        int* GetTargetLocation(int id);
        bool GetTargetHit(int id);
        void CalculateTargetLocation(int id, int x, int y, DirNouse dir);
};

#endif