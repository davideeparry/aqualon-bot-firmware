#include "Database.h"
#include "../state/globalObjs.h"

void Database::init(StateService &state) {
    stateService = &state;
    if (!SD.begin(chipSelect)) {
        communications.sendError("SD Initialization Failed");
        return;
    }
    systemLog = SD.open(systemLogId, FILE_WRITE); // NEED MORE ERROR CHECKING HERE
    dataLog = SD.open(dataLogId, FILE_WRITE);
};

void Database::writeToDataLog(char* dataMsg) {
    dataLog = SD.open(dataLogId, FILE_WRITE);
    dataLog.println(dataMsg);
    dataLog.close();
}

void Database::writeToSystemLog(char* systemMsg) {
    systemLog = SD.open(systemLogId, FILE_WRITE);
    systemLog.println(systemMsg);
    systemLog.close();
}

String Database::dumpDataLogs() { // WILL WANT TO MAKE THIS ONLY DUMP AS MUCH AS MEMORY WILL ALLOW, AND IT SHOULD BE CALLED MULTIPLE TIMES BY THE CALLER
    String dataDump;
    dataLog = SD.open(dataLogId, FILE_WRITE);
    while (dataLog.available()) {
        dataDump.append(dataLog.read());
    }
    dataLog.close();
    return dataDump;
}

String Database::dumpSystemLogs() { // WILL WANT TO MAKE THIS ONLY DUMP AS MUCH AS MEMORY WILL ALLOW, AND IT SHOULD BE CALLED MULTIPLE TIMES BY THE CALLER
    String systemDump;
    systemLog = SD.open(systemLogId, FILE_WRITE);
    while (systemLog.available()) {
        systemDump.append(systemLog.read());
    }
    dataLog.close();
    return systemDump;
}

void Database::clearSystemLog() {
    SD.remove(systemLogId);
}

void Database::clearDatalog() {
    SD.remove(dataLogId);
}

