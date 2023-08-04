import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO
import time

# MQTT broker parameters
MQTT_BROKER = "YOUR_HIVEMQ_BROKER_ADDRESS"
MQTT_PORT = 1883
MQTT_TOPIC_LED = "led_control"
MQTT_TOPIC_BUZZER = "buzzer_control"

# LED and buzzer GPIO pins
LED_PIN = 17  # Replace with the GPIO pin connected to the LED
BUZZER_PIN = 18  # Replace with the GPIO pin connected to the buzzer

# GPIO setup
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)
GPIO.setup(BUZZER_PIN, GPIO.OUT)

# MQTT client setup
client = mqtt.Client()

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(MQTT_TOPIC_LED)
    client.subscribe(MQTT_TOPIC_BUZZER)

def on_message(client, userdata, msg):
    if msg.topic == MQTT_TOPIC_LED:
        if msg.payload == b"on":
            GPIO.output(LED_PIN, GPIO.HIGH)
        elif msg.payload == b"off":
            GPIO.output(LED_PIN, GPIO.LOW)
    elif msg.topic == MQTT_TOPIC_BUZZER:
        if msg.payload == b"on":
            GPIO.output(BUZZER_PIN, GPIO.HIGH)
        elif msg.payload == b"off":
            GPIO.output(BUZZER_PIN, GPIO.LOW)

client.on_connect = on_connect
client.on_message = on_message

# Connect to MQTT broker
client.connect(MQTT_BROKER, MQTT_PORT, 60)

try:
    # Loop to maintain the connection
    client.loop_forever()

except KeyboardInterrupt:
    print("Interrupted by user.")
    GPIO.cleanup()
    client.disconnect()
