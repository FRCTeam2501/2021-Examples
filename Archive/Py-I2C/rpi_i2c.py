import smbus
import time

bus = smbus.SMBus(1)

address = 0x70

def writeNumber(value):
	bus.write_byte(address, value)
	return -1

# send single '0xA' message
writeNumber(int(0x0A))