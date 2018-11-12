#include "Vec3d.h"

double Vec3d::getX() { return x; }

double Vec3d::getY() { return y; }

double Vec3d::getZ() { return z; }

Vec3d operator+(Vec3d a, Vec3d b) {
    return Vec3d(a.getX() + b.getX(), 
                 a.getY() + b.getY(), 
                 a.getZ() + b.getZ());
}

Vec3d operator-(Vec3d a, Vec3d b) {
    return Vec3d(a.getX() - b.getX(), 
                 a.getY() - b.getY(), 
                 a.getZ() - b.getZ());
}