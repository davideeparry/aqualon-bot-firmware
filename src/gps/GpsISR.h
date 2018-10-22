#ifndef GPS_ISR
#define GPS_ISR

#include "../state/globalObjs.h"
void gpsISRWrapper() {  // GPS TX/RX are plugged into Serial2
    gps.isr();
#endif