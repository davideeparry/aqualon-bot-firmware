
#include "Communications.h"
#include "CommunicationsISR.h"


void Communications::init(usb_serial_class &serial) {
    index = 0;
    serialPort = &serial;
    //serialPort.begin(9600);
    //pinMode(interruptPin, INPUT); 
    //attachInterrupt(digitalPinToInterrupt(interruptPin), communicationsISRWrapper, RISING); 
    // removed due to testing 
}

void Communications::isr() {
    read();
    //serialPort->print("hit");
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
    serialPort->print(/*str*/msg);
}

void Communications::read() {
    int numQueued = serialPort->available();
    
    if (numQueued == 0) {
        if (index == 0) {
            sendError(errorStrings.transmissionErrorNotAvaliable);
            return;
        }  
    }
        
    
    int i;
    for (i=index; i < (index+numQueued); i++) {
        recBuffer[i] = serialPort->read();
        if (recBuffer[i] == '\n') {
            break;
        } else if (i == recBufferSize) {
            break;
        }
    }

    if (recBuffer[i] == '\n') {
        // A nice pretty non-segmented message came in
        // pass to message handler
        if (true/*messageHandler(string str(recBuffer))*/) {
            // FOR TESTING
            //String temp(recBuffer);
            //Serial.println("GOT THE CHARs");
            //sendSuccess("Someuuid"/*pull out the uuid as string and put here*/);
            String tmp(recBuffer);
            tmp = tmp.substring(0,i);
            messageHandler(tmp);
            index = 0;
        } else {
            sendError(errorStrings.transmissionErrorUnknownMsg); // might want uuid here
            index = 0;
        }
    } else if (i == numQueued) {
        // do nothing because we are expecting more
        index = i;
    } else {
        // We hit our size limit, which is terrible will call a transmission error
        sendError(errorStrings.transmissionErrorUnknownMsg);
        index = 0;
    }
}

void Communications::sendSuccess(String uuid) {
    //message success;
    //success.addKeyValuePair("uuid",uuid);
    sendMessage(/*success*/uuid);
}
