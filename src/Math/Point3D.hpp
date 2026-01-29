/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Point3D
*/

#pragma once

namespace Math {
    class Vector3D;
    class Point3D {
        public:
            Point3D();
            Point3D(double x, double y, double z) : x(x), y(y), z(z) {};
            ~Point3D();
            double getLength() const;
            Math::Vector3D toVector() const;
            double x, y, z;
            Point3D operator+(const Vector3D &vector) const;
            Point3D operator-(const Vector3D &vector) const;
            Point3D operator+(const Point3D &point) const { return Point3D(x + point.x, y + point.y, z + point.z); }
            Point3D operator-(const Point3D &point) const { return Point3D(x - point.x, y - point.y, z - point.z); }
            double &operator[](int index);
    };
}
