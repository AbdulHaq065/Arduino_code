#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
const int trigPin = 9;
const int echoPin = 10;
int i = 0;
const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 2, 3, 4, 5 };
byte colPins[COLS] = { 6, 7, 8, 9 };

//Create an object of keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//LCD display
LiquidCrystal_I2C lcd(0x20, 4, 5, 6, 0, 1, 2, 3, 7, POSITIVE);

void setup() {
  Serial.begin(9600);
  myservo.attach(10);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);
  pinMode(12, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Control");
  lcd.setCursor(0, 1);
  lcd.print("using Arduino");
  delay(2000);
}

int flag = 0;
int tries = 0;
String temp;
String pass; 
void loop() {
  myservo.write(0);
  char key = keypad.getKey();  // Read the key
  // Print if key pressed
  if (pass.length() == 0 && temp.length() == 0){
    flag = 1;
    lcd.clear();
    Serial.println("Password Change mode");
    lcd.setCursor(0, 0);
    lcd.print("Password Change mode");
  }
  if (key && i <= 4) {
    lcd.clear();
    Serial.print("Key Pressed : ");
    Serial.println(key);
    temp += key;
    i += 1;
    Serial.println(temp);
    lcd.setCursor(0, 0);
    lcd.print(temp.c_str());
  } 
  else if (i == 4) {
    if (strcmp(temp.c_str(), pass.c_str()) == 0){
      Serial.println("Password matched!!");
      lcd.setCursor(0, 0);
      lcd.print("Password matched :)");
      lcd.setCursor(0, 1);
      lcd.print("Door Open!");
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      myservo.write(180);
      delay(3000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access Control");
      lcd.setCursor(0, 1);
      lcd.print("using Arduino");
      i = 0;
      temp = "";
    }
    else if (strcmp(temp.c_str(), "5555") == 0){
      flag = 1;
      Serial.println("Password Change mode");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Password Change mode");
      i = 0;
      temp = "";
    }
    else if (flag == 1 && i == 4){
      pass = temp;
      flag = 0;
      lcd.setCursor(0, 0);
      lcd.print("your new password:");
      lcd.setCursor(0, 1);
      lcd.print(temp);
      delay(3000);
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Access Control");
      lcd.setCursor(0, 1);
      lcd.print("using Arduino");
      i = 0;
      temp = "";
    }
    else{
      tries += 1;
      Serial.println(temp);
      Serial.println("Invalid Password !!");
      lcd.setCursor(0, 0);
      lcd.print("Invalid password :(");
      lcd.setCursor(0, 1);
      lcd.print("Try again!");
      if (tries >= 3){
        digitalWrite(12, HIGH);
        tries = 0;
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        delay(2000);
        digitalWrite(12, LOW);
      }
      delay(3000);
      myservo.write(0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access Control");
      lcd.setCursor(0, 1);
      lcd.print("using Arduino");
      i = 0;
      temp = "";
    }
  } 
}
int dist(){
   digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
}