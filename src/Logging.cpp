#include "Logging.h"

static char logTimeBuff[32];
static char logStrBuff[256];

static char logLevelStrings[NUM_LOG_LEVELS][8] = {
    " NONE  ", " ERROR ", " INFO  ", " DEBUG "
};

static logLevels logLevelSerial = NONE;
static logLevels logLevelDatabase = NONE;
static logLevels logLevelComms = NONE;

static void loadTimeStr() {
    tmElements_t tm;
    time_t timeNow = now();
    breakTime(timeNow, tm);
    sprintf(logTimeBuff, "%04u-%02u-%02u %02u:%02u:%02u", 
            tmYearToCalendar(tm.Year), tm.Month, tm.Day, tm.Hour, tm.Minute, tm.Second);
}

static void LogOut(logLevels level, const char *str) {
    if(level <= logLevelSerial) {
        Serial.print(logTimeBuff); Serial.print(logLevelStrings[level]); 
        Serial.print(logStrBuff); Serial.print("\n");
    }
    if(level <= logLevelDatabase) {
        Database::instance().writeToSystemLog(String(logTimeBuff) + 
                                    String(logLevelStrings[level]) + 
                                    String(str) + String("\n"));
    }
    // if(level <= logLevelComms) {
    // }
}

void Log(logLevels level, const char* fmt, ...) {
    if(!(level <= logLevelDatabase || level <= logLevelComms || level <= logLevelSerial)) return;
    loadTimeStr();
    va_list args;
    va_start(args, fmt);
    vsprintf(logStrBuff, fmt, args);
    va_end(args);
    LogOut(level, logStrBuff);
}

void Log(logLevels level, const char* str) {
    if(!(level <= logLevelDatabase || level <= logLevelComms || level <= logLevelSerial)) return;
    loadTimeStr();
    LogOut(level, str);
}

void setLogLevelSerial(logLevels level) {
    logLevelSerial = level;
}

void setLogLevelDatabase(logLevels level) {
    logLevelDatabase = level;
}

void setLogLevelComms(logLevels level) {
    logLevelComms = level;
}