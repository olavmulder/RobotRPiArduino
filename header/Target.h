#ifndef TARGET_H
#define TARGET_H
#include "Location.h"
#include "enumVar.h"
#include "I2c.h"
#include <math.h>
#include <stdbool.h>
#include "Motor.h"
class Target{
    private:
        Location targetLocationArray;
        bool targetHit;
        int locArr[2];
        
        Motor motor;
    public:
        Target();
        void SetHit();
        bool GetHit();
        int* GetTargetLocation();
        void SetTargetLocation(int x, int y);
        int CalculateTargetLocation(int curX, int curY, DirNouse, int distance, int tileSize, int *offset);
};

#endif