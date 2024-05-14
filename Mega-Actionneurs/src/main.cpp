#include <Arduino.h>
#include <Servo.h>
#include <Stepper.h>
#include "pinSetup.h"
#include "Communication.h"
#include "IDS.h"
#include "Logger.h"

Servo rato;

Stepper elevator(200, PIN_ELEVATOR_STEP, PIN_ELEVATOR_DIR);
bool elevator_raised;

Servo SolarLeft;
Servo SolarRight;
bool IsSolarLeft;
bool IsSolarRight;

Communication comTeensy = Communication(&Serial1);
Message msg;

enum {
    JAUNE,
    BLEU
} teamColor;
bool buttonReleased;

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);

    Logger::setup(&Serial, &Serial, &Serial, false, false, false);

    rato.attach(PIN_CLAWS);
    rato.write(180);

    SolarLeft.attach(PIN_SOLARLEFT);
    SolarRight.attach(PIN_SOLARRIGHT);
    IsSolarLeft = false;
    IsSolarRight = false;
    SolarRight.write(10);
    SolarLeft.write(120);

    elevator.setSpeed(3000);
    elevator.step(3500);
    elevator_raised = false;
    elevator.setSpeed(800);

    pinMode(PIN_MAGNET, OUTPUT);
    digitalWrite(PIN_MAGNET, LOW);

    pinMode(PIN_BOUTON_ECRANT, INPUT);
    teamColor = JAUNE;
    buttonReleased = true;
}

void loop() {

    comTeensy.update();

    /* Handle the message from the Teensy */
    if (comTeensy.waitingRX()){
        msg = comTeensy.peekOldestMessage();

        switch(msg.aid) {
            case OpenClaws:
                if (msg.did == Todo) rato.write(180);
                break;
            case CloseClaws:
                if (msg.did == Todo) rato.write(138);
                break;
            case RaiseClaws:
                if (msg.did == Todo && !elevator_raised) {
                    elevator.step(2200);
                    elevator_raised = true;
                }
                break;
            case LowerClaws:
                if (msg.did == Todo && elevator_raised) {
                    elevator.step(-2200);
                    elevator_raised = false;
                }
                break;
            case StartMagnet:
                if (msg.did == Todo) digitalWrite(PIN_MAGNET, HIGH);
                break;
            case ShutdownMagnet:
                if (msg.did == Todo) digitalWrite(PIN_MAGNET, LOW);
                break;
            case SolarLeftOn:
                if (msg.did == Todo && !IsSolarRight) {
                    SolarLeft.write(30);
                    IsSolarLeft = true;
                }
                break;
            case SolarLeftOff:
                if (msg.did == Todo) {
                    SolarLeft.write(120);
                    IsSolarLeft = false;
                }  
                break;
            case SolarRightOn:
                if (msg.did == Todo && !IsSolarRight) {
                    SolarRight.write(100);
                    IsSolarRight = true;
                }
                break;
            case SolarRightOff:
                if (msg.did == Todo) {
                    SolarRight.write(10);
                    IsSolarRight = false;
                }
                break;
            default:
                break;
        }
        comTeensy.popOldestMessage();
    }

    /* Handle screen's buttons */
    if (digitalRead(PIN_BOUTON_ECRANT) == LOW && buttonReleased) {
        buttonReleased = false;
        if (teamColor == JAUNE) {
            teamColor = BLEU;
            comTeensy.send(newMessageActuator(Arduino, Teensy, SetTeamColorBleu));
        } else {
            teamColor = JAUNE;
            comTeensy.send(newMessageActuator(Arduino, Teensy, SetTeamColorJaune));
        }
    } else {
        if (digitalRead(PIN_BOUTON_ECRANT) == HIGH) {
            buttonReleased = true;
            delay(100);
        }
    }
}
