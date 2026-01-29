#include "Cylinder.hpp"
#include <limits>

namespace Primitive {

    Cylinder::Cylinder(const Math::Point3D &center, double radius, double height):
        _radius(radius), _height(height)
    {
        _position = center;
        _rotation = Math::Matrix4x4();
        _invRotation = Math::Matrix4x4();
        _name = "Cylinder";
    }

    bool Cylinder::hit(Raytracer::Ray &ray, HitInfo &info) {
        Math::Point3D localOrigin = _invRotation * (ray.origin - _position);
        Math::Vector3D localDirection = _invRotation * ray.direction;
        Raytracer::Ray localRay(localOrigin, localDirection.normalized());
    
        double a = localRay.direction.x * localRay.direction.x + localRay.direction.z * localRay.direction.z;
        double b = 2 * (localRay.direction.x * localRay.origin.x + localRay.direction.z * localRay.origin.z);
        double c = localRay.origin.x * localRay.origin.x + localRay.origin.z * localRay.origin.z - _radius * _radius;
        
        double discriminant = b * b - 4 * a * c;
        bool hit = false;
        double t_min = std::numeric_limits<double>::max();
    
        // Check cylinder body
        if (discriminant >= 0 && a > 0.0001) {
            double sqrt_disc = sqrt(discriminant);
            double t1 = (-b - sqrt_disc) / (2 * a);
            double t2 = (-b + sqrt_disc) / (2 * a);
    
            auto check_height = [&](double t) {
                double y = localRay.origin.y + t * localRay.direction.y;
                return (y >= -_height/2) && (y <= _height/2);
            };
    
            if (t1 > 0.001 && check_height(t1) && t1 < t_min) {
                t_min = t1;
                hit = true;
            }
            if (t2 > 0.001 && check_height(t2) && t2 < t_min) {
                t_min = t2;
                hit = true;
            }
        }
    
        // Check top cap
        if (std::abs(localRay.direction.y) > 0.0001) {
            double t_top = (_height/2 - localRay.origin.y) / localRay.direction.y;
            if (t_top > 0.001 && t_top < t_min) {
                Math::Point3D p = localRay.origin + localRay.direction * t_top;
                if (p.x * p.x + p.z * p.z <= _radius * _radius) {
                    t_min = t_top;
                    hit = true;
                }
            }
        }
    
        // Check bottom cap
        if (std::abs(localRay.direction.y) > 0.0001) {
            double t_bottom = (-_height/2 - localRay.origin.y) / localRay.direction.y;
            if (t_bottom > 0.001 && t_bottom < t_min) {
                Math::Point3D p = localRay.origin + localRay.direction * t_bottom;
                if (p.x * p.x + p.z * p.z <= _radius * _radius) {
                    t_min = t_bottom;
                    hit = true;
                }
            }
        }
    
        if (hit) {
            Math::Point3D localHit = localRay.origin + localRay.direction * t_min;
            Math::Vector3D localNormal;
    
            if (localHit.y >= _height/2 - 0.001) { // Top cap
                localNormal = Math::Vector3D(0, 1, 0);
            }
            else if (localHit.y <= -_height/2 + 0.001) { // Bottom cap
                localNormal = Math::Vector3D(0, -1, 0);
            }
            else {
                localNormal = Math::Vector3D(localHit.x, 0, localHit.z).normalized();
            }
    
            // Transform back to world space
            info.p = _rotation * localHit + _position;
            info.normal = (_rotation * localNormal).normalized();
            info.t = t_min;
            return true;
        }
        return false;
    }

    Math::Vector3D Cylinder::getNormalAtPoint(const Math::Point3D &point) const {
        double y_top = _position.y + _height / 2.0;
        double y_bottom = _position.y - _height / 2.0;

        // Si le point est sur le dessus ou le dessous du cylindre
        if (std::abs(point.y - y_top) < 1e-3)
            return Math::Vector3D(0, 1, 0);
        if (std::abs(point.y - y_bottom) < 1e-3)
            return Math::Vector3D(0, -1, 0);

        // Sinon c'est la surface latérale
        Math::Vector3D normal(point.x - _position.x, 0.0, point.z - _position.z);
        normal.normalize();
        return normal;
    }

    void Cylinder::translate(const Math::Vector3D &translation) {
        _position = _position + translation;
    }

    Raytracer::AABB Cylinder::boundingBox() const {
        return Raytracer::AABB(
            Math::Point3D(_position.x - _radius, _position.y - _height / 2.0, _position.z - _radius),
            Math::Point3D(_position.x + _radius, _position.y + _height / 2.0, _position.z + _radius));
    }
}
