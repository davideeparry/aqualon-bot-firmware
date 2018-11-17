#ifndef CONSTANTS_H
#define CONSTANTS_H

// Geometric
#define PI                  3.14159265359
#define EQUATORIAL_SCALE    111111.0 // meters per longitude/latitude at equator

// Navigation
// Some of these should just be defaults for changeable values stored in the nav object
#define MAX_WAYPOINTS       128
#define DISCOVERY_TIME_MS   3000
#define GPS_MIN_SPEED       0.5
#define GPS_WEIGHT          0.5
#define DEFAULT_P_GAIN      0.8
#define DEFAULT_D_GAIN      0
#define DEFAULT_I_GAIN      0
#define DEFAULT_I_CLAMP     (0.1 * PI)
#define DEFAULT_DRIVE_GAIN  (2.0)

#endif