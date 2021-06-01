#ifndef ROUTE_H
#define ROUTE_H
#include "Map.h"
#include "Location.h"

#include <bits/stdc++.h>
#include <utility>

using namespace std;
// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;
 
// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;

struct cell {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
};

class Route{
    
    private:
        int *routeArray;
        Location finishLocation;
        int sizeOfPath;
        int grid[HEIGHT][WIDTH];
        int srcX, srcY, destX, destY;
        //function of A*
        bool isTarget(int grid[][WIDTH], int row, int col);
        bool isValid(int row, int col);
        bool isUnBlocked(int grid[][WIDTH], int row, int col);
        bool isDestination(int row, int col, Pair dest);
        double calculateHValue(int row, int col, Pair dest);
        void tracePath(cell cellDetails[][WIDTH], Pair dest);
        void aStarSearch(int grid[][WIDTH], Pair src, Pair dest);
        
    public:
        Route();
        int* GetRoute();
        void SetRoute(int* map, int, int ,int,int);
        int GetSize();
        void PrintRoute();
        
};

#endif