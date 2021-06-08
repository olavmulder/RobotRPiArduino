#include "../header/grouping.h"

using namespace cv;
using namespace std;



// public view methods
// Swap method to sort vector
void Grouping::setSwap(float *xp, float *yp){
   int temp = *xp; 
   *xp = *yp; 
   *yp = temp;
}
//Selection from minimum to maximum
void Grouping::setSelectionSort(vector<Point2f> arr, int n){
   for (i = 0; i < n-1; i++) { 
      // Find the minimum element in unsorted array 
      min_idx = i; 
      for (j = i+1; j < n; j++) 
         if (arr[j].x < arr[min_idx].x) 
            min_idx = j; 
  
        // Swap the found minimum element with the first element 
        setSwap(&arr[min_idx].x, &arr[i].x); 
		setSwap(&arr[min_idx].y, &arr[i].y); 
    } 
}
//Group all centers with the same value
void Grouping::setGroup(){
   groupCounter = 0;
   for(int i = 0; i < contoursSize; i++){
      if(i+1 == contoursSize){
         continue;
      } if (group.at(i).x < (group.at(i + 1).x + 2) && group.at(i).x > (group.at(i + 1).x - 2) && group.at(i).x != 0){
         mcGroup.at(groupCounter) += group.at(i + 1);
         array.at(groupCounter) += 1;
      } else {
         groupCounter++;
      }
   }
}

void Grouping::setTotalGroups(){
	for (int i = 0; i < groupCounter; i++) {
		if(array.at(i) > 10 && array.at(i) < 20){
			totalGroup.at(i).x = 0;
			totalGroup.at(i).y = 0;
			if(mcGroup.at(i).x != 0 || mcGroup.at(i).y != 0){
				totalGroup.at(i).x = round(mcGroup.at(i).x / array.at(i));
				totalGroup.at(i).y = round(mcGroup.at(i).y / array.at(i));
//				cout << "totalGroup = " << totalGroup.at(i) << "j[i] = "  << array.at(i)  << endl;
			}
		}
	}
}

// // public update (controller) methods
Grouping::Grouping(vector<Point2f> group, vector<vector<Point>> contours_poly, int contoursSize, Mat processed){
   this->group = group;
   this->contoursSize = contoursSize;
   this->contours_poly = contours_poly;
   this->processed = processed;

   mcGroup.resize(contoursSize);
   array.resize(contoursSize);
   totalGroup.resize(contoursSize);
   
   setSelectionSort(group, contoursSize);
   setGroup();
   setTotalGroups();
   
   Target Target(totalGroup, groupCounter, array, contoursSize, contours_poly, processed);
}