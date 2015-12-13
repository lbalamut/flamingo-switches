/*
Usage: ./send <systemCode> <unitCode> <command>
Command is 0 for OFF and 1 for ON
*/

#include "FlamingoSwitch.h"
#include "codes.h"

#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>



void send(int device, int command)
{
    Serial.print("Sending device: ");
    Serial.print(device);
    Serial.print(" command: ");
    Serial.println(command);

}

int main(int argc, char *argv[]) {

    /*
       output PIN is hardcoded for testing purposes
       see https://projects.drogon.net/raspberry-pi/wiringpi/pins/
       for pin mapping of the raspberry pi GPIO connector
       */
    int PIN = 0;
    int device = atoi(argv[1]);
    int command  = atoi(argv[2]);

    if (wiringPiSetup () == -1) return 1;

    printf("Sending device[%i] command[%i]\n", unitCode, command);

    FlamingoSwitch aSwitch = FlamingoSwitch();
    aSwitch.enableTransmit(PIN);

    for (unsigned int i = 0; i< CODES_PER_COMMAND; i++) {
        uint32_t sendBuffer = DEVICE_CODES[device][command][i];
        aSwitch.send(sendBuffer);
    }
    return 0;
}
