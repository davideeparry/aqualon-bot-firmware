#ifndef HELPER_FUNCTIONS
#define HELPER_FUNCTIONS

class helperFunctions
{
    public:
    JsonObject getKeyValuePairs(char recBuffer[recBufferSize]);
    bool validateMsg(char recBuffer[recBufferSize]);
    bool storeUUID(string uuid);
    bool checkUUID(string uuid);
};

#endif