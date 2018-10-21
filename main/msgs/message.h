#ifndef MESSAGES
#define MESSAGES

class message
{
    public:
        // Also if we want we can easily use ENUM types for the key instead of a string
        message(void); // empty msg
        message(char msgBuffer[]); // generating a msg from a fresh buffer
        void makeError(string error);
        bool addKeyValuePair(string key, string pair); // Can overload this function for various types
        int getIntFromKey(string key); 
        double getDoubleFromKey(string key);
        string getStringFromKey(string key);
        message getMsgFromKey(string key); // So we can store recursive structures
        char* getSerialBuffer(); // Turn back into a byte buffer for transmission
        
};

#endif