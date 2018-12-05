/*
 * Some simple macros for logging out of the serial port in simulation. 
 */

#ifndef LOGGING_H
#define LOGGING_H

#include "Database.h"
#include <Arduino.h>
#include <TimeLib.h>

enum logLevels {
    NONE = 0,
    ERROR,
    INFO,
    DEBUG,
    NUM_LOG_LEVELS
};

void setLogLevelSerial(logLevels level);
void setLogLevelDatabase(logLevels level);
void setLogLevelComms(logLevels level);

void Log(logLevels level, const char* fmt, ...);

#define LogError(...) { Log(ERROR, __VA_ARGS__); }
#define LogInfo(...) Log(INFO, __VA_ARGS__)
#define LogDebug(...) { Log(DEBUG, __VA_ARGS__); }

// #define LogError(fmt) { Log(ERROR, fmt); }
// #define LogInfo(fmt) { Log(INFO, fmt); }
// #define LogDebug(fmt) { Log(DEBUG, fmt); }

#if 0 //defined(SIMULATION) || defined(LABBUILD)

#define LOG(s)      { Serial.print(millis()); Serial.print(": "); Serial.print(s); Serial.print("\n"); }
#define LOGV(s, v)  { Serial.print(millis()); Serial.print(": "); Serial.print(s); Serial.print(v); Serial.print("\n"); }
#define LOGX(s, v)  { Serial.print(millis()); Serial.print(": "); Serial.print(s); Serial.print(v, HEX); Serial.print("\n"); }
// #define LOGF(...)   { sprintf(debugPrintBuff, __VA_ARGS__); Serial.print(debugPrintBuff); Serial.print("\n"); }
#else
#define LOG(s)      { Database::instance().writeToDataLog(s); }
#define LOGV(s, v)  { Database::instance().writeToDataLog(String(s + String(": ") + String(v))); }
#define LOGX(s, v)  LOGV(s, v)
// #define LOGF(...)
#endif


#endif // LOG_H
