#include "Odometry.h"
#include "Arduino.h"
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
    this->switchRight=switchR;
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

    const double dforward_codeuseL = codeuseLeft->getDeltaAvance ();
    const double dforward_codeuseR = codeuseRight->getDeltaAvance ();

    const double theta = kinetics->getTheta ();

    const double dforward = (dforward_codeuseL + dforward_codeuseR) / 2.0;  // pure translation movement

    if (std::fabs(dforward_codeuseL - dforward_codeuseR) < std::numeric_limits<double>::epsilon ()) {  // Are they equal? TODO: dirty
        // the robot strictly goes forward

        *kinetics += Kinetic (
            dforward * std::cos (theta),
            dforward * std::sin (theta),
            0.0,
            dforward / dt,
            0.0
        );
    } else {
        // The robot did a rotation

        /*
        Hypothesis:
            - the robot moved along a circular trajectory.
            - it was perpendicular to its trajectory at the beggining of its movemement.

        Note that these are the exact values, there is no approximations unless these hypothesis.
        */

        const double dtheta = (dforward_codeuseR - dforward_codeuseL) / codeusesSpacing;   // new_theta = prev_theta + dtheta

        const double rotation_circle_radius = (codeusesSpacing / 2.0) * (dforward_codeuseR + dforward_codeuseL) / (dforward_codeuseR - dforward_codeuseL);    // the radius of the imaginary circle

        // Compute the movement relatively to the robot orientation
        const double local_deltaX = std::fabs (rotation_circle_radius * (1 - std::cos (dtheta)));
        const double local_deltaY = std::fabs (rotation_circle_radius * std::sin (dtheta));

        const float v = (codeuseLeft->getTranslationSpeedCodeuse() + codeuseRight->getTranslationSpeedCodeuse()) / 2;
        const float w = (codeuseRight->getTranslationSpeedCodeuse() - codeuseLeft->getTranslationSpeedCodeuse()) / this->codeusesSpacing;

        if (dforward > 0.0) {
            if (dtheta > 0.0) {
                *kinetics += Kinetic (
                    -local_deltaX * std::sin (theta) + local_deltaY * std::cos (theta),   // conversion from local to global
                    local_deltaX * std::cos (theta) + local_deltaY * std::sin (theta),    // conversion from local to global
                    dtheta,
                    v,
                    w
                );
            } else {
                *kinetics += Kinetic (
                    local_deltaX * std::sin (theta) + local_deltaY * std::cos (theta),   // conversion from local to global
                    -local_deltaX * std::cos (theta) + local_deltaY * std::sin (theta),    // conversion from local to global
                    dtheta,
                    v,
                    w
                );
            }
        } else {
            if (dtheta > 0.0) {
                *kinetics += Kinetic (
                    local_deltaX * std::sin (theta) - local_deltaY * std::cos (theta),   // conversion from local to global
                    -local_deltaX * std::cos (theta) - local_deltaY * std::sin (theta),    // conversion from local to global
                    dtheta,
                    v,
                    w
                );
            } else {
                *kinetics += Kinetic (
                    -local_deltaX * std::sin (theta) - local_deltaY * std::cos (theta),   // conversion from local to global
                    local_deltaX * std::cos (theta) - local_deltaY * std::sin (theta),    // conversion from local to global
                    dtheta,
                    v,
                    w
                );
            }
        }

        kinetics->setTranslationSpeed(v);
        kinetics->setRotationSpeed(w);

        kinetics->normalizeTheta ();
    }

    //kinetics->printTeleplot("ROBOT ");
}
