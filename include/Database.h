#ifndef DATABASE
#define DATABASE 1
#include "SD.h"
#include "SPI.h"
#include "StateService.h"

#include "Arduino.h"

class Database
{
    bool initFailed = false;
    const int chipSelect = BUILTIN_SDCARD;
    char dataLogId[9] = "data.log";
    char systemLogId[11] = "system.log";
    File dataLog;
    File systemLog;
    Database(){

    };

  public:
    static Database &instance()
    {
        static Database INSTANCE;
        return INSTANCE;
    }
    void init();
    void writeToSystemLog(String messageBuffer);
    void writeToDataLog(String messageBuffer);
    String dumpSystemLogs();
    String dumpDataLogs();
    void clearSystemLog();
    void clearDatalog();
};

#endif