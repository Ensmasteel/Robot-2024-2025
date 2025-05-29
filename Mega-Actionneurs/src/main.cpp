#include <Arduino.h>
#include <Servo.h>
#include <Stepper.h>
#include "pinSetup.h"
#include "Communication.h"
#include "IDS.h"
#include "Logger.h"

Servo frontRato;
bool frontRatoIsClosed;
Servo backRato;
bool backRatoIsClosed;

/*Stepper frontElevator(200, PIN_FRONT_ELEVATOR_STEP, PIN_FRONT_ELEVATOR_DIR);
bool frontElevatorIsRaised;
Stepper backElevator(200, PIN_BACK_ELEVATOR_STEP, PIN_BACK_ELEVATOR_DIR);
bool backElevatorIsRaised;*/

Communication comTeensy = Communication(&Serial1);
Message msg;

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);

    Logger::setup(&Serial, &Serial, &Serial, false, false, false);

    frontRato.attach(PIN_FRONT_RATO);
    frontRato.write(80);
    frontRatoIsClosed = false;
    backRato.attach(PIN_BACK_RATO);
    backRato.write(20);
    backRatoIsClosed = false;

    pinMode(PIN_FRONT_LEFT_MAGNETS, OUTPUT);
    pinMode(PIN_FRONT_RIGHT_MAGNETS, OUTPUT);
    pinMode(PIN_BACK_LEFT_MAGNETS, OUTPUT);
    pinMode(PIN_BACK_RIGHT_MAGNETS, OUTPUT);
    digitalWrite(PIN_FRONT_LEFT_MAGNETS, HIGH);
    digitalWrite(PIN_FRONT_RIGHT_MAGNETS, HIGH);
    digitalWrite(PIN_BACK_LEFT_MAGNETS, HIGH);
    digitalWrite(PIN_BACK_RIGHT_MAGNETS, HIGH);

    /*frontElevator.setSpeed(3000);
    frontElevator.step(3500);
    frontElevatorIsRaised = false;
    frontElevator.setSpeed(800);
    backElevator.setSpeed(3000);
    backElevator.step(3500);
    backElevatorIsRaised = false;
    backElevator.setSpeed(800);*/
}

void loop() {

    comTeensy.update();

    /* Handle the message from the Teensy */
    if (comTeensy.waitingRX()){
        msg = comTeensy.peekOldestMessage();

        switch(msg.aid) {
            case OpenFrontRato:
                if (msg.did == Todo) frontRato.write(80);
                break;
            case CloseFrontRato:
                if (msg.did == Todo) frontRato.write(0);
                break;
            case OpenBackRato:
                if (msg.did == Todo) backRato.write(100);
                break;
            case CloseBackRato:
                if (msg.did == Todo) backRato.write(20);
                break;/*
            case RaiseFrontMagnets:
                if (msg.did == Todo && !frontElevatorIsRaised) {
                    frontElevator.step(2200);
                    frontElevatorIsRaised = true;
                }
                break;
            case LowerFrontMagnets:
                if (msg.did == Todo && frontElevatorIsRaised) {
                    frontElevator.step(-2200);
                    frontElevatorIsRaised = false;
                }
                break;
            case RaiseBackMagnets:
                if (msg.did == Todo && !backElevatorIsRaised) {
                    backElevator.step(2200);
                    backElevatorIsRaised = true;
                }
                break;
            case LowerBackMagnets:
                if (msg.did == Todo && backElevatorIsRaised) {
                    backElevator.step(-2200);
                    backElevatorIsRaised = false;
                }
                break;*/
            case StartFrontLeftMagnets:
                if (msg.did == Todo) digitalWrite(PIN_FRONT_LEFT_MAGNETS, LOW);
                break;
            case StopFrontLeftMagnets:
                if (msg.did == Todo) digitalWrite(PIN_FRONT_LEFT_MAGNETS, HIGH);
                break;
            case StartFrontRightMagnets:
                if (msg.did == Todo) digitalWrite(PIN_FRONT_RIGHT_MAGNETS, LOW);
                break;
            case StopFrontRightMagnets:
                if (msg.did == Todo) digitalWrite(PIN_FRONT_RIGHT_MAGNETS, HIGH);
                break;
            case StartBackRightMagnets:
                if (msg.did == Todo) digitalWrite(PIN_BACK_RIGHT_MAGNETS, LOW);
                break;
            case StopBackRightMagnets:
                if (msg.did == Todo) digitalWrite(PIN_BACK_RIGHT_MAGNETS, HIGH);
                break;
            case StartBackLeftMagnets:
                if (msg.did == Todo) digitalWrite(PIN_BACK_LEFT_MAGNETS, LOW);
                break;
            case StopBackLeftMagnets:
                if (msg.did == Todo) digitalWrite(PIN_BACK_LEFT_MAGNETS, HIGH);
                break;
            default:
                break;
        }
        comTeensy.popOldestMessage();
    }
}
