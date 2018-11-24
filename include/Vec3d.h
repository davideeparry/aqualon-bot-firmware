#ifndef VEC3D_H
#define VEC3D_H

#include "Constants.h"

class Vec3d {
    public:
        Vec3d() {};
        Vec3d(float x, float y, float z) : x(x), y(y), z(z) {};
        float getX();
        float getY();
        float getZ();
        friend Vec3d operator+(Vec3d a, Vec3d b);
        friend Vec3d operator-(Vec3d a, Vec3d b);
        friend Vec3d operator*(Vec3d v, float s); // scalar multiplication
        friend Vec3d operator/(Vec3d v, float s); // scalar multiplication
    private:
        float x;
        float y;
        float z;
};

Vec3d rotate(Vec3d input, float angle);

#endif 