#ifndef SEQUENCE_MANAGER_H_
#define SEQUENCE_MANAGER_H_

#include <vector>
#include "Sequence.h"
#include "Robot.h"

class SequenceManager {
    public:
        SequenceManager(std::vector<Sequence> sequences, unsigned int curSeqId = 0);
        SequenceManager() {};
        void update(float dt, Robot* robot);
        void forceRetourBase();
        void setEnemy(bool enemy, float enemyDst = -1.0f, float enemyAng = 0.0f);
        bool getEnemy() {return enemy;};
        void resume();

    private:

        std::vector<Sequence> sequences;    // the last sequence must be the retourBase one
        unsigned int curSeqId;

        bool enemy;
        float enemyDst;
        float enemyAng;

};

#endif
