#include "Motor.h"

Motor::Motor(uint8_t pinPWM, uint8_t pinIN1, uint8_t numberBitsPWM, bool rotationWay) {
    this->pinPWM = pinPWM;
    pinMode(pinPWM, OUTPUT);
    this->pinIN1 = pinIN1;
    pinMode(pinIN1, OUTPUT);
    this->numberBitsPWM = numberBitsPWM;

    maxPWM = (uint16_t)round(pow(2, numberBitsPWM)) - 1;
    analogWriteResolution(numberBitsPWM);
    analogWriteFrequency(pinPWM, idealFrequency(numberBitsPWM));

    pwmValue = 0;
    priorityOrder = false;
    priorityPWMValue = 0;
    rotationWay = true;
}

void Motor::actuate() {
    if (priorityOrder) {
        analogWrite(pinPWM, priorityPWMValue);
    } else {
        analogWrite(pinPWM, pwmValue);
    }
    if (rotationWay) {
        digitalWrite(pinIN1, HIGH);
    } else {
        digitalWrite(pinIN1, LOW);
    }
}

void Motor::stop() {
    priorityPWMValue = 0;
    pwmValue = 0;
    priorityOrder = true;
}

void Motor::resume() { priorityOrder = false; }

void Motor::setPWMValue(float PIDOrder) {
    if (PIDOrder >= 0) {
        rotationWay = true;
    } else {
        rotationWay = false;
    }
    if (!priorityOrder) {
        pwmValue = round(abs(PIDOrder) * maxPWM);
    }
}

double Motor::idealFrequency(uint8_t numberBitsPWM) {
    double frequency;
    switch (numberBitsPWM) {
        case 8:
            frequency = 585937.5;
            break;
        case 9:
            frequency = 292968.75;
            break;
        case 10:
            frequency = 146484.38;
            break;
        case 11:
            frequency = 73242.19;
            break;
        case 12:
            frequency = 36621.09;
            break;
        case 13:
            frequency = 18310.55;
            break;
        case 14:
            frequency = 9155.27;
            break;
        case 15:
            frequency = 4577.64;
            break;
        default:
            frequency = 0;
            break;
    }
    return frequency;
}
