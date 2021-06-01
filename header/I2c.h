#ifndef I2C_H
#define I2C_H

#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <iostream>
#include <wiringPiI2C.h>
class I2C{
    public:
        void CloseBus();
        bool OpenBus();
        int ReadBytes();
        bool WriteBytes(unsigned char);
        int file;
        int data;
        
};
   
    
#endif