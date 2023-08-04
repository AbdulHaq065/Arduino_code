import RPi.GPIO as GPIO
import time

# Set the GPIO mode to BCM
GPIO.setmode(GPIO.BCM)

# Define the GPIO pin connected to the servo
servo_pin = 18

# Set the servo pin as an output
GPIO.setup(servo_pin, GPIO.OUT)

# Create a PWM object with 50Hz frequency
pwm = GPIO.PWM(servo_pin, 50)

# Function to set the servo angle
def set_angle(angle):
    duty_cycle = angle / 18.0 + 2.5
    pwm.ChangeDutyCycle(duty_cycle)
    time.sleep(0.3)

try:
    # Start the PWM with 0 degree angle
    pwm.start(0)

    while True:
        # Rotate the servo from 0 to 180 degrees
        for angle in range(0, 181, 10):
            set_angle(angle)

        # Rotate the servo from 180 to 0 degrees
        for angle in range(180, -1, -10):
            set_angle(angle)

except KeyboardInterrupt:
    # When Ctrl+C is pressed, cleanup and exit
    pwm.stop()
    GPIO.cleanup()
