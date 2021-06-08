#include "../header/I2c.h"


void I2C::CloseBus(){
	close(file);
}
bool I2C::OpenBus(){
	file = wiringPiI2CSetup(0x08);
	if(file == -1){
		printf("error setup\n");
		return false;
	}
	//std::cout << "setup success!" << std::endl;
	return true;
}
int I2C::ReadBytes(){
	data = wiringPiI2CRead(file);
	return data;

}
bool I2C::WriteBytes(unsigned char byte){
	if(wiringPiI2CWrite(file, byte)){
		//std::cout << "write success!!"<<std::endl;
		return true;
	}
	return false;
}