#ifndef DATACAMERA_H
#define DATACAMERA_H
#include "enumVar.h"

class DataCamera{
    private:     
        int offset;
        bool hit;
    public:     

        void SetOffset(int);
        int GetOffset();
        void SetHit(bool h);
        bool GetHit();
};

#endif