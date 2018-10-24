#ifndef MESSAGES
#define MESSAGES 1
#include <string>

class message
{
    public:
        // Also if we want we can easily use ENUM types for the key instead of a String
        message(void); // empty msg
        message(char msgBuffer[]); // generating a msg from a fresh buffer
        void makeError(String error);
        bool addKeyValuePair(String key, String pair); // Can overload this function for various types
        int getIntFromKey(String key); 
        double getDoubleFromKey(String key);
        String getStringFromKey(String key);
        message getMsgFromKey(String key); // So we can store recursive structures
        char* getSerialBuffer(); // Turn back into a byte buffer for transmission     
};

#endif