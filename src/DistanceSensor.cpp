#include "../header/DistanceSensor.h"

int DistanceSensor::GetDistance(){
    return distance;
}
void DistanceSensor::SetDistance(int d){
  distance = d;
}
void DistanceSensor::ReadDistanceValue(int id){
    i2c.OpenBus();
    std::cout << "write " << id <<" to arduino to read distance value: ";
    i2c.WriteBytes(id);
    sleep(1);
    SetDistance(i2c.ReadBytes());
    std::cout << GetDistance() << std::endl;
    sleep(1);
    i2c.CloseBus();
   
}
