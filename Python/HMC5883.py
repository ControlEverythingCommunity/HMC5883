# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# HMC5883
# This code is designed to work with the HMC5883_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/content/Compass?sku=HMC5883_I2CS#tabs-0-product_tabset-2

import smbus
import time

# Get I2C bus
bus = smbus.SMBus(1)

# HMC5883 address, 0x1E(30)
# Select configuration register A, 0x00(00)
#		0x60(96)	Normal measurement configuration, Data output rate = 0.75 Hz
bus.write_byte_data(0x1E, 0x00, 0x60)
# HMC5883 address, 0x1E(30)
# Select mode register, 0x02(02)
#		0x00(00)	Continuous measurement mode
bus.write_byte_data(0x1E, 0x02, 0x00)

time.sleep(0.5)

# HMC5883 address, 0x1E(30)
# Read data back from 0x03(03), 6 bytes
# X-Axis MSB, X-Axis LSB, Z-Axis MSB, Z-Axis LSB, Y-Axis MSB, Y-Axis LSB
data = bus.read_i2c_block_data(0x1E, 0x03, 6)

# Convert the data
xMag = data[0] * 256 + data[1]
if xMag > 32767 :
	xMag -= 65536

zMag = data[2] * 256 + data[3]
if zMag > 32767 :
	zMag -= 65536

yMag = data[4] * 256 + data[5]
if yMag > 32767 :
	yMag -= 65536

# Output data to screen
print "Magnetic field in X-Axis : %d" %xMag
print "Magnetic field in Y-Axis : %d" %yMag
print "Magnetic field in Z-Axis : %d" %zMag
