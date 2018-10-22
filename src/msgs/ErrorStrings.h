#ifndef JSON_MESSAGES
#define JSON_MESSAGES
#include <string>

class ErrorStrings {
    public:
    const String transmissionErrorNotAvaliable = "No bytes avaliable to read following rec interupt";
    const String transmissionErrorOverflow = "Overflow";
    const String transmissionErrorUnknownMsg = "No message handler for msg";
};


#endif