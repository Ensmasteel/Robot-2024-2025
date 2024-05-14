#include "Motor.h"

//----------Motor Class----------//

Motor::Motor(uint8_t pinPWM, uint8_t pinIN1, uint8_t pinIN2, uint8_t numberBitsPWM){

    this->pinPWM=pinPWM;
    pinMode(pinPWM,OUTPUT);
    this->pinIN1=pinIN1;
    this->pinIN2=pinIN2;
    pinMode(pinIN1,OUTPUT);
    pinMode(pinIN2, OUTPUT);
    this->numberBitsPWM=numberBitsPWM;

    maxPWM = (uint16_t) round(pow(2,numberBitsPWM)) - 1;
    analogWriteResolution(numberBitsPWM);
    analogWriteFrequency(pinPWM,idealFrequency(numberBitsPWM));

    pwmValue=0;
    priorityOrder=false;
    priorityPWMValue=0;
    rotationWay=true;
}

void Motor::actuate(){

    if (priorityOrder){
        analogWrite(pinPWM,priorityPWMValue);
    }
    else{
        analogWrite(pinPWM,pwmValue);
    }
    if(rotationWay){                  //ATTENTION : Peut dependre dans quel sens les differents moteurs sont montés physiquement (à l'envers il faudra inverser)
        digitalWrite(pinIN1, HIGH); // cf datasheet of motor driver
        digitalWrite(pinIN2, LOW);
    }
    else{
        digitalWrite(pinIN1, LOW); // cf datasheet of motor driver
        digitalWrite(pinIN2, HIGH);
    }
}

void Motor::stop(){
    priorityPWMValue = 0;
    priorityOrder = true;
}

void Motor::resume(){
    priorityOrder = false;
    isRoueLibre = false;
}

void Motor::setPWMValue(float PIDOrder){
    if (PIDOrder>=0){
        rotationWay=true;
    }
    else{
        rotationWay=false;
    }
    if (priorityOrder){
        priorityPWMValue = round(abs(PIDOrder)*maxPWM);
    }
    else{
        pwmValue = round(abs(PIDOrder)*maxPWM);
    }
}

void Motor::roueLibre()
{
    priorityPWMValue = 0;
    priorityOrder = true;
    
    isRoueLibre = true;

    digitalWrite(pinIN1, LOW); // cf datasheet of motor driver
    digitalWrite(pinIN2, LOW);
    analogWrite(pinPWM,0);

}

double Motor::idealFrequency(uint8_t numberBitsPWM){

    double frequency;
    switch (numberBitsPWM)
    {
    case 8:
        frequency = 585937.5;
        break;
    case 9:
        frequency = 292968.75;
        break;
    case 10:
        frequency = 146484.38 ;
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
        break;
    }
    return frequency;
}

//----------End Motor Class----------//