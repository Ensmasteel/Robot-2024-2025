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
            case OPEN_FRONT_RATO:
                msduration = 300;
                break;
            case CLOSE_FRONT_RATO:
                msduration = 300;
                break;
            case OPEN_BACK_RATO:
                msduration = 300;
                break;
            case CLOSE_BACK_RATO:
                msduration = 300;
                break;
            case RAISE_FRONT_MAGNETS:
                msduration = 800;
                break;
            case LOWER_FRONT_MAGNETS:
                msduration = 800;
                break;
            case RAISE_BACK_MAGNETS:
                msduration = 800;
                break;
            case LOWER_BACK_MAGNETS:
                msduration = 800;
                break;
            case START_FRONT_LEFT_MAGNETS:
                msduration = 0;
                break;
            case STOP_FRONT_LEFT_MAGNETS:
                msduration = 0;
                break;
            case START_FRONT_RIGHT_MAGNETS:
                msduration = 0;
                break;
            case STOP_FRONT_RIGHT_MAGNETS:
                msduration = 0;
                break;
            case START_BACK_LEFT_MAGNETS:
                msduration = 0;
                break;
            case STOP_BACK_LEFT_MAGNETS:
                msduration = 0;
                break;
            case START_BACK_RIGHT_MAGNETS:
                msduration = 0;
                break;
            case STOP_BACK_RIGHT_MAGNETS:
                msduration = 0;
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
            case OPEN_FRONT_RATO:
                robot->openFrontRato();
                break;
            case CLOSE_FRONT_RATO:
                robot->closeFrontRato();
                break;
            case OPEN_BACK_RATO:
                robot->openBackRato();
                break;
            case CLOSE_BACK_RATO:
                robot->closeBackRato();
                break;
            case RAISE_FRONT_MAGNETS:
                robot->raiseFrontMagnets();
                break;
            case LOWER_FRONT_MAGNETS:
                robot->lowerFrontMagnets();
                break;
            case RAISE_BACK_MAGNETS:
                robot->raiseBackMagnets();
                break;
            case LOWER_BACK_MAGNETS:
                robot->lowerBackMagnets();
                break;
            case START_FRONT_LEFT_MAGNETS:
                robot->startFrontLeftMagnets();
                break;
            case STOP_FRONT_LEFT_MAGNETS:
                robot->stopFrontLeftMagnets();
                break;
            case START_FRONT_RIGHT_MAGNETS:
                robot->startFrontRightMagnets();
                break;
            case STOP_FRONT_RIGHT_MAGNETS:
                robot->stopFrontRightMagnets();
                break;
            case START_BACK_LEFT_MAGNETS:
                robot->startBackLeftMagnets();
                break;
            case STOP_BACK_LEFT_MAGNETS:
                robot->stopBackLeftMagnets();
                break;
            case START_BACK_RIGHT_MAGNETS:
                robot->startBackRightMagnets();
                break;
            case STOP_BACK_RIGHT_MAGNETS:
                robot->stopBackRightMagnets();
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
