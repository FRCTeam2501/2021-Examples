import time, math
from adafruit_servokit import ServoKit
from evdev import InputDevice, categorize, ecodes, KeyEvent

gamepad = InputDevice('/dev/input/event0')

kit = ServoKit(channels=16)

y = 0
x = 0
left = 0
right = 0

def arcadeDrive(forward, rotation):
	maxInput = math.copysign(max(abs(forward), abs(rotation)), forward)
	if forward >= 0.0:
		if rotation >= 0.0:
			left = maxInput
			right = forward - rotation
		else:
			left = forward + rotation
			right = maxInput
	else:
		if rotation >= 0.0:
			left = forward + rotation
			right = maxInput
		else:
			left = maxInput
			right = forward - rotation
	return (left, right)


for event in gamepad.read_loop():
	if event.type == ecodes.EV_ABS:
		event = categorize(event)
		code = ecodes.bytype[event.event.type][event.event.code]
		if code == 'ABS_Y': # Forward/Backwards
			y = (event.event.value / -128) + 1.0
		elif code == 'ABS_X': # Rotation
			x = (event.event.value / 128) - 1.0
		
		# print(x, y)
		left, right = arcadeDrive(y, x)
		print(left, right)
		
		kit.continuous_servo[0].throttle = left
		kit.continuous_servo[1].throttle = right
		kit.continuous_servo[2].throttle = left
		kit.continuous_servo[3].throttle = right