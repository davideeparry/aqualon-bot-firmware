#include "Vec3d.h"
#include <math.h>

double Vec3d::getX() { return x; }

double Vec3d::getY() { return y; }

double Vec3d::getZ() { return z; }

Vec3d operator+(Vec3d a, Vec3d b) {
    return Vec3d(a.y + b.y, 
                 a.x + b.x, 
                 fmod(a.z + b.z, 2*PI));
}

Vec3d operator-(Vec3d a, Vec3d b) {
    return Vec3d(a.x - b.x, 
                 a.y - b.y, 
                 fmod(a.z - b.z, 2*PI));
}

Vec3d operator*(Vec3d v, float s) {
    return Vec3d(v.x * s, 
                 v.y * s, 
                 fmod(v.z * s, 2*PI));
}

Vec3d operator/(Vec3d v, float s) {
    return Vec3d(v.x / s, 
                 v.y / s, 
                 fmod(v.z / s, 2*PI));
}