#ifndef COMMUNICATIONS
#define COMMUNICATIONS
#include "../msgs/messageHandler.h"
#include "../msgs/message.h"
#include "../msgs/errorStrings.h"
#include "../state/StateService.h"

class Communications
{
    const int recBufferSize = 256;
    Serial4 serialPort;
    const int interruptPin = 12;
    ErrorStrings errorStrings;
    StateService* stateService;

    public:
    int getMaxSerialMsgSize();
    void init(StateService &stateService);
    void receiveMsg();
    void sendError(string errorMsg);
    void sendMessage(message msg);
    void read(char recBuffer[], int &index);
    void sendSuccess(string uuid);
};

#endif