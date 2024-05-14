#ifndef IDS_H_
#define IDS_H_

#include <stdint.h>
#include "Arduino.h"

enum EquipmentID : uint8_t
{
    EmptyE,
    Teensy,
    Arduino,
    ESP_32,
    Raspberry
};

enum ActionID : uint8_t
{
    Pause,
    OpenClaws,
    CloseClaws,
    RaiseClaws,
    LowerClaws,
    StartMagnet,
    ShutdownMagnet,
    SolarLeftOn,
    SolarLeftOff,
    SolarRightOn,
    SolarRightOff,
    SetTeamColorJaune,
    SetTeamColorBleu
};

enum DataID : uint8_t
{
    EmptyD,
    Coordonnees,    //Endroit
    Todo,           //Quelle action ?
    MessLidar,      //Message du Lidar
    MessActuator ,  //Message 
    EndAction
};

#endif
