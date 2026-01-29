/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Point3D
*/

#include "Point3D.hpp"
#include "Vector3D.hpp"

namespace Math {
    Point3D::Point3D()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    Point3D::~Point3D()
    {
    }

    double Point3D::getLength() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    Vector3D Point3D::toVector() const {
        return Vector3D(x, y, z);
    }

    Point3D Point3D::operator+(const Vector3D &vector) const
    {
        return Point3D(x + vector.x, y + vector.y, z + vector.z);
    }

    double &Point3D::operator[](int index)
    {
        switch (index)
        {
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        default:
            return z;
            break;
        }
    }
    Point3D Point3D::operator-(const Vector3D &vector) const
    {
        return Point3D(x - vector.x, y - vector.y, z - vector.z);
    }
}
