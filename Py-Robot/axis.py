import time
from adafruit_servokit import ServoKit
from evdev import InputDevice, categorize, ecodes, KeyEvent

gamepad = InputDevice('/dev/input/event0')
x = 0 # rotation
y = 0 # forward/backwards
kit = ServoKit(channels=16)
# 0: left
# 1: right

for event in gamepad.read_loop():
	if event.type == ecodes.EV_ABS:
		event = categorize(event)
		# print(event)
		code = ecodes.bytype[event.event.type][event.event.code]
		if code == 'ABS_Y':
			y = (event.event.value / -128) + 1.0
		elif code == 'ABS_X':
			x = (event.event.value / 128) - 1.0
		
		print(x, y)
		if y >= 0.0:
			if x >= 0.0:
				kit.continuous_servo[0].throttle = max(x, y)
				kit.continuous_servo[1].throttle = y - x
			else:
				kit.continuous_servo[0].throttle = y + x
				kit.continuous_servo[1].throttle = max(x, y)
		else:
			if x >= 0.0:
				kit.continuous_servo[0].throttle = y + x
				kit.continuous_servo[1].throttle = max(x, y)
			else:
				kit.continuous_servo[0].throttle = max(x, y)
				kit.continuous_servo[1].throttle = y - x