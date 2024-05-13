#include "Sequence.h"

Sequence::Sequence(std::vector<Action*> actions, unsigned int curActionId):
    actions(actions),
    curActionId(curActionId)
{}

bool Sequence::checkClearPath(float angle, float distance){
    return actions[curActionId]->checkClearPath(angle, distance);
}

void Sequence::run(float dt, Robot* robot){
    if (curActionId < actions.size ()) { // there is still an action to do
        actions[curActionId]->run(dt, robot);

        if (actions[curActionId]->isDone()) {
            curActionId++;
        }
    }
}

bool Sequence::isDone(){
    return (curActionId >= actions.size());
}

void Sequence::reset(){
    curActionId = 0;
    for (auto& action : actions) action->reset();
}

void Sequence::resume() {
    if (curActionId < actions.size ()) { // there is still an action to do
        actions[curActionId]->reset();
    }
}
