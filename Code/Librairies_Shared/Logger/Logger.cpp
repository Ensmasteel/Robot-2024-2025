#include "Logger.h"

//----------Logger Class----------//
           
Print* Logger::serialInfo;
Print* Logger::serialDebug;
Print* Logger::serialTeleplot;
  
bool Logger::infoOpened;
bool Logger::debugOpened;
bool Logger::teleplotOpened;

void Logger::setup(Print* infoSerial, Print* debugSerial, Print* teleplotSerial, bool info, bool debug, bool teleplot){
    serialInfo=infoSerial;
    serialDebug=debugSerial;
    serialTeleplot = teleplotSerial;
    infoOpened=info;
    debugOpened=debug;
    teleplotOpened=teleplot;
    if (info){
        serialInfo->println("Le canal Info est ouvert");
    }
    if (debug){
        serialDebug->println("Le canal Debug est ouvert");
    }
    if(teleplot){
        serialTeleplot->println("Le canal telplot est ouvert");
    }
}

void Logger::info(const String &message){
    if (infoOpened){
        serialInfo->println("[Info] " + message);
    }
}

void Logger::debug(const String &message){
    if (debugOpened){
        serialDebug->println("[Debug] " + message);
    }
}

void Logger::teleplot(const String &message){
    if (teleplotOpened){
        serialTeleplot->println(message);
    }
}

//----------End Logger Class----------//