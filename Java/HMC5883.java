// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HMC5883
// This code is designed to work with the HMC5883_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Compass?sku=HMC5883_I2CS#tabs-0-product_tabset-2#tabs-0-product_tabset-2

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class HMC5883
{
	public static void main(String args[]) throws Exception
	{
		// Create I2C bus
		I2CBus Bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, HMC5883 I2C address is 0x1E(30)
		I2CDevice device = Bus.getDevice(0x1E);

		// Select Configuration register A
		// Normal measurement configuration, data rate o/p = 0.75 Hz 
		device.write(0x00, (byte)0x60);
		// Select Mode register
		// Continuous measurement mode
		device.write(0x02, (byte)0x00);
		Thread.sleep(500);

		// Read 6 bytes of data from 0x03(3)
		// xMag msb, xMag lsb, zMag msb, zMag lsb, yMag msb, yMag lsb
		byte[] data = new byte[6];
		device.read(0x03, data, 0, 6);

		// Convert the data
		int xMag = ((data[0] & 0xFF) * 256 + (data[1] & 0xFF));
		if(xMag > 32767)
		{
			xMag -= 65536;
		}

		int zMag = ((data[2] & 0xFF) * 256 + (data[3] & 0xFF));
		if(zMag > 32767) 
		{
			zMag -= 65536;
		}

		int yMag = ((data[4] & 0xFF) * 256 + (data[5] & 0xFF));
		if(yMag > 32767) 
		{
			yMag -= 65536;
		}

		// Output data to screen
		System.out.printf("Magnetic field in X-Axis : %d %n", xMag);
		System.out.printf("Magnetic field in Y-Axis : %d %n", yMag);
		System.out.printf("Magnetic field in Z-Axis : %d %n", zMag);
	}
}
