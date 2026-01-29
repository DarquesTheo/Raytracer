#pragma once

#include "Vector3D.hpp"
#include "Point3D.hpp"
#include <cmath>

namespace Math {

    class Matrix4x4 {
    public:
        float m[4][4];

        Matrix4x4();

        static Matrix4x4 rotation(const Vector3D &axis, float angleDegrees);
        Matrix4x4 inverse() const;
        static Matrix4x4 translate(const Vector3D &offset);
        static Matrix4x4 rotateAroundPoint(const Vector3D &axis, float angleDeg, const Point3D &pivot);

        Point3D operator*(const Point3D &p) const;
        Vector3D operator*(const Vector3D &v) const;
        Matrix4x4 operator*(const Matrix4x4 &other) const;
    };

}
