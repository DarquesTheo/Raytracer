#pragma once

#include "APrimitives.hpp"
#include "../Math/Point3D.hpp"
#include "../Math/Vector3D.hpp"
#include "../Raytracer/BVH/BVH.hpp"

namespace Primitive {

class Cone : public APrimitives {
public:
    Cone(const Math::Point3D& baseCenter, const Math::Vector3D& axis, 
         float height, float baseRadius);
    
    bool hit(Raytracer::Ray& ray, HitInfo& info) override;
    Math::Vector3D getNormalAtPoint(const Math::Point3D& point) const override;
    Raytracer::AABB boundingBox() const override;
    void translate(const Math::Vector3D& translation) override;

private:
    Math::Vector3D _axis;
    float _height;
    float _baseRadius;

    bool intersectConeBody(const Raytracer::Ray& localRay, float& t) const;
    bool intersectBaseDisk(const Raytracer::Ray& localRay, float& t) const;
};

}