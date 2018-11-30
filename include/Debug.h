/*
 * Some simple macros for logging out of the serial port in simulation. 
 */

#ifndef DEBUG_H
#define DEBUG_H

#include "Database.h"
#include <Arduino.h>

extern char debugLogBuff[256];

#if defined(SIMULATION) || defined(LABBUILD)

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
