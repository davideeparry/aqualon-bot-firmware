#ifndef COMMUNICATIONS
#define COMMUNICATIONS 1
#include "ErrorStrings.h"
#include "StateService.h"
#include "Arduino.h"
#include "HardwareSerial.h"
#include "MessageHandler.h"
#include "Message.h"

class Communications
{
    static const int recBufferSize = 256;
    char recBuffer[recBufferSize];
    int index;
    //HardwareSerial4 serialPort; // This is for when the radio is here
    usb_serial_class *serialPort;
    const int interruptPin = 12;
    ErrorStrings errorStrings;
    Communications(){};

  public:
    static Communications &instance()
    {
        static Communications INSTANCE;
        return INSTANCE;
    }
    int getMaxSerialMsgSize();
    void init(usb_serial_class &serial);
    void isr();
    void sendError(String errorMsg);
    void sendMessage(Message msg);
    void read();
    void sendSuccess(String uuid);
};

#endif