/*
 * Some simple macros for logging out of the serial port in simulation. 
 */


#ifndef LOG_H
#define LOG_H

#ifdef SIMULATION
#define LOG(s)      { Serial.print(millis()); Serial.print(": "); Serial.print(s); Serial.print("\n"); }
#define LOGV(s, v)  { Serial.print(millis()); Serial.print(": "); Serial.print(s); Serial.print(v); Serial.print("\n"); }
#define LOGX(s, v)  { Serial.print(millis()); Serial.print(": "); Serial.print(s); Serial.print(v, HEX); Serial.print("\n"); }
#else
#define LOG(s)
#define LOGV(s, v)
#define LOGX(s, v)
#endif

#endif // LOG_H
