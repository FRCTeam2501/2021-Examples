import time
from adafruit_servokit import ServoKit
from evdev import InputDevice, categorize, ecodes, KeyEvent

gamepad = InputDevice('/dev/input/event0')
x = 0
y = 0
kit = ServoKit(channels=16)

for event in gamepad.read_loop():
	if event.type == ecodes.EV_ABS:
		event = categorize(event)
		# print(event)
		code = ecodes.bytype[event.event.type][event.event.code]
		if code == 'ABS_Y':
			y = event.event.value / -32768
		elif code == 'ABS_X':
			x = event.event.value / 32768
		
		print(x, y)
		kit.continuous_servo[0].throttle = y
		kit.continuous_servo[1].throttle = x