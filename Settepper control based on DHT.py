from dht11 import DHT11
import RPi.GPIO as GPIO
import time

# Stepper Constants
coil_A_1_pin = 16
coil_A_2_pin = 19
coil_B_1_pin = 20
coil_B_2_pin = 21

sensorPin = 26
sensor = DHT11(pin=sensorPin)

lowSpeed = 10.0 / 1000.0
medSpeed = 5.0 / 1000.0
highSpeed = 2.0 / 1000.0

GPIO.setmode(GPIO.BCM)  # Using BCM numbering
GPIO.setup(coil_A_1_pin, GPIO.OUT)
GPIO.setup(coil_A_2_pin, GPIO.OUT)
GPIO.setup(coil_B_1_pin, GPIO.OUT)
GPIO.setup(coil_B_2_pin, GPIO.OUT)


def setStep(w1, w2, w3, w4):
    GPIO.output(coil_A_1_pin, w1)
    GPIO.output(coil_A_2_pin, w2)
    GPIO.output(coil_B_1_pin, w3)
    GPIO.output(coil_B_2_pin, w4)


def forward(delay, steps):
    for i in range(0, steps):
        setStep(1, 0, 1, 0)
        time.sleep(delay)
        setStep(0, 1, 1, 0)
        time.sleep(delay)
        setStep(0, 1, 0, 1)
        time.sleep(delay)
        setStep(1, 0, 0, 1)
        time.sleep(delay)


try:
    while True:
        result = sensor.read()
        if result.is_valid():
            temperature = round(result.temperature, 2)
            print("Temperature:", temperature)
            if temperature >= 35:
                forward(highSpeed, 1000)
            elif temperature >= 20 and temperature < 35:
                forward(medSpeed, 400)
            elif temperature < 20:
                forward(lowSpeed, 200)

except KeyboardInterrupt:
    GPIO.cleanup()
    exit()
