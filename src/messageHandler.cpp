#include "messageHandler.h"
#include "Arduino.h"

// NOT YET IMPLEMENTED

bool messageHandler(Message msg) {
    /*if (msg.getStringFromKey("type") == "PUTDATA") {
        Database::instance().writeToDataLog(msg.getJSONString);
        
        return true;
    } else if (msg.getStringFromKey("type") == "DUMPDATA") {
        String tmp = Database::instance().dumpDataLogs();
        Communications::instance().sendMessage(tmp);
        return true;
    } else if (msg.getStringFromKey("type") == "DELETEDATA") {
        Database::instance().clearDatalog();
        //String msg = "NewTimestamp";
        //Database::instance().writeToDataLog(msg);
        return true;
    }*/
    return true;
    
}