#pragma once

#include "ALight.hpp"
#include "../../Primitives/Plane.hpp"

namespace Raytracer {
    class PointLight : public ALight {
    public:
        PointLight(const Math::Point3D &position, double intensity, const Raytracer::Color &color);

        Raytracer::Color computeLighting(
            const Math::Point3D &intersectionPoint,
            const Math::Vector3D &normal,
            const BVHNode *spacePartition,
            const IPrimitives *currentPrimitive
        ) const override;
    };
}