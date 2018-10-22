#ifndef COMMUNICATIONS
#define COMMUNICATIONS
#include "../msgs/messageHandler.h"
#include "../msgs/message.h"
#include "../msgs/errorStrings.h"
#include "../state/StateService.h"
#include "Arduino.h"
#include "HardwareSerial.h"



class Communications
{
    const int recBufferSize = 256;
    HardwareSerial4 serialPort;
    const int interruptPin = 12;
    ErrorStrings errorStrings;
    StateService* stateService;

    public:
    int getMaxSerialMsgSize();
    void init(StateService &stateService);
    void isr();
    //void receiveMsg(); ISR FUNCTION NEEDS TO BE NOT A CLASS FUNCTION
    void sendError(String errorMsg);
    void sendMessage(message msg);
    void read(char recBuffer[], int &index);
    void sendSuccess(String uuid);
};

#endif