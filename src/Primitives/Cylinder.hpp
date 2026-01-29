#pragma once

#include "APrimitives.hpp"
#include "../Math/Point3D.hpp"
#include "../Raytracer/Ray.hpp"
#include "../Math/Vector3D.hpp"
#include "../Raytracer/BVH/BVH.hpp"
#include "../Math/Matrix4x4.hpp"

namespace Primitive {

    class Cylinder : public APrimitives {
    public:
        Cylinder(const Math::Point3D &center, double radius, double height);
        ~Cylinder() override = default;

        bool hit(Raytracer::Ray &ray, HitInfo &info) override;
        Math::Vector3D getNormalAtPoint(const Math::Point3D &point) const override;
        void translate(const Math::Vector3D &translation) override;
        Math::Vector3D getNormal() const override { return Math::Vector3D(0, 1, 0); }
        Raytracer::AABB boundingBox() const override;

    private:
        double _radius;
        double _height;
        Raytracer::AABB bbox;
    };

}
