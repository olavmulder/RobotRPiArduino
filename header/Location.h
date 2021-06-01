#ifndef LOCATION_H
#define LOCATION_H

class Location{
    private:
        int x,y;
    public:
        int GetLocationX();
        int GetLocationY();
        void SetLocation(int x, int y);
};

#endif