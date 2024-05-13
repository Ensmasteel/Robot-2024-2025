#ifndef ACTION_H_
#define ACTION_H_

#include "Arduino.h"
#include "Communication.h"
#include "Tirette.h"
#include "Logger.h"
#include "ActionManager.h"
#include "Vector.h"


class Action{

    public :

        Action(uint8_t id);
        Action(){};
        void start();
        void finish();

        uint8_t idAction;
};

class MoveAction : public Action
{

    public:

        MoveAction();
        void start();
        void update(float dt);

    private: 


};

#endif