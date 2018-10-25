#include "Database.h"
#include "../comms/Communications.h"

void Database::init() {
    if (!SD.begin(chipSelect)) { // Need a callback
        Communications::instance().sendError("SD Initialization Failed");
        initFailed = true;
        return;
    }
};

void Database::writeToDataLog(String msg) {
    dataLog = SD.open(dataLogId, FILE_WRITE); // needs callback
    dataLog.println(msg);
    dataLog.close();
};

void Database::writeToSystemLog(String msg) {
    systemLog = SD.open(systemLogId, FILE_WRITE); // needs callback
    systemLog.println(msg);
    systemLog.close();
};

String Database::dumpDataLogs() { // WILL WANT TO MAKE THIS ONLY DUMP AS MUCH AS MEMORY WILL ALLOW, AND IT SHOULD BE CALLED MULTIPLE TIMES BY THE CALLER
    String dataDump = "";
    dataLog = SD.open(dataLogId); // callback
    while (dataLog.available()) {
        char tmp = dataLog.read();
        dataDump.concat(tmp);
    }
    dataLog.close();
    return dataDump;
};

String Database::dumpSystemLogs() { // WILL WANT TO MAKE THIS ONLY DUMP AS MUCH AS MEMORY WILL ALLOW, AND IT SHOULD BE CALLED MULTIPLE TIMES BY THE CALLER
    String systemDump;
    systemLog = SD.open(systemLogId, FILE_WRITE);
    while (systemLog.available()) {
        systemDump.append(systemLog.read());
    }
    dataLog.close();
    return systemDump;
};

void Database::clearSystemLog() {
    SD.remove(systemLogId);
};

void Database::clearDatalog() {
    SD.remove(dataLogId);
};

