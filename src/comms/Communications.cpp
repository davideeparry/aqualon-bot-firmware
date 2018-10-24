
#include "Communications.h"
#include "CommunicationsISR.h"


void Communications::init() {
    serialPort.begin(9600);
    pinMode(interruptPin, INPUT); 
    attachInterrupt(digitalPinToInterrupt(interruptPin), communicationsISRWrapper, RISING); 
}

void Communications::isr() {
    char recBuffer[recBufferSize];
    int i = 0;
    read(recBuffer, i);
}


void Communications::sendError(String errorMsg) {
    //message error;
    //error.makeError(errorMsg);
    String error = errorMsg; // tmp becauase no message lib
    sendMessage(error);
    sendMessage(error);
    sendMessage(error);
}

void Communications::sendMessage(/*message*/String msg) { 
    //String str(msg.getSerialBuffer()); needs msg finished
    serialPort.print(/*str*/msg);
}

void Communications::read(char recBuffer[], int &index) {
    int numQueued = serialPort.available();
    
    if (numQueued == 0) {
        sendError(errorStrings.transmissionErrorNotAvaliable);
        return;
    }
    int i;
    for (i=index; i < numQueued; i++) {
        recBuffer[i] = serialPort.read();
        if (recBuffer[i] == '\0') {
            break;
        } else if (i == recBufferSize) {
            break;
        }
    }

    if (recBuffer[i] == '\0') {
        // A nice pretty non-segmented message came in
        // pass to message handler
        if (true/*messageHandler(string str(recBuffer))*/) {
            sendSuccess("Someuuid"/*pull out the uuid as string and put here*/);
        } else {
            sendError(errorStrings.transmissionErrorUnknownMsg); // might want uuid here
        }
    } else if (i == numQueued) {
        // We've read in the whole buffer and still didn't get a cap to our message assume it is segmented
        // WANT A CALLBACK HERE SO WE WAIT A LITTLE BIT
        read(recBuffer, i);
    } else {
        // We hit our size limit, which is terrible will call a transmission error
        sendError(errorStrings.transmissionErrorUnknownMsg);
    }
}

void Communications::sendSuccess(String uuid) {
    //message success;
    //success.addKeyValuePair("uuid",uuid);
    sendMessage(/*success*/uuid);
}
