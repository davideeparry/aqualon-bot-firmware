#ifndef GPS_ISR
#define GPS_ISR 1

#include "Gps.h"
void gpsISRWrapper() {  // GPS TX/RX are plugged into Serial2
    Gps::instance().isr();
}
#endif