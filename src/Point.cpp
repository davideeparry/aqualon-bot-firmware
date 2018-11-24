#include "Point.h"

// Initialize the origin set flag. 
bool Point::originSet = false;
double Point::originLat = 0;
double Point::originLon = 0;
double Point::lonScale = 0;

// Default constructor creates invalid Point
// (evaluates to false in boolean context. See overloaded
// bool() operator)
Point:: Point() {
    valid = false;
}

Point::Point(double latitude, double longitude) {
    if (!originSet) setOrigin(latitude, longitude);
    lat = latitude;
    lon = longitude;
    valid = true;
}

Point::Point(Vec3d vec) {
    if (!originSet) setOrigin(lat, lon);
    lat = vec.getX() / EQUATORIAL_SCALE;
    lon = vec.getY() / (lonScale * EQUATORIAL_SCALE);
    valid = true;
}

// First Point constructed sets the arbitrary origin point
// for conversion to meters. 
void Point::setOrigin(double latitude, double longitude) {
    lonScale = cos(latitude * PI / 180.0);
    originLat = latitude + 0.000001;  // Add offset so point isn't right on 0
    originLon = longitude + 0.000001;
    originSet = true;
    LOG("Point, setting origin");
    LOGV("    Lat: ", latitude);
    LOGV("    Lon: ", longitude);
}

double Point::getLon() { return lon; }

double Point::getLat() { return lat; }

float Point::getX() { 
    return ((lat - originLat) * EQUATORIAL_SCALE);
}

float Point::getY() { 
    return ((lon - originLon) * lonScale * EQUATORIAL_SCALE);
}

float Point::getDist() {
    return sqrt(pow(getX(), 2) + pow(getY(), 2));
}

Vec3d Point::getVec3d(float angle) {
    return Vec3d(getX(), getY(), angle);
}

// Return vector angle (in radians)
float Point::getAngle() {
    float x = getX();
    float y = getY();
    if(x == 0) return 0.0;
    double angle = fmod(atan2(y,x) + 2*PI, 2*PI);
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
