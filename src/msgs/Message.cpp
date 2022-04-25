#include "Message.h"

Message::Message() {
    stringMsg = "{}";
};
Message::Message(String msg) {
    stringMsg = msg;
    if (stringMsg == "") {
        stringMsg = "{}";
    }
}; // generating a msg from a fresh buffer
Message::Message(const Message &oldMsg) {
    stringMsg = oldMsg.stringMsg;
};
void Message::makeError(String error) {
    JsonObject& root = jsonBuffer.createObject();
    root["type"] = "error";
    root["msg"] = error;
    stringMsg = "";
    root.printTo(stringMsg);
};
bool Message::addKeyValuePair(String key, String pair) {
    JsonObject& root = jsonBuffer.parseObject(stringMsg);
    root[key] = pair;
    stringMsg = "";
    root.printTo(stringMsg);
    return true;
}; // Can overload this function for various types
int Message::getIntFromKey(String key) {
    JsonObject& root = jsonBuffer.parseObject(stringMsg);
    return root[key].as<int>();
}; 
double Message::getDoubleFromKey(String key)  {
    JsonObject& root = jsonBuffer.parseObject(stringMsg);
    return root[key].as<double>();
};
String Message::getStringFromKey(String key)  {
    JsonObject& root = jsonBuffer.parseObject(stringMsg);
    return root[key].as<String>();
};
String Message::getJSONString() {
    return stringMsg;
}; // Turn back into a byte buffer for transmission  