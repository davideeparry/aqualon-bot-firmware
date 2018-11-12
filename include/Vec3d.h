#ifndef VEC3D_H
#define VEC3D_H

#include "Constants.h"

class Vec3d {
    public:
        Vec3d(double x, double y, double z) : x(x), y(y), z(z) {};
        double getX();
        double getY();
        double getZ();
        friend Vec3d operator+(Vec3d a, Vec3d b);
        friend Vec3d operator-(Vec3d a, Vec3d b);
    private:
        double x;
        double y;
        double z;
};

#endif 