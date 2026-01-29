#include "Cone.hpp"
#include <cmath>
#include <algorithm>

namespace Primitive {

Cone::Cone(const Math::Point3D& baseCenter, const Math::Vector3D& axis, 
           float height, float baseRadius)
    : _axis(axis), _height(height), _baseRadius(baseRadius)
{
    _position = baseCenter;
    _axis = _axis.normalized();
    _name = "Cone";
    addMaterial(std::make_shared<Material>(FLAT, 1.0));
}

bool Cone::hit(Raytracer::Ray& ray, HitInfo& info) {
    // Transform ray to local space
    Math::Point3D localOrigin = _invRotation * (ray.origin - _position);
    Math::Vector3D localDirection = _invRotation * ray.direction;
    Raytracer::Ray localRay(localOrigin, localDirection.normalized());

    float t_min = std::numeric_limits<float>::max();
    bool hit = false;
    float t_body, t_disk;

    // Check cone body
    if (intersectConeBody(localRay, t_body)) {
        t_min = t_body;
        hit = true;
    }

    // Check base disk
    if (intersectBaseDisk(localRay, t_disk)) {
        if (t_disk < t_min) {
            t_min = t_disk;
        }
        hit = true;
    }

    if (hit) {
        // Transform back to world space
        Math::Point3D hitPointLocal = localRay.origin + localRay.direction * t_min;
        Math::Vector3D localNormal = getNormalAtPoint(hitPointLocal);
        
        info.p = _rotation * hitPointLocal + _position;
        info.normal = (_rotation * localNormal).normalized();
        info.t = static_cast<double>(t_min);
        info.materials = _materials;
        return true;
    }

    return false;
}

bool Cone::intersectConeBody(const Raytracer::Ray& ray, float& t) const {
    const float angle = std::atan(_baseRadius / _height);
    const float k = std::pow(std::tan(angle), 2);
    const float a = ray.direction.x * ray.direction.x 
                  + ray.direction.z * ray.direction.z
                  - k * ray.direction.y * ray.direction.y;
    const float b = 2 * (ray.direction.x * ray.origin.x 
                       + ray.direction.z * ray.origin.z
                       - k * ray.direction.y * (ray.origin.y - _height));
    const float c = ray.origin.x * ray.origin.x 
                  + ray.origin.z * ray.origin.z
                  - k * std::pow(ray.origin.y - _height, 2);

    const float discriminant = b*b - 4*a*c;
    if (discriminant < 0) return false;

    const float sqrt_disc = std::sqrt(discriminant);
    const float t1 = (-b - sqrt_disc) / (2*a);
    const float t2 = (-b + sqrt_disc) / (2*a);

    auto check_height = [&](float t_val) {
        float y = ray.origin.y + t_val * ray.direction.y;
        return y >= 0 && y <= _height;
    };

    t = std::numeric_limits<float>::max();
    bool hit = false;

    if (t1 > 0.001f && check_height(t1)) {
        t = t1;
        hit = true;
    }
    if (t2 > 0.001f && check_height(t2) && t2 < t) {
        t = t2;
        hit = true;
    }

    return hit;
}

bool Cone::intersectBaseDisk(const Raytracer::Ray& ray, float& t) const {
    if (std::abs(ray.direction.y) < 1e-6f) return false;

    t = -ray.origin.y / ray.direction.y;
    if (t < 0.001f) return false;

    const Math::Point3D p = ray.origin + ray.direction * t;
    const float dist2 = p.x*p.x + p.z*p.z;
    return (dist2 <= _baseRadius*_baseRadius);
}

Math::Vector3D Cone::getNormalAtPoint(const Math::Point3D& point) const {
    Math::Point3D localPoint = _invRotation * (point - _position);
    
    // Base disk normal
    if (std::abs(localPoint.y) < 1e-4f) {
        return Math::Vector3D(0, -1, 0);
    }
    
    // Cone body normal
    const float r = _baseRadius * (1 - localPoint.y / _height);
    Math::Vector3D normal(localPoint.x/r, _baseRadius/_height, localPoint.z/r);
    return normal.normalized();
}

Raytracer::AABB Cone::boundingBox() const {
    Math::Point3D apex = _position + _axis * _height;
    float maxRadius = std::max(_baseRadius, static_cast<float>(_height * std::tan(std::atan(_baseRadius/_height))));
    
    Math::Vector3D minPoint(
        std::min(_position.x, apex.x) - maxRadius,
        std::min(_position.y, apex.y) - maxRadius,
        std::min(_position.z, apex.z) - maxRadius
    );
    
    Math::Vector3D maxPoint(
        std::max(_position.x, apex.x) + maxRadius,
        std::max(_position.y, apex.y) + maxRadius,
        std::max(_position.z, apex.z) + maxRadius
    );

    return Raytracer::AABB(
        Math::Point3D(minPoint.x, minPoint.y, minPoint.z),
        Math::Point3D(maxPoint.x, maxPoint.y, maxPoint.z)
    );
}

    void Cone::translate(const Math::Vector3D& translation) {
        _position = _position + translation;
    }
}