import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO

import dht11
import json
import time
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.cleanup()

# MQTT broker parameters
MQTT_BROKER = "broker.hivemq.com"
MQTT_PORT = 1883
MQTT_TOPIC = "YOUR_TOPIC_NAME"

# DHT sensor parameters
instance = dht11.DHT11(pin = 14)
result = instance.read()
# MQTT client setup
client = mqtt.Client()

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

def on_publish(client, userdata, mid):
    print("Message Published")

client.on_connect = on_connect
client.on_publish = on_publish

# Connect to MQTT broker
client.connect(MQTT_BROKER, MQTT_PORT, 60)

try:
    while True:
        # Read DHT sensor data
        humidity = result.humidity
        temperature = result.temperature

        if humidity is not None and temperature is not None:
            # Create a JSON payload
            data = {
                "temperature": round(temperature, 2),
                "humidity": round(humidity, 2)
            }

            # Convert to JSON format
            payload = json.dumps(data)

            # Publish the data to the MQTT broker
            client.publish(MQTT_TOPIC, payload)

            print(f"Published: {payload}")

        else:
            print("Failed to retrieve data from sensor.")

        # Wait for some time before publishing the next data
        time.sleep(5)

except KeyboardInterrupt:
    print("Interrupted by user.")
    client.disconnect()
