// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HMC5883
// This code is designed to work with the HMC5883_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Compass?sku=HMC5883_I2CS#tabs-0-product_tabset-2#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, HMC5883 I2C address is 0x1E(30)
	ioctl(file, I2C_SLAVE, 0x1E);

	// Select Configuration register A(0x00)
	// Normal measurement configuration, data rate o/p = 0.75 Hz(0x60)
	char config[2] = {0};
	config[0] = 0x00;
	config[1] = 0x60;
	write(file, config, 2);
	// Select Mode register(0x02)
	// Continuous measurement mode(0x00)
	config[0] = 0x02;
	config[1] = 0x00;
	write(file, config, 2);
	sleep(1);

	// Read 6 bytes of data from register(0x03)
	// xMag msb, xMag lsb, zMag msb, zMag lsb, yMag msb, yMag lsb
	char reg[1] = {0x03};
	write(file, reg, 1);
	char data[6] ={0};
	if(read(file, data, 6) != 6)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else
	{
		// Convert the data
		int xMag = (data[0] * 256 + data[1]);
		if(xMag > 32767)
		{
			xMag -= 65536;
		}

		int zMag = (data[2] * 256 + data[3]);
		if(zMag > 32767) 
		{
			zMag -= 65536;
		}

		int yMag = (data[4] * 256 + data[5]);
		if(yMag > 32767) 
		{
			yMag -= 65536;
		}

		// Output data to screen
		printf("Magnetic field in X-Axis : %d \n", xMag);
		printf("Magnetic field in Y-Axis : %d \n", yMag);
		printf("Magnetic field in Z-Axis : %d \n", zMag);
	}
}
