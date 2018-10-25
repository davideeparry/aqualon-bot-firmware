#ifndef COMM_ISR
#define COMM_ISR 1

#include "Communications.h"
void communicationsISRWrapper() {
    Communications::instance().isr();
};

#endif