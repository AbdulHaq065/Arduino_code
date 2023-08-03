#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
const int trigPin = 10;
const int echoPin = 11;

const int servo_pin = A2;

// const int green_led = 5;
// const int red_led = 6;
// const int blue_led = 9;

const int buzzer = A0;

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

// 1 - green, 2 - red, 3 - off all the LED
// void RGB_LED_status(int choice){
//   switch(choice){
//     case 1: 
//       digitalWrite(green_led, HIGH);
//       digitalWrite(red_led, LOW);
//       break;
//     case 2: 
//       digitalWrite(red_led, HIGH);
//       digitalWrite(green_led, LOW);
//       break;
//     default: 
//       digitalWrite(red_led, LOW);
//       digitalWrite(green_led, LOW);
//       break;    
//   }
// }

int get_distance(){
  float distance, duration;
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

void setup() {
  Serial.begin(9600);
  myservo.attach(servo_pin);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);

  pinMode(buzzer, OUTPUT);
  // pinMode(green_led, OUTPUT);
  // pinMode(red_led, OUTPUT);
  // pinMode(blue_led, OUTPUT);

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Security system");
  lcd.setCursor(0, 1);
  lcd.print("using Arduino");
  delay(2000);
}

int flag = 0;
int tries = 0;
String temp = "";
String pass = "1234"; //default password, might change in the course of program execution.

void password_check(){
  // RGB_LED_status(3);
  if (strcmp(temp.c_str(), pass.c_str()) == 0){
    // RGB_LED_status(1);
    Serial.println("Password matched!!");
    lcd.setCursor(0, 0);
    lcd.print("Password matched :)");
    lcd.setCursor(0, 1);
    lcd.print("WELCOME!!");

    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    myservo.write(90);
    delay(3000);
    myservo.write(0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Security system");
    lcd.setCursor(0, 1);
    lcd.print("using Arduino");
    // RGB_LED_status(3);
    i = 0;
    temp = "";
  }
  else {
    // RGB_LED_status(2);

    tries += 1;
    if (tries >= 3){
      digitalWrite(buzzer, HIGH);
      tries = 0;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Invalid password");
    delay(2000);
    digitalWrite(buzzer, LOW);
    // RGB_LED_status(3);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Security system");
    lcd.setCursor(0, 1);
    lcd.print("using Arduino");
  }
  temp = "";
}

void password_reset(){
  i = 0;
  Serial.println("Password reset mode");
  // RGB_LED_status(3);
  String t = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Old password: ");
  while(i < 4){
    char key = keypad.getKey();
    if (key){
      t += key;
      i += 1;
    }
    // if (strcmp(key, "#") == 0){
    //   t = t.substring(0, t.length() - 1);
    //   i = 10;
    //   break;
    // }
    lcd.setCursor(0, 1);
    lcd.print(t);
    delay(100);
  }
  Serial.print("t: ");
  Serial.println(t);
  Serial.println(strcmp(t.c_str(), pass.c_str()));
  if (strcmp(t.c_str(), pass.c_str()) == 0){
    t = "";
    i = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New password: ");
    while(i < 4){
      char key = keypad.getKey();
      if (key){
        t += key;
        i += 1;
      }
      // if (key == "#"){
      //   t = t.substring(0, t.length() - 1);
      //   i = 10;
      //   break;
      // }
      lcd.setCursor(0, 1);
      lcd.print(t);
      delay(100);
    }
    // if (t.length() == 4){
    pass = t;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New password:");
    // RGB_LED_status(1);
    lcd.setCursor(0, 1);
    lcd.print(pass);
    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Security system");
    lcd.setCursor(0, 1);
    lcd.print("using Arduino");
    // RGB_LED_status(3);
    i = 0;
    t = "";
    // }
    // else{
    //   i = 0;
    //   t = "";
    //   RGB_LED_status(2);
    //   lcd.clear();
    //   lcd.setCursor(0, 0);
    //   lcd.print("password length");
    //   lcd.setCursor(0, 1);
    //   lcd.print("should be 4");
      
    //   delay(2000);
    //   lcd.clear();      
    //   lcd.setCursor(0, 0);
    //   lcd.print("Security system");
    //   lcd.setCursor(0, 1);
    //   lcd.print("using Arduino");
    //   RGB_LED_status(3);
    // }
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Invalid password");
    // RGB_LED_status(2);
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Security system");
    lcd.setCursor(0, 1);
    lcd.print("using Arduino");
    // RGB_LED_status(3);
  }
}

void loop() {
  myservo.write(0);
  if (get_distance() < 25){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter password: ");
    while (i < 4){
      char key = keypad.getKey();  // Read the key
      if (key){
        temp += key;
        i += 1;
        // lcd.clear();
        Serial.print("Key Pressed : ");
        Serial.println(key);
        lcd.setCursor(0, 1);
        lcd.print(temp.c_str());
      }
      if (i == 4){
        password_check();
        i = 10;
      }
      if (key == '*'){
        password_reset();
        i = 10;
      }

      // Print if key pressed
      // switch (key) {
      //   case '#': 
      //     temp = temp.substring(0,4);
      //     password_check();
      //     i = 10;
      //     break; 
      //   case '*': 
      //     temp = "";
      //     password_reset();
      //     i = 10;
      //     break;
      //   default: 
      //     break;
      // }
    }
    i = 0;
    temp = "";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Security system");
    lcd.setCursor(0, 1);
    lcd.print("using Arduino");
  }
  else{
    i = 0;
  }
}
