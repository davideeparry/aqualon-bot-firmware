#ifndef GPS_ISR
#define GPS_ISR 1

#include "../state/globalObjs.h"
void gpsISRWrapper() {  // GPS TX/RX are plugged into Serial2
    gps.isr();
}
#endif