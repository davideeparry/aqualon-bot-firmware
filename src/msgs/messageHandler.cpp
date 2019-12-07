#include "messageHandler.h"
#include "Arduino.h"

// NOT YET IMPLEMENTED

bool messageHandler(Message msg) {
    if (msg.getStringFromKey("type") == "PUTDATA") {
        Database::instance().writeToDataLog(msg.getJSONString());
        return true;
    } else if (msg.getStringFromKey("type") == "get" && msg.getStringFromKey("data") == "data") {
        String dumped = Database::instance().dumpDataLogs(100);
        if (dumped.length() == 0) {
            dumped = "NO DATA";
        }
        int seqId = 0;
        Message tmpMessage("");
        tmpMessage.addKeyValuePair("type", "sysLogChunk");
        tmpMessage.addKeyValuePair("seqId", String(seqId));
        tmpMessage.addKeyValuePair("length", String(dumped.length()));
        tmpMessage.addKeyValuePair("uuid", msg.getStringFromKey("uuid"));
        tmpMessage.addKeyValuePair("msg", dumped);
        Communications::instance().sendMessage(tmpMessage);
       while (dumped.length() >= 100) {
            seqId = seqId + 1;
            dumped = Database::instance().dumpDataLogs(100);
            if (dumped.length() != 0) {
                Message tmpMessage("");
                tmpMessage.addKeyValuePair("type", "sysLogChunk");
                tmpMessage.addKeyValuePair("seqId", String(seqId));
                tmpMessage.addKeyValuePair("uuid", msg.getStringFromKey("uuid"));
                tmpMessage.addKeyValuePair("msg", dumped);
                Communications::instance().sendMessage(tmpMessage);
            }   
        }
        return true;
    } else if (msg.getStringFromKey("type") == "DELETEDATA") {
        Database::instance().clearDatalog();
        //String msg = "NewTimestamp";
        //Database::instance().writeToDataLog(msg);
        return true;
    }
   return false;
    
}