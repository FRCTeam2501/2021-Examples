import time
from adafruit_servokit import ServoKit
from evdev import InputDevice, categorize, ecodes, KeyEvent

gamepad = InputDevice('/dev/input/event0')

for event in gamepad.read_loop():
	if event.type == ecodes.EV_KEY:
		keyevent = categorize(event)
		# print(keyevent)
		if keyevent.keystate == KeyEvent.key_down:
			# print(keyevent.keycode)
			if keyevent.keycode.__contains__('BTN_Y'):
				print("Y")
			elif keyevent.keycode.__contains__('BTN_A'):
				print("A")
			elif keyevent.keycode.__contains__('BTN_B'):
				print("B")
			elif keyevent.keycode.__contains__('BTN_X'):
				print("X")
