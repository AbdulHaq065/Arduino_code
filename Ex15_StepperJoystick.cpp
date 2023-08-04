#include <AccelStepper.h>

int joyX = A0;
int joyY = A1;

AccelStepper stepper(8, 10, 12, 11, 13);

void setup()
{
    Serial.begin(9600);
    Serial.println("Move Joystick around to change the speed of the stepper motor");

    pinMode(joyX, INPUT);
    pinMode(joyY, INPUT);

    stepper.setMaxSpeed(1000.0);
    stepper.setAcceleration(100.0);
    stepper.setSpeed(200);
    stepper.moveTo(20000);
}

void loop()
{
    if (analogRead(joyX) < 300 && analogRead(joyY) > 700)
    {
        stepper.setSpeed(200);
    }
    else if (analogRead(joyX) > 700 && analogRead(joyY) > 700)
    {
        stepper.setSpeed(400);
    }
    else if (analogRead(joyX) > 700 && analogRead(joyY) < 300)
    {
        stepper.setSpeed(600);
    }
    else if (analogRead(joyX) < 300 && analogRead(joyY) < 300)
    {
        stepper.setSpeed(800);
    }
    else
    {
        stepper.setSpeed(100);
    }

    if (stepper.distanceToGo() == 0)
    {
        stepper.moveTo(-stepper.currentPosition());
    }
    stepper.run();
}