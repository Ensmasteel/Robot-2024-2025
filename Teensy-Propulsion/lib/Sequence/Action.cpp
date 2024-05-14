#include "Action.h"

MoveAction::MoveAction(VectorOriented destination, bool isOnlyRotation, bool isBackward, bool nullInitSpeed, bool nullFinalSpeed, unsigned int mstimeout):
    destination(destination),
    isOnlyRotation(isOnlyRotation),
    isBackward(isBackward),
    nullInitSpeed(nullInitSpeed),
    nullFinalSpeed(nullFinalSpeed)
{
    id = MOVE;
    msduration = mstimeout;
    msstart = 0;
    movementDone = false;
}

bool MoveAction::checkClearPath(float distance, float angle) {
    if (isOnlyRotation) return true;    // we don't care of enemies as we are not moving out of our position
    if (!isBackward && angle < 0.0f && angle > -PI / 2.0f) return false;    // front
    if (isBackward && angle > PI / 2.0f) return false;  // back
    return true;
    // TODO
}

void MoveAction::run(float dt, Robot* robot) {
    if (!msstart) {
        msstart = millis();
        robot->startMovement(destination, isOnlyRotation, isBackward, nullInitSpeed, nullFinalSpeed);
    }
    robot->updateMovement();
    if (robot->movementDone() || (msduration && (millis() - msstart) > msduration)) {
        movementDone = true;
    }
}

bool MoveAction::isDone() {
    return movementDone;
}

void MoveAction::reset() {
    msstart = 0;
    movementDone = false;
}


StaticAction::StaticAction(actionType aid, bool noDuration):
    noDuration(noDuration)
{
    id = aid;
    msstart = 0;
    if (!noDuration) {
        switch (id) {
            case OPEN_CLAWS:
                msduration = 300;
                break;
            case CLOSE_CLAWS:
                msduration = 300;
                break;
            case RAISE_CLAWS:
                msduration = 800;
                break;
            case LOWER_CLAWS:
                msduration = 800;
                break;
            case START_MAGNET:
                msduration = 0;
                break;

            case SHUTDOWN_MAGNET:
                msduration = 0;
                break;

            case SOLAR_LEFT_ON:
                msduration = 300;
                break;

            case SOLAR_LEFT_OFF:
                msduration = 300;
                break;

            case SOLAR_RIGHT_ON:
                msduration = 300;
                break;

            case SOLAR_RIGHT_OFF:
                msduration = 300;
                break;

            default:
                msduration = 0;
                break;
        }
    } else {
        msduration = 0;
    }
}

bool StaticAction::checkClearPath(float distance, float angle) {
    return true;    // as it is a static action, we do not care of the enemies
}

void StaticAction::run(float dt, Robot* robot) {
    if (!msstart) {
        msstart = millis();
        switch (id) {
            case OPEN_CLAWS:
                robot->openClaws();
                break;
            case CLOSE_CLAWS:
                robot->closeClaws();
                break;
            case RAISE_CLAWS:
                robot->raiseClaws();
                break;
            case LOWER_CLAWS:
                robot->lowerClaws();
                break;
            case START_MAGNET:
                robot->startMagnet();
                break;

            case SHUTDOWN_MAGNET:
                robot->shutdownMagnet();
                break;

            case SOLAR_LEFT_ON:
                robot->solarLeftOn();
                break;

            case SOLAR_LEFT_OFF:
                robot->solarLeftOff();
                break;

            case SOLAR_RIGHT_ON:
                robot->solarRightOn();
                break;

            case SOLAR_RIGHT_OFF:
                robot->solarRightOff();
                break;

            default:
                break;
        }
    }
}

bool StaticAction::isDone() {
    return ((millis() - msstart) >= msduration);
}

void StaticAction::reset() {
    msstart = 0;
}


DelayAction::DelayAction(unsigned long msduration){
    msstart = 0;
    this->msduration = msduration;
    id = DELAY;
}

bool DelayAction::checkClearPath(float distance, float angle) {
    return true;
}

void DelayAction::run(float dt, Robot* robot) {
    if (!msstart) msstart = millis(); 
}

bool DelayAction::isDone() {
    return ((millis() - msstart) >= msduration);
}

void DelayAction::reset() {
   msstart = 0;
}
