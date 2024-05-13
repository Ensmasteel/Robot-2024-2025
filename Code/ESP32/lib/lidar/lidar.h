#ifndef LIDAR_H
#define LIDAR_H

#include <stdint.h>
#include <vector>
#include <Arduino.h>
#include "Logger.h"

#define FRAMEBOX_LEN 10
#define FRAME_NPOINT 12

typedef struct __attribute__((packed)){
    typedef struct __attribute__((packed)){
        uint16_t distance;
        uint8_t confidence;
    } point;
    uint8_t header;         // should be 0x54
    uint8_t ver_len;        // should be 0x2C
    uint16_t speed;         // deg per sec
    uint16_t start_angle;   // starting angle of the data packet (unit is 0.01 deg)
    point data[FRAME_NPOINT];
    uint16_t end_angle;     // ending angle of the data packet (unit is 0.01 deg)
    uint16_t timestamp;     // ms, max 30000, when it reaches 30000 it will be counted again, indicating the timestamp value of the data packet
    uint8_t crc8;           // check
} lidarFrame;

class FrameBox{
    public :
        FrameBox();
        lidarFrame pull();
        void push(lidarFrame fr);
        int size();
        lidarFrame& peek();
        bool getEmpty();

    private :
        uint8_t ifirst = 0;
        lidarFrame box[FRAMEBOX_LEN];
        uint8_t iend = 0;
        bool empty=true;
};

class Lidar {
    public:
        Lidar(Stream* port);
        void update();
        lidarFrame& peekOldestFrame();
        void popOldestFrame();
        bool waitingRX();

    private:
        Stream* port;
        FrameBox recevBox;
};

#endif  // LIDAR_H
