
#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include <Arduino.h>
#include <Print.h>



/**
 * This class gives the ability to send telemtry data to the ESP32 that will later sends it via wifi
 * This class can also be setup to only stream certain points of data.
*/
class Telemetry{



    public :


/**
 * Constructor of the Telemetry class
 * @param telemetryPort the serial port that data is streamed to
 * @param enable a boolean that enables or disables telemetry streaming
*/
    Telemetry(Print* telemetryPort, bool enable = true);


    void disable();
    void enable();


    /**
     * @returns a bool of the state (enabled or disabled)
    */
    bool getState();


    /**
     * Send telemetry to the appropriate Serial port 
    */
    void update();

    private :

    bool enabled = true;

};




#endif