#ifndef I2C_H
#define I2C_H

#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <iostream>
#include <wiringPiI2C.h>



class I2C{
    private:
        int file;
        int data;
    public:
        void CloseBus();
        bool OpenBus();
        int ReadBytes();
        bool WriteBytes(unsigned char);
        
        
};
   
    
#endif