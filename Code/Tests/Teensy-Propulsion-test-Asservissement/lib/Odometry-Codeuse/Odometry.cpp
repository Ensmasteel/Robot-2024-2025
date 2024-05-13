#include "Odometry.h"

Switch::Switch(uint8_t pin)
{
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);
    contact = false;
}

void Switch::updateContact()
{
    contact = (digitalRead(pin) == LOW);
}

bool Switch::getContact()
{
    return contact;
}



Odometry::Odometry(Codeuse *codeuseL, Codeuse *codeuseR, float cSpacing, Switch *switchL, Switch *switchR, Kinetic *kinetic){
    this->codeuseLeft=codeuseL;
    this->codeuseRight=codeuseR;
    this->codeusesSpacing=cSpacing;
    this->switchLeft=switchL;
    this->switchRight=switchRight;
    this->kinetics=kinetic;
}


bool Odometry::getContactSwitchL(){
    return switchLeft->getContact();
}

bool Odometry::getContactSwitchR(){
    return switchRight->getContact();
}

void Odometry::updateOdometry(float dt){
    codeuseLeft->actuate(dt);
    codeuseRight->actuate(dt);

    float translationSpeedCodeuses = (codeuseLeft->getTranslationSpeedCodeuse() + codeuseRight->getTranslationSpeedCodeuse()) / 2;
    float rotationSpeedCodeuses = (codeuseRight->getTranslationSpeedCodeuse() - codeuseLeft->getTranslationSpeedCodeuse()) / this->codeusesSpacing;
    float thetaCodeuses = (codeuseRight->getDeltaAvance() - codeuseLeft->getDeltaAvance()) / this->codeusesSpacing; 
    
    kinetics->setTranslationSpeed(translationSpeedCodeuses);
    kinetics->setRotationSpeed(rotationSpeedCodeuses);
    kinetics->setTheta(thetaCodeuses+kinetics->getTheta());
    kinetics->normalizeTheta();
    
    float moyenneAvanceCodeuses = (codeuseRight->getDeltaAvance() + codeuseLeft->getDeltaAvance()) / 2;
    
    kinetics->setX( kinetics->getX() + moyenneAvanceCodeuses*(cos(kinetics->getTheta())));
    kinetics->setY( kinetics->getY() + moyenneAvanceCodeuses*(sin(kinetics->getTheta())));
}


