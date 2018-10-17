#include "comms.h"

void commsInit() {
   commPort.begin(9600);
   pinMode(INTERRUPT_PIN_COMM, INPUT); 
   attachInterrupt(digitalPinToInterrupt(commIntPin), recTransmission, RISING); 
}


void recTransmission() {
    char recBuffer[recBufferSize];
    Serial.println("Start interrupt:");
    int i = 0;
    readSerial(recBuffer, 0, numQueued);
}

void transmissionError(string errorMsg) {
    sendTransmission(errorMsg);
    sendTransmission(errorMsg);
    sendTransmission(errorMsg);
    Serial.print(errorMsg);
}

void sendTransmission(string msg) {
    commPort.print(msg);
}

void readSerial(char recBuffer[recBufferSize], int &index) {
    int numQueued = commPort.available();
    
    if (numQueued == 0) {
        transmissionError(transmissionErrorNotAvaliable);
        break;
    }
    int i;
    for (i=index; i < numQueued; i++) {
        recBuffer[i] = commPort.read();
        if (recBuffer[i] == '\0') {
            break;
        } else if (i == recBufferSize) {
            break;
        }
    }

    if (recBuffer[i] == '\0') {
        // A nice pretty non-segmented message came in
        // pass to message handler
    } else if (i == numQueued) {
        // We've read in the whole buffer and still didn't get a cap to our message assume it is segmented
        readSerial(recBuffer, i);
    } else {
        // We hit our size limit, which is terrible will call a transmission error
        transmissionError(transmissionErrorOverflow);
    }
}

