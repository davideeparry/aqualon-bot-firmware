#include "Point.h"

// Initialize the origin set flag. 
bool Point::origin_set = false;
double Point::origin_x = 0;
double Point::origin_y = 0;
double Point::lon_scale = 0;

Point::Point(double latitude, double longitude)
{
    lat = latitude;
    lon = longitude;
    if (!origin_set)
    {
        lon_scale = cos(latitude);
        origin_x = lat * EQUATORIAL_SCALE;
        origin_y = lon * lon_scale * EQUATORIAL_SCALE;
        origin_set = true;
        LOG("Point, setting origin");
        LOGV("    X: ", origin_x);
        LOGV("    Y: ", origin_y);
    }
}

Point::Point(Vec3d vec) {
    lat = vec.getX() / EQUATORIAL_SCALE;
    lon = vec.getY() / (lon_scale * EQUATORIAL_SCALE);
}

double Point::getLon() { return lon; }

double Point::getLat() { return lat; }

double Point::getX() { 
    return (lat * EQUATORIAL_SCALE) - origin_x;
}

double Point::getY() { 
    return (lon * lon_scale * EQUATORIAL_SCALE) - origin_y;
}

double Point::getDist() {
    return sqrt(pow(getX(), 2) + pow(getY(), 2));
}

Vec3d Point::getVec3d(double angle) {
    return Vec3d(getX(), getY(), angle);
}

// Return vector angle (in radians)
double Point::getAngle() {
    double x = getX();
    double y = getY();
    if(x == 0) return 0.0;
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
