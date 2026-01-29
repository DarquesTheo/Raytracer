#pragma once

#include "APrimitives.hpp"
#include "../Math/Point3D.hpp"
#include "../Raytracer/Ray.hpp"
#include "../Math/Vector3D.hpp"
#include "../Raytracer/BVH/BVH.hpp"

namespace Primitive {

    class Sphere : public APrimitives {
    public:
        Sphere(Math::Point3D center, double radius);
        ~Sphere() override = default;

        bool hit(Raytracer::Ray &ray, HitInfo &info) override;
        void translate(const Math::Vector3D &translation) override;
        Math::Vector3D getNormalAtPoint(const Math::Point3D &point) const;
        Raytracer::AABB boundingBox() const override;

    private:
        double _radius;
        Raytracer::AABB bbox;
    };

}