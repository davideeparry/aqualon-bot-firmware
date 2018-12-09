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

#endif // LOG_H
