#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include <vector>
#include "Action.h"
#include "Robot.h"

class Sequence {
    public:
        Sequence(std::vector<Action*> actions, unsigned int curActionId = 0);
        bool checkClearPath(float distance, float angle);
        void run(float dt, Robot* robot);
        bool isDone();
        void reset();
        void resume();

    private:
        std::vector<Action*> actions;
        unsigned int curActionId;

};

#endif
