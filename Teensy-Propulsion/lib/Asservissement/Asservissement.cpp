#include "Asservissement.h"

#define RATIOPID 1000.0

//----------Filtre Class----------//

Filtre::Filtre(float initValue, float frequency) {
    value = initValue;
    wc = 2 * PI * frequency;
}

void Filtre::in(float newValue, float dt) {
    if (dt > 0.5) {  // Temps depuis la derniere execution trop longue
        value = newValue;
    } else {
        value = (value + newValue * wc * dt) / (1 + wc * dt);
    }
}

float Filtre::out() { return value; }

//----------End Filtre Class----------//

//----------MoveProfile Class----------//

void MoveProfile::set(float KP, float KI, float KD, float epsilon, float dEpsilon, float maxErr, float speedRamps, float cruisingSpeed) {
    this->KP = KP / RATIOPID;
    this->KI = KI / RATIOPID;
    this->KD = KD / RATIOPID;
    this->epsilon = epsilon;
    this->dEpsilon = dEpsilon;
    this->maxError = maxError;
    this->speedRamps = speedRamps;
    this->cruisingSpeed = cruisingSpeed;
}

//----------End MoveProfile Class----------//

//----------MoveProfilesSetup Class----------//

MoveProfile MoveProfilesSetup::profiles[2 * __NBPROFILES__];

void MoveProfilesSetup::setup() {
    get(off, true)->set(1.0, 1.0, 1.0, 0.01, 0.01, 100.0, 0.5, 1.0);
    get(off, false)->set(1.0, 1.0, 1.0, 0.01, 0.01, 100.0, 0.5, 1.0);

    get(brake, true)->set(0.0, 0.0, 0.0, 100.0, 100.0, 100.0, 0.001, 0.001);
    get(brake, false)->set(0.0, 0.0, 0.0, 100.0, 100.0, 100.0, 0.001, 0.001);

    get(accurate,true)->set(32000.0 , 0.01 , 0.1 , 0.01 , 0.003 , 0.03 , 0.6 , 0.6);           //translation
    get(accurate,false)->set(2200.0 , 0.02, 0.05 , 0.05 , 0.01 , 20*0.017 , 3.0/2.0*PI , 2*PI);   //rotation

    get(standard, true)->set(32000.0f, 0.01f, 0.1f, 0.005f, 0.003f, 0.01f, 0.6f, 0.6f);                     // translation
    get(standard, false)->set(4000.0f , 0.02f, 0.05f, 0.05f , 0.01f , 20*0.017f , 3.0/2.0*PI , 2*PI);  // rotation

    get(fast, true)->set(0.0, 0.0, 0.0, 100.0, 100.0, 100.0, 0.001, 0.001);
    get(fast, false)->set(0.0, 0.0, 0.0, 100.0, 100.0, 100.0, 0.001, 0.001);

    get(recalibrate, true)->set(0.0, 0.0, 0.0, 100.0, 100.0, 100.0, 0.001, 0.001);
    get(recalibrate, false)->set(0.0, 0.0, 0.0, 100.0, 100.0, 100.0, 0.001, 0.001);
}

MoveProfile *MoveProfilesSetup::get(MoveProfileName name, bool translation) {
    return &profiles[2 * (int)name + ((translation) ? 0 : 1)];  // en fonction de l'ordre de definition dans la classe enumeree MoveProfileName
}

//----------End MoveProfilesSetup Class----------//

//----------PID Class----------//

PID::PID(bool transla, float frequency) {
    translation = transla;
    dxF = new Filtre(0, frequency);
    close = true;
    tooFar = false;
    currentProfile = MoveProfilesSetup::get(standard, transla);

    iTerm = 0;
}

void PID::setCurrentProfile(MoveProfileName profile) { currentProfile = MoveProfilesSetup::get(profile, translation); }

void PID::reset() { iTerm = 0; }

bool PID::getClose() { return close; }

bool PID::getTooFar() { return tooFar; }

MoveProfile *PID::getCurrentProfile() { return currentProfile; }

float PID::compute(float x, float dx, float xTarget, float dxTarget, float dt) {
    float diffX;   // difference de position (angulaire ou de translation en fonction du PID)
    float diffDX;  // difference de vitesse (angulaire ou de translation en fonction du PID)
    if (translation) {
        diffX = xTarget - x;
    } else {
        diffX = normalizeAngle(xTarget - x);
    }

    dxF->in(dx, dt);
    diffDX = dxTarget - dxF->out();
    iTerm += diffX * dt;

    close = (abs(diffX) <= currentProfile->epsilon) && (abs(diffDX) <= currentProfile->dEpsilon);
    float out = constrain(currentProfile->KP * diffX + currentProfile->KI * iTerm + currentProfile->KD * (dxTarget - dxF->out()), -1.0, 1.0);

    return out;
}

//----------End PID Class----------//

//----------Asservissement Class------------//

Asservissement::Asservissement(float *translaOrder, float *rotaOrder, Kinetic *kRobot, Kinetic *kGhost, const float frequency) {
    pidTranslation = new PID(true, frequency);
    pidRotation = new PID(false, frequency);

    this->translationOrder = translaOrder;
    this->rotationOrder = rotaOrder;
    this->kineticRobot = kRobot;
    this->kineticGhost = kGhost;
    this->close = false;
    this->needForward = false;
    this->tooFar = false;
}

void Asservissement::reset() {
    pidTranslation->reset();
    pidRotation->reset();
}

void Asservissement::compute(float dt) {
    float xError = kineticGhost->getX() - kineticRobot->getX();
    float yError = kineticGhost->getY() - kineticRobot->getY();

    float distError = xError * cos(kineticRobot->getTheta()) + yError * sin(kineticRobot->getTheta());

    needForward = (distError > 0);

    *rotationOrder = pidRotation->compute(kineticRobot->getTheta(), kineticRobot->getRotationSpeed(), kineticGhost->getTheta(), kineticGhost->getRotationSpeed(), dt);

    *translationOrder = pidTranslation->compute(0, kineticRobot->getTranslationSpeed(), distError, kineticGhost->getTranslationSpeed(), dt);

    close = pidTranslation->getClose() && pidRotation->getClose();
    // tooFar = pidTranslation->getTooFar() || pidRotation->getTooFar() || (*kineticGhost - *kineticRobot).norm() > pidTranslation->getCurrentProfile().epsilon;
}
