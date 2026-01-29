/*
** EPITECH PROJECT, 2025
** B-OOP-400-TLS-4-1-raytracer-antonin.veau
** File description:
** triangle
*/

#include "Triangle.hpp"
#include <cmath>
#include <algorithm>

namespace Primitive {

    Triangle::Triangle(Math::Point3D &v0, Math::Point3D &v1, Math::Point3D &v2)
        : _v0(v0), _v1(v1), _v2(v2) {
        Math::Vector3D edge1 = Math::Vector3D(v0, v1);
        Math::Vector3D edge2 = Math::Vector3D(v0, v2);
        _normal = edge1.cross(edge2).normalized();
        Math::Point3D minPoint(
            std::min({_v0.x, _v1.x, _v2.x}),
            std::min({_v0.y, _v1.y, _v2.y}),
            std::min({_v0.z, _v1.z, _v2.z})
        );

        Math::Point3D maxPoint(
            std::max({_v0.x, _v1.x, _v2.x}),
            std::max({_v0.y, _v1.y, _v2.y}),
            std::max({_v0.z, _v1.z, _v2.z})
        );
        bbox = Raytracer::AABB(minPoint, maxPoint); 
    }

    bool Triangle::hit(Raytracer::Ray &ray, HitInfo &info) {
        const double epsilon = 1e-6;
        Math::Vector3D edge1 = Math::Vector3D(_v0, _v1);
        Math::Vector3D edge2 = Math::Vector3D(_v0, _v2);
        Math::Vector3D h = ray.direction.cross(edge2);
        double a = edge1.dot(h);

        if (std::abs(a) < epsilon)
            return false;

        double f = 1.0 / a;
        Math::Vector3D s = Math::Vector3D(_v0, ray.origin);
        double u = f * s.dot(h);

        if (u < 0.0 || u > 1.0)
            return false;

        Math::Vector3D q = s.cross(edge1);
        double v = f * ray.direction.dot(q);

        if (v < 0.0 || u + v > 1.0)
            return false;
        double t = f * edge2.dot(q);

        if (t > epsilon) {
            info.t = t;
            info.p = ray.origin + ray.direction * t;
            info.normal = _normal;
            return true;
        }
        return false;
    }

    Math::Vector3D Triangle::getNormalAtPoint(const Math::Point3D &point) const {
        return _normal;
    }

    Raytracer::AABB Triangle::boundingBox() const {
        return bbox;
    }

    void Triangle::translate(const Math::Vector3D &translation) {
        _v0 = _v0 + translation;
        _v1 = _v1 + translation;
        _v2 = _v2 + translation;

        Math::Point3D minPoint(
            std::min({_v0.x, _v1.x, _v2.x}),
            std::min({_v0.y, _v1.y, _v2.y}),
            std::min({_v0.z, _v1.z, _v2.z})
        );

        Math::Point3D maxPoint(
            std::max({_v0.x, _v1.x, _v2.x}),
            std::max({_v0.y, _v1.y, _v2.y}),
            std::max({_v0.z, _v1.z, _v2.z})
        );
        bbox = Raytracer::AABB(minPoint, maxPoint); 
    }

}