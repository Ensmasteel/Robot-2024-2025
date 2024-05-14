#ifndef ACTION_H
#define ACTION_H

#include "Robot.h"
#include "Vector.h"

enum actionType {
    MOVE,
    DELAY,
    OPEN_CLAWS,
    CLOSE_CLAWS,
    RAISE_CLAWS,
    LOWER_CLAWS,
    START_MAGNET,
    SHUTDOWN_MAGNET,
    SOLAR_LEFT_ON,
    SOLAR_LEFT_OFF,
    SOLAR_RIGHT_ON,
    SOLAR_RIGHT_OFF
};

class Action {
    public:
        virtual bool checkClearPath(float distance, float angle) = 0;
        virtual void run(float dt, Robot* robot) = 0;
        virtual bool isDone() = 0;
        virtual void reset() = 0; 

    protected:
        actionType id;
        unsigned long msstart;
        unsigned long msduration;
};

class MoveAction : public Action {
    public:
        MoveAction(VectorOriented destination, bool isOnlyRotation, bool isBackward, bool nullInitSpeed = true, bool nullFinalSpeed = true, unsigned int mstimeout = 0);
        bool checkClearPath(float distance, float angle) override;
        void run(float dt, Robot* robot) override;
        bool isDone() override;
        void reset() override;

    private:
        VectorOriented destination;
        bool isOnlyRotation;
        bool isBackward;
        bool movementDone;
        bool nullInitSpeed;
        bool nullFinalSpeed;
        unsigned int mstimeout;
};

class StaticAction : public Action {
    public:
        StaticAction(actionType aid, bool noDuration = false);
        bool checkClearPath(float distance, float angle) override;
        void run(float dt, Robot* robot) override;
        bool isDone() override;
        void reset() override;

    private:
        bool noDuration;
};

class DelayAction : public Action {
    public:
        DelayAction(unsigned long msduration);
        bool checkClearPath(float distance, float angle) override;
        void run(float dt, Robot* robot) override;
        bool isDone() override;
        void reset() override;
};


#endif
