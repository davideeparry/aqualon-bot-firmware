#ifndef DATABASE
#define DATABASE 1
#include "SD.h"
#include "SPI.h"
#include "../state/stateService.h"

#include "Arduino.h"

class Database {
    const int chipSelect = BUILTIN_SDCARD;
    char dataLogId[9] = "data.log";
    char systemLogId[11] = "system.log";
    File dataLog;
    File systemLog;
    Database() {

    };
    public:
    static Database& instance() {
        static Database INSTANCE;
        return INSTANCE;
    }
    void init();
    void writeToSystemLog(char* messageBuffer);
    void writeToDataLog(char* messageBuffer);
    String dumpSystemLogs();
    String dumpDataLogs();
    void clearSystemLog();
    void clearDatalog();
};

#endif