#ifndef POINT_H
#define POINT_H

#include <math.h>
#include "Constants.h"
#include "Vec3d.h"
#include "Logging.h"

// Defines a coordinate in latitude and longitude as well
// as meters relative to an arbitrary origin (established by the
// first Point declared)
// NOTE: North is the positive X axis and east is the positive Y axis. 
//       This is so that 0 degrees is north, in keeping with the values
//       returned by the GPS module. 
class Point {
    public:
        Point();
        Point(double latitude, double longitude);
        Point(Vec3d vec);
        double getLat();
        double getLon();
        float getX();
        float getY();
        float getDist();
        float getAngle();
        Vec3d getVec3d(float angle);
        friend Point operator+(Point a, Point b);
        friend Point operator-(Point a, Point b);
        operator bool() const { return valid; };
    private:
        static double originLat;
        static double originLon;
        static double lonScale;  // Longitude scaling factor, based on latitude
        static bool originSet;
        void setOrigin(double lat, double lon);

        bool valid;

        double lat;
        double lon;
};

#endif // POINT_H