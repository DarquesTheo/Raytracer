#include "Vector3D.hpp"

#include <math.h>
#include <iostream>

namespace Math {

    double Vector3D::getLength() {
        if (_length != 0.0)
            return _length;
        computeLength();
        return _length;
    }

    double Vector3D::getSquaredLength() {
        if (_lengthSquared != 0.0)
            return _lengthSquared;
        computeSquaredLength();
        return _lengthSquared;
    }

    Point3D Vector3D::operator+(const Point3D &point) const {
        return Point3D(point.x + x, point.y + y, point.z + z);
    }

    Vector3D::Vector3D(double x1, double y1, double z1)
        : x(x1), y(y1), z(z1) {
        _length = 0.0;
        _lengthSquared = 0.0;
    }

    Vector3D::Vector3D(const Point3D &point1, const Point3D &point2)
        : x(point2.x - point1.x), y(point2.y - point1.y), z(point2.z - point1.z) {
        _length = 0.0;
        _lengthSquared = 0.0;
    }

    Vector3D Vector3D::operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3D Vector3D::operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Vector3D Vector3D::operator*(const Vector3D& other) const {
        return Vector3D(x * other.x, y * other.y, z * other.z);
    }

    Vector3D Vector3D::operator/(const Vector3D& other) const {
        return Vector3D(x / other.x, y / other.y, z / other.z);
    }

    Vector3D Vector3D::operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    Vector3D Vector3D::operator/(double scalar) const {
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }

    double Vector3D::dot(const Vector3D& other) const {
        return (x * other.x + y * other.y + z * other.z);
    }

    Vector3D Vector3D::cross(const Vector3D& other) const {
        return Vector3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    Vector3D& Vector3D::operator+=(const Vector3D& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        _length = 0.0;
        _lengthSquared = 0.0;
        return *this;
    }

    Vector3D& Vector3D::operator-=(const Vector3D& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        _length = 0.0;
        _lengthSquared = 0.0;
        return *this;
    }

    Vector3D& Vector3D::operator*=(const Vector3D& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        _length = 0.0;
        _lengthSquared = 0.0;
        return *this;
    }

    Vector3D& Vector3D::operator/=(const Vector3D& other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        _length = 0.0;
        _lengthSquared = 0.0;
        return *this;
    }

    Vector3D& Vector3D::operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        _length = 0.0;
        _lengthSquared = 0.0;
        return *this;
    }

    Vector3D& Vector3D::operator=(const Vector3D& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
            _length = other._length;
            _lengthSquared = other._lengthSquared;
        }
        return *this;
    }

    Vector3D Vector3D::operator-() const
    {
        return Vector3D(-x, -y, -z);
    }

    void Vector3D::computeLength() {
        if (_lengthSquared != 0) {
            _length = sqrt(_lengthSquared);
        }
        _length = sqrt(abs(x) * abs(x) + abs(y) * abs(y) + abs(z) * abs(z));
    }

    void Vector3D::computeSquaredLength() {
        _lengthSquared = abs(x) * abs(x) + abs(y) * abs(y) + abs(z) * abs(z);
    }

    void Vector3D::normalize() {
        computeLength();
        if (_length > 0) {
            x /= _length;
            y /= _length;
            z /= _length;
        }
    }

    Vector3D &Vector3D::normalized()
    {
        normalize();
        return *this;
    }
}