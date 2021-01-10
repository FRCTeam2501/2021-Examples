import time
from adafruit_servokit import ServoKit

kit = ServoKit(channels=16)
kit.continuous_servo[0].set_pulse_width_range(1100, 1950)
kit.continuous_servo[1].set_pulse_width_range(1100, 1950)

while True:
    print("1")
    kit.continuous_servo[0].throttle = 1
    kit.continuous_servo[1].throttle = -1
    time.sleep(5)
    print("-1")
    kit.continuous_servo[0].throttle = -1
    kit.continuous_servo[1].throttle = 1
    time.sleep(5)
    print("0")
    kit.continuous_servo[0].throttle = 0
    kit.continuous_servo[1].throttle = 0
    time.sleep(5)