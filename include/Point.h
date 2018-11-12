#ifndef POINT_H
#define POINT_H

#include <math.h>
#include "Constants.h"

class Point {
    public:
        Point(double latitude, double longitude);
        double getLat();
        double getLon();
        double getX();
        double getY();
        double getDist();
        double getAngle();
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