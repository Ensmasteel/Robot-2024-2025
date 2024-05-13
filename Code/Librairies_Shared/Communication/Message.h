#ifndef Message_H_
#define Message_H_

#include "Arduino.h"
#include <Stream.h>
#include "IDS.h"

#define MESSAGE_BOX_SIZE 10

typedef struct __attribute__((packed)){
    uint8_t start = 255;
    EquipmentID s = EmptyE;
    EquipmentID d = EmptyE;
    DataID did = EmptyD;
    uint16_t x = 0;
    uint16_t y = 0;
    ActionID aid = Pause;
    uint16_t distance = 0;
    int16_t angle = 0;
} Message;

Message newMessageLidar(EquipmentID s, EquipmentID d, uint16_t distance,uint16_t angle);
Message newMessageCoordonnees(EquipmentID s, EquipmentID d, uint16_t x, uint16_t y);
Message newMessageToDo(EquipmentID s, EquipmentID d, ActionID aid);
Message newMessageEndAction(EquipmentID s, EquipmentID d, ActionID aid);
Message newMessageActuator(EquipmentID s, EquipmentID d, ActionID aid);

class MessageBox{
    public :
        MessageBox();
        Message pull();
        void push(Message Message2);
        int size();
        Message peek();
        bool getEmpty();

    private :
        uint8_t ifirst = 0;
        Message box[MESSAGE_BOX_SIZE];
        uint8_t iend = 0;
        bool empty=true;
};


#endif