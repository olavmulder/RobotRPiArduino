#ifndef DistanceSensor_H
#define DistanceSensor_h
#include "enumVar.h"
#include "I2c.h"

class DistanceSensor{
    private: 
        I2C i2c;
        int distance[3];
    public: 
        int* GetDistance();
        void SetDistance(int d, int i);
        void ReadDistanceValue();

};

#endif