#ifndef TARGET_H
#define TARGET_H
#include "Location.h"
#include "enumVar.h"
#include "DataCamera.h"
#include "I2c.h"
#include <math.h>
#include <stdbool.h>
#include <vector>
class Target{
    private:
        Location targetLocation;
        DataCamera dataCamera;
        Location targetLocationArray;
        bool targetHit;
        int locArr[2];
    public:
        void SetHit(bool Hit);
        bool GetHit();
        int* GetTargetLocation();
        void SetTargetLocation(int x, int y);
        void CalculateTargetLocation(int curX, int curY, DirNouse, int distance, int tileSize);
};

#endif