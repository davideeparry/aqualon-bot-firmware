#include "messageHandler.h"
#include "Arduino.h"

// NOT YET IMPLEMENTED

bool messageHandler(String msg) {
    if (msg == "PUTDATA") {
        Database::instance().writeToDataLog(msg);
        
        return true;
    } else if (msg == "DUMPDATA") {
        String tmp = Database::instance().dumpDataLogs();
        Communications::instance().sendMessage(tmp);
        return true;
    } else if (msg == "DELETEDATA") {
        Database::instance().clearDatalog();
        //String msg = "NewTimestamp";
        //Database::instance().writeToDataLog(msg);
        return true;
    }
    return true;
    
}