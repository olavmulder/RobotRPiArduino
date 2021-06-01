#include "../header/DistanceSensor.h"

int* DistanceSensor::GetDistance(){
    return &distance[0];
}
void DistanceSensor::SetDistance(int d, int i){
  distance[i] = d;
}
void DistanceSensor::ReadDistanceValue(){
    i2c.OpenBus();
    for(unsigned char i=0;i<3;i++){
      std::cout << "write " << i <<" to arduino to read distance value: ";
      i2c.WriteBytes(i);
      sleep(1);
      int data = i2c.ReadBytes();
      SetDistance(data,i);
      std::cout << data << std::endl;
      sleep(1);
    }
    i2c.CloseBus();
}
