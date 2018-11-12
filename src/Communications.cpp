
#include "Communications.h"
#include "CommunicationsISR.h"

void Communications::init(usb_serial_class &serial)
{
    index = 0;
    serialPort = &serial;
}

void Communications::isr()
{
    read();
}

void Communications::sendError(String errorMsg)
{
    Message newMsg;
    newMsg.addKeyValuePair("type", "error");
    newMsg.addKeyValuePair("msg", errorMsg);
    sendMessage(newMsg); 
}

void Communications::sendMessage(Message msg)
{
    serialPort->println(msg.getJSONString());
}

void Communications::read()
{
    int numQueued = serialPort->available();

    if (numQueued == 0)
    {
        if (index == 0)
        {
            sendError(errorStrings.transmissionErrorNotAvaliable);
            return;
        }
    }

    int i;
    for (i = index; i < (index + numQueued); i++)
    {
        recBuffer[i] = serialPort->read();
        if (recBuffer[i] == '\n')
        {
            break;
        }
        else if (i == recBufferSize)
        {
            break;
        }
    }

    if (recBuffer[i] == '\n')
    {
        String tmp(recBuffer);
        tmp = tmp.substring(0, i);
        Message newMessage(tmp);
        if (messageHandler(newMessage))
        {
            sendSuccess(newMessage.getStringFromKey("uuid"));    
        }
        else
        {
            sendError(errorStrings.transmissionErrorUnknownMsg);
        }
        index = 0;
    }
    else if (i == numQueued)
    {
        // do nothing because we are expecting more as we never got our delineator
        index = i;
    }
    else
    {
        // We hit our size limit, which is terrible will call a transmission error
        sendError(errorStrings.transmissionErrorUnknownMsg);
        index = 0;
    }
}

void Communications::sendSuccess(String uuid)
{
    Message newMsg;
    newMsg.addKeyValuePair("type", "success");
    newMsg.addKeyValuePair("uuid", uuid);
    sendMessage(newMsg);
}
