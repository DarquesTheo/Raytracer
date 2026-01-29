#include "Sphere.hpp"
#include <math.h>
#include <iostream>
#include "../Raytracer/BVH/BVH.hpp"

namespace Primitive {

    Sphere::Sphere(Math::Point3D center, double radius): _radius(radius) {
        _position = center;
        _normal = Math::Vector3D(center.x, center.y, center.z);
        _normal.normalize();
        bbox = Raytracer::AABB(
            Math::Point3D(center.x - radius, center.y - radius, center.z - radius),
            Math::Point3D(center.x + radius, center.y + radius, center.z + radius));
        _name = "Sphere";
    }

    Math::Vector3D Sphere::getNormalAtPoint(const Math::Point3D &point) const {
        Math::Vector3D normal(point.x - _position.x, point.y - _position.y, point.z - _position.z);
        normal.normalize();
        return normal;
    }

    bool Sphere::hit(Raytracer::Ray &ray, HitInfo &info) {
        double dx = ray.direction.x;
        double dy = ray.direction.y;
        double dz = ray.direction.z;

        double ox = ray.origin.x - _position.x;
        double oy = ray.origin.y - _position.y;
        double oz = ray.origin.z - _position.z;

        double a = dx * dx + dy * dy + dz * dz;
        double b = 2 * (dx * ox + dy * oy + dz * oz);
        double c = ox * ox + oy * oy + oz * oz - _radius * _radius;

        double discriminant = b * b - 4 * a * c;
        info.materials = _materials;

        if (discriminant < 0) {
            return false;
        }

        double discriminantSqrt = sqrt(discriminant);
        double t1 = (-b - discriminantSqrt) / (2.0 * a);
        double t2 = (-b + discriminantSqrt) / (2.0 * a);

        const double epsilon = 1e-6;
    
        double t = (t1 > epsilon && t1 < t2) ? t1 : (t2 > epsilon ? t2 : -1);
        Math::Point3D intersectionPoint = ray.origin + ray.direction * t;
            Math::Vector3D normal(intersectionPoint.x - _position.x, intersectionPoint.y - _position.y, intersectionPoint.z - _position.z);
            normal.normalize();
            if (ray.direction.dot(normal) > 0) {
                normal = normal * -1;
            }
            info.normal = normal;
        if (t > epsilon) {
            info.t = t;
            return true;
        }

        primitivesHit++;
        return false;
    }

    void Sphere::translate(const Math::Vector3D &translation) {
        _position = _position + translation;
        bbox = Raytracer::AABB(
            Math::Point3D(_position.x - _radius, _position.y - _radius, _position.z - _radius),
            Math::Point3D(_position.x + _radius, _position.y + _radius, _position.z + _radius));
    }

    Raytracer::AABB Sphere::boundingBox() const
    {
        return bbox;
    }
}