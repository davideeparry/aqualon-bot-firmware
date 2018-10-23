#ifndef DATABASE
#define DATABASE
#include <SD.h>
#include <SPI.h>
#include <String>
#include "../state/stateService.h"

class Database {
    const int chipSelect = BUILTIN_SDCARD;
    StateService* stateService;
    char* dataLogId = "data.log";
    char* systemLogId = "system.log";
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

#endif