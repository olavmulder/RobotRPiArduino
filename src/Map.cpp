#include "../header/Map.h"

Map::Map(Motor *mot, int amountTarget){
    motor = mot;
    map = (int*)calloc(HEIGHT*WIDTH, sizeof(int));
    for(uint8_t i=0;i< HEIGHT;i++){
        for(uint8_t j=0;j< WIDTH;j++){
            *(map+(i*WIDTH)+j) = UNKNOWN;
        }
    }
    amountTargets = amountTarget;
    target.resize(amountTargets);
}
int* Map::GetTargetLocation(int id){
  int* x = target.at(id).GetTargetLocation();
  int *y = x+1;
  locArr[0] = *x;
  locArr[1] = *y;
  return &locArr[0];
}
bool Map::GetTargetHit(int id){
    return target.at(id).GetHit();
}
void Map::CalculateTargetLocation(int id, int x, int y, DirNouse dir, int distance){
    target.at(id).CalculateTargetLocation(x,y,dir,distance,TILE_SIZE);
}
int* Map::GetDistanceArray(){
    ds.ReadDistanceValue();
    return ds.GetDistance();
}
int* Map::SetMap(DirNouse dir, int* addrDistance, int *x,int *y){
    //set targets in map
    for(int i=0;i<amountTargets;i++){
        if(*GetTargetLocation(i) != -1){
            int *x = GetTargetLocation(i);
            int *y = x+1;
            *(map+(*y*WIDTH)+*x) = TARGET;
        }
    }
    //get all three distance values
    for(uint8_t i=0;i<3;i++){
        arrayDistanceValues[i] = *(addrDistance+i);
    }
    //convert ds distance to amount of tiles;
    
    int tileDistanceA=1, tileDistanceB=1, tileDistanceC=1;//three var to store amount tiles to block 
    bool state= true;
    while(state){
        if(arrayDistanceValues[0] > TILE_SIZE){tileDistanceA ++;arrayDistanceValues[0]-=TILE_SIZE;}
        else state = false;
    }

    state = true;
    while(state){
        if(arrayDistanceValues[1] > TILE_SIZE){tileDistanceB ++;arrayDistanceValues[1]-=TILE_SIZE;}
        else state = false;
    }
    state = true;
    while(state){
        if(arrayDistanceValues[2] > TILE_SIZE){tileDistanceC ++;arrayDistanceValues[2]-=TILE_SIZE;}
        else state = false;
    }
   
    //get current location 

    
    int numberInMap = *x+(*y * WIDTH);
    std::cout << "cur x: " << *x << " cur y: "<< *y<<std::endl;
    std::cout << "number is map :" << numberInMap << std::endl;

    //determine which ds is map location

    //TODO dynamic field = update map = ,
    //tiles between cur pos & block = OPEn

    //place value in map
    int i = 1;
    printf("tileA:%d, tileB:%d, tileC:%d\n", tileDistanceA, tileDistanceB, tileDistanceC);
    if(dir == NORTH){//0= west, 1 = north, 2 = east
        if((numberInMap-tileDistanceA>= 0) && (numberInMap-tileDistanceA < WIDTH*HEIGHT)){//stay in map memory
            if(*x - tileDistanceA > 0 ){//prevent going out of field
                *(map+numberInMap-tileDistanceA) = CLOSED;
            }
        }
        i=1;
        while(tileDistanceA - i > 0){
            *(map+numberInMap-i)= OPEN;
            i++;
        
        }
        if((numberInMap-(tileDistanceB*WIDTH) >= 0) && (numberInMap-(tileDistanceB*WIDTH) < WIDTH*HEIGHT)){
            *(map+numberInMap-(tileDistanceB*WIDTH)) = CLOSED;
        }
        i=1;
        while(tileDistanceB - i > 0){
            *(map+numberInMap-i*WIDTH)= OPEN;
            i++;
        }
        if((numberInMap+tileDistanceC >= 0) && (numberInMap+tileDistanceC < WIDTH*HEIGHT)){
            if(*x + tileDistanceC < WIDTH ){//prevent going out of field
                *(map+numberInMap+tileDistanceC) = CLOSED;
            }
        } 
        i=1;       
        while(tileDistanceC - i > 0){
            *(map+numberInMap+i)= OPEN;
            i++;
        }
    }
    if(dir == EAST){//0 = north, 1 = east, 2 = south
        if((numberInMap-(tileDistanceA*WIDTH) >= 0) && (numberInMap-(tileDistanceA*WIDTH) < WIDTH*HEIGHT)){
            *(map+numberInMap-(tileDistanceA*WIDTH)) = CLOSED;
            

        }
        i=1;
        while(tileDistanceA - i > 0){
            *(map+numberInMap-i*WIDTH)= OPEN;
            i++;
        }       
        if((numberInMap+tileDistanceB >= 0 ) && (numberInMap+tileDistanceB < WIDTH*HEIGHT)){
            int numberInRow = *x;
            if(numberInRow+tileDistanceB < WIDTH){
                *(map+numberInMap+tileDistanceB) = CLOSED;  
            }
        }
        i=1;
        while(tileDistanceB - i > 0){
            *(map+numberInMap+i)= OPEN;
            i++;
        }
        if((numberInMap+(tileDistanceC*WIDTH) >= 0) && (numberInMap+(tileDistanceC*WIDTH) < WIDTH*HEIGHT)){
            *(map+numberInMap+(tileDistanceC*WIDTH)) = CLOSED;
            
            
        }
        i=1;
        while(tileDistanceC - i > 0){
            *(map+numberInMap+i*WIDTH)= OPEN;
            i++;
        }
        
    }

    if(dir == SOUTH){//0 = east, 1 = south, 2 = west
        if((numberInMap+tileDistanceA < WIDTH*HEIGHT) && (numberInMap+tileDistanceA > 0)){
            if(*x+tileDistanceA < WIDTH){
                *(map+numberInMap+tileDistanceA) = CLOSED;
            }
        }
        i=1;
        while(tileDistanceA - i > 0){
            *(map+numberInMap+i)= OPEN;
            i++;
        }
        if((numberInMap+(tileDistanceB*WIDTH) < WIDTH*HEIGHT) && (numberInMap+(tileDistanceB*WIDTH) >=0 )){
            *(map+numberInMap+(tileDistanceB*WIDTH)) = CLOSED;
        }
        i=1;
        while(tileDistanceB - i > 0){
            *(map+numberInMap+i*WIDTH)= OPEN;
            i++;
        }
        if((numberInMap-tileDistanceC < WIDTH*HEIGHT) && numberInMap-tileDistanceC >=0){
            if(*x -tileDistanceC >=0){
                *(map+numberInMap-tileDistanceC) = CLOSED; 
            }
        }  
        i=1; 
        while(tileDistanceC - i > 0){
            *(map+numberInMap-i)= OPEN;
            i++;
        }         
    }
    if(dir == WEST){//0 = south, 1, west, 2 = north
        if((numberInMap+(tileDistanceA*WIDTH) < WIDTH*HEIGHT) && (numberInMap+(tileDistanceA*WIDTH) >=0)){
            *(map+numberInMap+(tileDistanceA*WIDTH)) = CLOSED;
        }
        i=1;
        while(tileDistanceA - i > 0){
            *(map+numberInMap-i*WIDTH)= OPEN;
            i++;
        }
        if((numberInMap-tileDistanceB < WIDTH*HEIGHT) && (numberInMap-tileDistanceB >= 0)){
            if(*x-tileDistanceB >=0){
                *(map+numberInMap-tileDistanceB) = CLOSED;
            } 
        }
        i=1;
        while(tileDistanceB - i > 0){
            *(map+numberInMap-i)= OPEN;
            i++;
        }
            
        if((numberInMap+(tileDistanceC*WIDTH) < WIDTH* HEIGHT) && (numberInMap+(tileDistanceC*WIDTH) >= 0)){
            *(map+numberInMap+(tileDistanceC*WIDTH)) = CLOSED;
        }
        i=1;
        while(tileDistanceC - i > 0){
            *(map+numberInMap+i*WIDTH)= OPEN;
            i++;
        }
    }
    //place map in map.txt for debug
    std::ofstream myfile;
    /*myfile.open ("../map.txt");
    if(myfile.is_open()){
        for(uint8_t i=0;i< HEIGHT;i++){
            for(uint8_t j=0;j< WIDTH;j++){
                myfile << *(map+(i*WIDTH)+j);
                //myfile << " ";
            }
            myfile << "\n";
        }        
    }
    myfile.close();*/
    *map = OPEN;
    for(uint8_t i=0;i< HEIGHT;i++){
            for(uint8_t j=0;j< WIDTH;j++){
                std::cout << *(map+(i*WIDTH)+j);
                //myfile << " ";
            }
            std::cout << "\n";
    }        
    
    return &arrayDistanceValues[0];

} 
int* Map::GetMap(){
    return &map[0];
}

bool Map::CheckDifference(int* oldMap, int *newMap){
    for(int i=0;i<WIDTH*HEIGHT;i++){
        if(*(oldMap+i) != *(newMap+i))return true;
    }
    return false;
    
}