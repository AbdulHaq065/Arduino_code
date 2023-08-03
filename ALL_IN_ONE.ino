#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <NewPing.h>

const int trigPin = 10;
const int echoPin = 11;
const int dhtPin = 12;
int xPin = A0;
int yPin = A1;
// int LED1 = D0;
// int LED2 = 1;
int relay = A2;

float h, t;     
int xAxis, yAxis;
int distance;
String lastKeyPressed = "";
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 2, 3, 4, 5 };
byte colPins[COLS] = { 6, 7, 8, 9 };
unsigned long int lastBlinkTime = 0;
unsigned long int lastBuzzTime = 0;
unsigned long int lastRelayTime = 0;
bool state = HIGH;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x20, 4, 5, 6, 0, 1, 2, 3, 7, POSITIVE);

DHT dht(dhtPin, DHT11);

NewPing sonar(trigPin, echoPin, 300);

String tempMessage, humMessage, disMessage, x_axis, y_axis;

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IOTIF Demo");
  // pinMode(LED1, OUTPUT);
  // pinMode(LED2, OUTPUT);
  // pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  turnOffDevices();
  Serial.println("Connect keypad, DHT11, Ultrasonic and LCD");
  delay(2000);
}
void loop() {
  char customKey = customKeypad.getKey();

  if (String(customKey) != "") {
    Serial.println("Value changed");
    Serial.println(lastKeyPressed);
    lastKeyPressed = customKey;
    turnOffDevices();
  }

  xAxis = analogRead(xPin);
  yAxis = analogRead(yPin);

  h = dht.readHumidity();
  t = dht.readTemperature();

  tempMessage = "Temp: " + String(t) + " C";
  humMessage = "Hum: " + String(h) + " %";
  disMessage = "Distance: " + String(distance) + " cm";
  x_axis = "X axis: " + String(xAxis);
  y_axis = "Y axis: " + String(yAxis);

  distance = sonar.ping_cm();
  if (lastKeyPressed == "1") {
    Serial.print(xAxis);
    Serial.print(" : ");
    Serial.println(yAxis);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(x_axis);
    lcd.setCursor(0, 1);
    lcd.print(y_axis);
    turnOffDevices();
  } else if (lastKeyPressed == "2") {
    Serial.println(h);
    Serial.println(t);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(tempMessage);
    lcd.setCursor(0, 1);
    lcd.print(humMessage);
    turnOffDevices();
  } else if (lastKeyPressed == "3") {
    Serial.println(distance);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(disMessage);
    turnOffDevices();
  } else if (lastKeyPressed == "4") {
    // digitalWrite(buzzer, LOW);
    if (millis() - lastRelayTime > 1000) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Relay");
      digitalWrite(relay, state);
      lastRelayTime = millis();
      state = !state;
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("IOTIF Demo");
  }
  delay(100);
}

void turnOffDevices() {
  // digitalWrite(buzzer, LOW);
  digitalWrite(relay, LOW);
  // digitalWrite(LED1, LOW);
  // digitalWrite(LED2, LOW);
}
