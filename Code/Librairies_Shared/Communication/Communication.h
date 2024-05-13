#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#define ANTISPAM_MS 300

#include "Message.h"
#include "Arduino.h"
#include <Stream.h>
#include "IDS.h"

class Communication{

    public : 

    Communication(Stream* port);
    Communication() {};
    void send(Message mess);
    void update();
    void popOldestMessage();
    Message peekOldestMessage();
    bool waitingRX();
    bool waitingTX();

    private :

    MessageBox* sendingBox;
    MessageBox* receivingBox;
    Stream* port;
    uint32_t millisLastSend;


};

#endif