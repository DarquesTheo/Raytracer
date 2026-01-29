#include "Rectangle3D.hpp"
#include <iostream>

namespace Math
{
    Rectangle3D::Rectangle3D(Point3D origin, Vector3D bottom_side, Vector3D left_side)
        : origin(origin), bottom_side(bottom_side), left_side(left_side)
    {
    }

    Point3D Rectangle3D::pointAt(double u, double v) const
    {
        return Point3D(origin.x + bottom_side.x * u + left_side.x * v,
                       origin.y + bottom_side.y * u + left_side.y * v,
                       origin.z + bottom_side.z * u + left_side.z * v);
    }

    void Rectangle3D::translate(const Vector3D &offset)
    {
        origin = origin + offset;
    }

    void Rectangle3D::rotate(float xDeg, float yDeg, float zDeg)
    {
        Point3D center = pointAt(0.5, 0.5);
        
        Matrix4x4 rot = Matrix4x4::rotation(Vector3D(1,0,0), xDeg)
                            * Matrix4x4::rotation(Vector3D(0,1,0), yDeg)
                            * Matrix4x4::rotation(Vector3D(0,0,1), zDeg);
        
        bottom_side = rot * bottom_side;
        left_side = rot * left_side;
        
        origin = center - bottom_side/2 - left_side/2;

        Math::Vector3D normal = bottom_side.cross(left_side).normalized();
        left_side = normal.cross(bottom_side).normalized() * left_side.getLength();
    }
}
