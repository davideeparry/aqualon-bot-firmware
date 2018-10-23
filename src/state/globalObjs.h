#ifndef GLOBAL_OBJS
#define GLOBAL_OBJS
#include "../comms/Communications.h"
#include "../gps/Gps.h"
#include "../motors/Motors.h"
#include "../mpu/Mpu.h"
#include "../power/Power.h"
#include "../db/Database.h"

Communications communications;
Gps gps;
Motors motors;
Mpu mpu;
Power power;
Database database;
StateService stateService;

#endif