#pragma once

#include "ALight.hpp"


namespace Raytracer {
    class GlobalLight : public ALight {
        public:
            GlobalLight(const Math::Point3D &position, double intensity, const Raytracer::Color &color);

            Raytracer::Color computeLighting(
                const Math::Point3D &intersectionPoint,
                const Math::Vector3D &normal,
                const BVHNode *spacePartition,
                const IPrimitives *currentPrimitive
            ) const;
    };
}
