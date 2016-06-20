// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HMC5883
// This code is designed to work with the HMC5883_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Compass?sku=HMC5883_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// HMC5883 I2C address is 0x1E(30)
#define Addr 0x1E

int xMag = 0, yMag =  0, zMag = 0;
void setup()
{
    // Set variable
    Particle.variable("i2cdevice", "HMC5883");
    Particle.variable("xMag", xMag);
    Particle.variable("yMag", yMag);
    Particle.variable("zMag", zMag);
  
    // Initialise I2C communication as MASTER
    Wire.begin();
    // Initialise Serial Communication, set baud rate = 9600
    Serial.begin(9600);
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select configure register A
    Wire.write(0x00);
    // Set normal measurement configuration, data output rate = 0.75Hz
    Wire.write(0x60);
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select Mode register
    Wire.write(0x02);
    // Set continuous measurement
    Wire.write(0x00);
    // Stop I2C Transmission
    Wire.endTransmission();
    delay(300);
}
    
void loop()
{
    unsigned int data[6];
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write(0x03);
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 6 bytes of data
    Wire.requestFrom(Addr, 6);

    // Read 6 bytes of data
    // xMag msb, xMag lsb, zMag msb, zMag lsb, yMag msb, yMag lsb
    if(Wire.available() == 6)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
        data[2] = Wire.read();
        data[3] = Wire.read();
        data[4] = Wire.read();
        data[5] = Wire.read();
    }
    delay(300);
      
    // Convert the data 
    xMag = ((data[0] * 256) + data[1]);
    if(xMag > 32767)
    {
        xMag -= 65536;
    }
    zMag = ((data[2] * 256) + data[3]);
    if(zMag > 32767)
    {
        zMag -= 65536;
    }
    yMag = ((data[4] * 256) + data[5]);
    if(yMag > 32767)
    {
        yMag -= 65536;
    }
    
    // Output data to dashboard
    Particle.publish("Magnetic Field in X-Axis : ", String(xMag));
    delay(1000);
    Particle.publish("Magnetic Field in Y-Axis : ", String(yMag));
    delay(1000);
    Particle.publish("Magnetic Field in Z-Axis : ", String(zMag));
    delay(1000);
}

