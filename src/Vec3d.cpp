#include "Vec3d.h"
#include <math.h>

float Vec3d::getX() { return x; }

float Vec3d::getY() { return y; }

float Vec3d::getZ() { return z; }

Vec3d operator+(Vec3d a, Vec3d b) {
    return Vec3d(a.x + b.x, 
                 a.y + b.y, 
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
                 v.z * s);
}

Vec3d operator/(Vec3d v, float s) {
    return Vec3d(v.x / s, 
                 v.y / s, 
                 v.z / s);
}

Vec3d rotate(Vec3d v, float angle) {
    float c = cos(angle); float s = sin(angle);
    float x = v.getX() * c - v.getY() * s;
    float y = v.getX() * s - v.getY() * c;
    return Vec3d(x, y, v.getZ());
}