#ifndef ACTION_H
#define ACTION_H

#include "Robot.h"
#include "Vector.h"

enum actionType {
    MOVE,
    DELAY,
    OPEN_FRONT_RATO,
    CLOSE_FRONT_RATO,
    OPEN_BACK_RATO,
    CLOSE_BACK_RATO,
    START_FRONT_LEFT_MAGNETS,
    STOP_FRONT_LEFT_MAGNETS,
    START_FRONT_RIGHT_MAGNETS,
    STOP_FRONT_RIGHT_MAGNETS,
    START_BACK_LEFT_MAGNETS,
    STOP_BACK_LEFT_MAGNETS,
    START_BACK_RIGHT_MAGNETS,
    STOP_BACK_RIGHT_MAGNETS,
    RAISE_FRONT_MAGNETS,
    LOWER_FRONT_MAGNETS,
    RAISE_BACK_MAGNETS,
    LOWER_BACK_MAGNETS,
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
