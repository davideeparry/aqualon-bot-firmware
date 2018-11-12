#ifndef MESSAGES
#define MESSAGES 1
#include "Arduino.h"
#include "ArduinoJson.h"

class Message
{
    String stringMsg;
    StaticJsonBuffer<200> jsonBuffer;
    public:
        // Also if we want we can easily use ENUM types for the key instead of a String
    Message();
    Message(String msg); // generating a msg from a fresh buffer
    Message(const Message &msg);
    void makeError(String error);
    bool addKeyValuePair(String key, String pair); // Can overload this function for various types
    int getIntFromKey(String key); 
    double getDoubleFromKey(String key);
    String getStringFromKey(String key);
    String getJSONString(); // Turn back into a byte buffer for transmission     
};

#endif