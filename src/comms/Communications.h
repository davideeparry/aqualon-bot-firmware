#ifndef COMMUNICATIONS
#define COMMUNICATIONS 1
//#include "../msgs/messageHandler.h"
//#include "../msgs/message.h"
#include "../msgs/ErrorStrings.h"
#include "../state/StateService.h"
#include "Arduino.h"
#include "HardwareSerial.h"
#include "../msgs/messageHandler.h"




class Communications
{
    static const int recBufferSize = 256;
    char recBuffer[recBufferSize];
    int index;
    //HardwareSerial4 serialPort;
    usb_serial_class *serialPort;
    const int interruptPin = 12;
    ErrorStrings errorStrings;
    Communications() {

    };
    public:
    static Communications& instance() {
        static Communications INSTANCE;
        return INSTANCE;
    }
    int getMaxSerialMsgSize();
    void init(usb_serial_class &serial);
    void isr();
    //void receiveMsg(); ISR FUNCTION NEEDS TO BE NOT A CLASS FUNCTION
    void sendError(String errorMsg);
    void sendMessage(/*message*/String msg);
    void read();
    void sendSuccess(String uuid);
};

#endif