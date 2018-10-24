#ifndef DATABASE
#define DATABASE 1
#include "SD.h"
#include "SPI.h"
#include "../state/stateService.h"

#include "Arduino.h"

class Database {
    const int chipSelect = BUILTIN_SDCARD;
    StateService* stateService;
    char dataLogId[9] = "data.log";
    char systemLogId[11] = "system.log";
    File dataLog;
    File systemLog;

    public:
    void init(StateService &state);
    void writeToSystemLog(char* messageBuffer);
    void writeToDataLog(char* messageBuffer);
    String dumpSystemLogs();
    String dumpDataLogs();
    void clearSystemLog();
    void clearDatalog();
};

#include "../state/globalObjs.h"
#endif