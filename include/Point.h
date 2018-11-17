#ifndef POINT_H
#define POINT_H

#include <math.h>
#include "Constants.h"
#include "Vec3d.h"
#include "Debug.h"

// Defines a coordinate in latitude and longitude as well
// as meters relative to an arbitrary origin (established by the
// first Point declared)
// NOTE: North is the positive X axis and east is the positive Y axis. 
//       This is so that 0 degrees is north, in keeping with the values
//       returned by the GPS module. 
class Point {
    public:
        Point() {};
        Point(double latitude, double longitude);
        Point(Vec3d vec);
        double getLat();
        double getLon();
        double getX();
        double getY();
        double getDist();
        double getAngle();
        Vec3d getVec3d(double angle);
        friend Point operator+(Point a, Point b);
        friend Point operator-(Point a, Point b);
    private:
        static double origin_x;
        static double origin_y;
        static double lon_scale;  // Longitude scaling factor, based on latitude
        static bool origin_set;

        double lat;
        double lon;
};

#endif // POINT_H