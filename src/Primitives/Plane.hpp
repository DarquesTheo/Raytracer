#pragma once

#include "APrimitives.hpp"
#include "../Math/Point3D.hpp"
#include "../Raytracer/Ray.hpp"
#include "../Math/Vector3D.hpp"
#include "../Math/Rectangle3D.hpp"
#include "../Raytracer/BVH/BVH.hpp"

namespace Primitive {

    class Plane : public APrimitives {
    public:
        Plane(Math::Point3D point, Math::Vector3D normal);
        ~Plane() override = default;

        bool hit(Raytracer::Ray &ray, HitInfo &info) override;
        void translate(const Math::Vector3D &translation) override;
        Math::Vector3D getNormalAtPoint(const Math::Point3D &point) const;

        Math::Point3D getPoint() const { return _point; }
        Math::Vector3D getNormal() const { return _normal; }
        bool isPlane() const override { return true; }
        Raytracer::AABB boundingBox() const override;

    private:
        Math::Point3D _point;
        Raytracer::AABB bbox;
    };

}