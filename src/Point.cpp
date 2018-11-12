#include "Point.h"

// Initialize the origin set flag. 
bool Point::origin_set = false;

Point::Point(double latitude, double longitude)
{
    lat = latitude;
    lon = longitude;
    if (!origin_set)
    {
        lon_scale = cos(latitude);
        origin_x = lon * lon_scale * EQUATORIAL_SCALE;
        origin_y = lat * EQUATORIAL_SCALE;
        origin_set = true;
    }
}

double Point::getLon() { return lon; }

double Point::getLat() { return lat; }

double Point::getX() { 
    return (lon * lon_scale * EQUATORIAL_SCALE) - origin_x;
}

double Point::getX() { 
    return (lat * EQUATORIAL_SCALE) - origin_y;
}

double Point::getDist() {
    return sqrt(pow(getX(), 2) + pow(getY(), 2));
}

double Point::getAngle() {
    double x = getX();
    double y = getY();
    double angle = atan(y / x);
    if(x < 0) {
        angle = PI + angle;
    } else if(y < 0) {
        angle = 2 * PI + angle;
    }
    return angle;
}

Point operator+(Point a, Point b)
{
    return Point(a.getLat() + b.getLat(), a.getLon() + b.getLon());
}

Point operator-(Point a, Point b)
{
    return Point(a.getLat() - b.getLat(), a.getLon() - b.getLon());
}
