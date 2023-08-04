import RPi.GPIO as GPIO
import RPi_I2C_driver
import time

# Check the I2C address using I2C command in Getting Started
mylcd = RPi_I2C_driver.lcd(0x38)
mylcd.backlight(1)
mylcd.lcd_clear()

trigPin = 15
echoPin = 14
PIRPin = 6
buzzerPin = 22

threshold_dist = 100


def distance():
    GPIO.output(trigPin, True)
    time.sleep(0.00001)
    GPIO.output(trigPin, False)

    while GPIO.input(echoPin) == 0:
        pulse_start = time.time()

    while GPIO.input(echoPin) == 1:
        pulse_end = time.time()

    try:
        pulse_duration = pulse_end - pulse_start
    except:
        print("Calibrating")
        return 2000

    distance = pulse_duration * 17150

    distance = round(distance+1.15, 2)

    return distance


def main():
    GPIO.setup(trigPinGER, GPIO.OUT)
    GPIO.setup(echoPin, GPIO.IN)
    GPIO.setup(PIRPin, GPIO.IN)
    GPIO.setup(buzzerPin, GPIO.OUT)

    try:
        while True:
            if (GPIO.input(PIRPin)):
                if distance() <= threshold_dist:
                    mylcd.lcd_clear()
                    mylcd.lcd_display_string("Intruder Detected", 1)
                    GPIO.output(buzzerPin, GPIO.HIGH)
                else:
                    mylcd.lcd_clear()
                    mylcd.lcd_display_string("Motion Detected", 1)
                    mylcd.lcd_display_string("Object far away", 2)
                    GPIO.output(buzzerPin, GPIO.LOW)
            else:
                mylcd.lcd_clear()
                mylcd.lcd_display_string("Nothing Detected", 1)
                GPIO.output(buzzerPin, GPIO.LOW)
            time.sleep(1)

    except KeyboardInterrupt:
        print("\nExiting the program\n")
        GPIO.cleanup()
        exit()


main()
