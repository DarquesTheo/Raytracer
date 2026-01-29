#include "Plane.hpp"

#include <iostream>

namespace Primitive {

    Plane::Plane(Math::Point3D point, Math::Vector3D normal): APrimitives(), _point(point) {
        _normal = normal;
        _position = point;
        _normal.normalize();
        _rotation = Math::Matrix4x4();
        _invRotation = Math::Matrix4x4();

        const double INF = 1e5;
        bbox = Raytracer::AABB(Math::Point3D(-INF, -INF, -INF), Math::Point3D(INF, INF, INF));
        _name = "Plane";
    };

    Math::Vector3D Plane::getNormalAtPoint(const Math::Point3D &point) const {
        return (_rotation * _normal).normalized();
    }

    bool Plane::hit(Raytracer::Ray &ray, HitInfo &info) {
        Math::Point3D localOrigin = _invRotation * (ray.origin - _position);
        Math::Vector3D localDirection = _invRotation * ray.direction;
        Raytracer::Ray localRay(localOrigin, localDirection.normalized());
        
        Math::Vector3D localNormal = _normal;
        
        double d = localNormal.dot(localRay.direction);
        info.materials = _materials;
        
        const double epsilon = 1e-8;
        if (std::abs(d) < epsilon)
            return false;

        Math::Vector3D originToPlane(localRay.origin, _point);
        double tHit = originToPlane.dot(localNormal) / d;
        
        if (tHit < epsilon)
            return false;

        info.t = tHit;

        Math::Vector3D worldNormal = (_rotation * localNormal).normalized();
        
        if (d > 0)
            info.normal = worldNormal * -1;
        else
            info.normal = worldNormal;

        info.p = ray.origin + ray.direction * tHit;
        
        primitivesHit++;
        return true;
    }

    void Plane::translate(const Math::Vector3D &translation) {
        _position = _position + translation;
        _point = _point + translation;
    }

    Raytracer::AABB Plane::boundingBox() const {
        return bbox;
    }
}
