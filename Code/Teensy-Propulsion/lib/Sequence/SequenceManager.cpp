#include "SequenceManager.h"

SequenceManager::SequenceManager(std::vector<Sequence> sequences, unsigned int curSeqId):
    sequences(sequences),
    curSeqId(curSeqId)
{}

void SequenceManager::setEnemy(bool enemy, float enemyDst, float enemyAng){
    this->enemy = enemy;
    this->enemyDst = enemyDst;
    this->enemyAng = enemyAng;
}

void SequenceManager::forceRetourBase() {
    curSeqId = sequences.size() - 1;
}

void SequenceManager::update(float dt, Robot* robot){
    if (curSeqId < sequences.size ()) { // there is still a sequence to do
        if (enemy) {
            if (sequences[curSeqId].checkClearPath(enemyDst, enemyAng)){
                // the enemy is not on our path

                enemy = false;
                robot->resumeMotor();
                this->resume();
                robot->getGhost().setLock(false);

                sequences[curSeqId].run(dt, robot);
            } else {
                // we cannot pursue our sequence as there is someone on the way

                // TODO: what can we do?

                robot->stopMovement();
                robot->getGhost().setLock(true);
            }
        } else {
            sequences[curSeqId].run(dt, robot);
        }

        if (sequences[curSeqId].isDone()) {
            sequences[curSeqId].reset();    // reset the sequences to be able to reuse it later
            curSeqId++;
        }
    }
}

void SequenceManager::resume() {
    if (curSeqId < sequences.size ()) { // there is still a sequence to do
        sequences[curSeqId].resume();
    }
}
