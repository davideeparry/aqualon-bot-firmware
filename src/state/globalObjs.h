#ifndef GLOBAL_OBJS
#define GLOBAL_OBJS 1
#include "../comms/Communications.h"
#include "../gps/Gps.h"
#include "../motors/Motors.h"
#include "../mpu/Mpu.h"
#include "../power/Power.h"
#include "../db/Database.h"

Database database;
Communications communications;
Gps gps;
Motors motors;
Mpu mpu;
Power power;
StateService stateService;

#endif