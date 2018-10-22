#ifndef COMM_ISR
#define COMM_ISR

#include "../state/globalObjs.h"

void communicationsISRWrapper() {
    communications.isr();
};

#endif