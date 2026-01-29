#pragma once

#include "Point3D.hpp"
#include <cmath>

namespace Math {

    class Vector3D {
    public:
        double x;
        double y;
        double z;

        Vector3D(double x = 0, double y = 0, double z = 0);
        Vector3D(const Point3D &point1, const Point3D &point2);
        ~Vector3D() = default;

        double getLength();
        double getSquaredLength();

        Point3D operator+(const Point3D &point) const;
        Vector3D operator+(const Vector3D& other) const;
        Vector3D operator-(const Vector3D& other) const;
        Vector3D operator*(const Vector3D& other) const;
        Vector3D operator/(const Vector3D& other) const;
        Vector3D operator*(double scalar) const;
        Vector3D operator/(double scalar) const;
        double dot(const Vector3D& other) const;
        Vector3D cross(const Vector3D& other) const;
        Vector3D& operator+=(const Vector3D& other);
        Vector3D& operator-=(const Vector3D& other);
        Vector3D& operator*=(const Vector3D& other);
        Vector3D& operator/=(const Vector3D& other);
        Vector3D& operator*=(double scalar);
        Vector3D& operator/=(double scalar);
        Vector3D& operator=(const Vector3D& other);
        Vector3D operator-() const;
        void normalize();
        Vector3D& normalized();
    private:
        double _length;
        double _lengthSquared;
        void computeLength();
        void computeSquaredLength();
    };

}