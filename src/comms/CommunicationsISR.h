#ifndef COMM_ISR
#define COMM_ISR 1

#include "../state/globalObjs.h"

void communicationsISRWrapper() {
    communications.isr();
};

#endif