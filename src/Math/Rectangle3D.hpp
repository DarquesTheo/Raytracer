#pragma once

#include "Point3D.hpp"
#include "Vector3D.hpp"
#include "Matrix4x4.hpp"

namespace Math {
    class Rectangle3D {
    public:
        Rectangle3D(Point3D origin, Vector3D bottom_side, Vector3D left_side);
        ~Rectangle3D() = default;
        Point3D pointAt(double u, double v) const;
        Point3D origin;
        Vector3D bottom_side;
        Vector3D left_side;
        void translate(const Vector3D &offset);
        void rotate(float xDeg, float yDeg, float zDeg);
    };

}