// #include <Adafruit_DotStar.h>

/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-ultrasonic-sensor-lcd
 */

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 4, 5, 6, 0, 1, 2, 3, 7, POSITIVE);

int trigPin = 10;    // TRIG pin
int echoPin = 11;    // ECHO pin

float duration_us, distance_cm;

void setup() {
  Serial.begin(9600);  
  lcd.begin(16, 2);
  lcd.backlight();          // open the backlight 
  lcd.clear();
  lcd.setCursor(0, 0);  
  pinMode(trigPin, OUTPUT); // config trigger pin to output mode
  pinMode(echoPin, INPUT);  // config echo pin to input mode
}

void loop() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;
  //distance_m = distance_cm/100;

  lcd.clear();
  lcd.setCursor(0, 0); // start to print at the first row
  lcd.print("Distance: ");
  lcd.print(distance_cm);
  Serial.println(distance_cm);

  delay(100);
}
