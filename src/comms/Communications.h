#ifndef COMMUNICATIONS
#define COMMUNICATIONS 1
#include "../msgs/ErrorStrings.h"
#include "../state/StateService.h"
#include "Arduino.h"
#include "HardwareSerial.h"
#include "../msgs/messageHandler.h"
#include "../msgs/Message.h"

class Communications
{
    static const int recBufferSize = 256;
    char recBuffer[recBufferSize];
    int index;
    //HardwareSerial4 serialPort; // This is for when the radio is here
    HardwareSerial2 *serialPort;
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
    void init(HardwareSerial2 &serial);
    void isr();
    void sendError(String errorMsg);
    void sendMessage(Message msg);
    void read();
    void sendSuccess(String uuid);
};

#endif