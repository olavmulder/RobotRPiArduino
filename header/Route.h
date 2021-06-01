#ifndef ROUTE_H
#define ROUTE_H
#include "Map.h"
#include "Location.h"

#include <bits/stdc++.h>
#include <utility>
#define ROW 5
#define COL 5
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
        int grid[ROW][COL];
        int srcX, srcY, destX, destY;
        //function of A*
        
        bool isValid(int row, int col);
        bool isUnBlocked(int grid[][COL], int row, int col);
        bool isDestination(int row, int col, Pair dest);
        double calculateHValue(int row, int col, Pair dest);
        void tracePath(cell cellDetails[][COL], Pair dest);
        void aStarSearch(int grid[][COL], Pair src, Pair dest);
        
    public:
        Route(int a, int b, int c, int d);
        int* GetRoute();
        void SetRoute(int* map, int, int ,int,int);
        int GetSize();
        void PrintRoute();
        
};

#endif