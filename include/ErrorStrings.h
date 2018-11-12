#ifndef JSON_MESSAGES
#define JSON_MESSAGES 1
#include "Arduino.h"

class ErrorStrings {
    public:
    String transmissionErrorNotAvaliable = "No bytes avaliable to read following rec interupt";
    String transmissionErrorOverflow = "Overflow";
    String transmissionErrorUnknownMsg = "No message handler for msg";
};


#endif