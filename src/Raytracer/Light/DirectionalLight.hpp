#pragma once

#include "ALight.hpp"
#include "../../Math/Vector3D.hpp"
#include "../BVH/BVH.hpp"

namespace Raytracer {
    class DirectionalLight : public ALight {
        public:
            DirectionalLight(const Math::Vector3D &direction, double intensity, const Raytracer::Color &color);

            Raytracer::Color computeLighting(
                const Math::Point3D &intersectionPoint,
                const Math::Vector3D &normal,
                const BVHNode *spacePartition,
                const IPrimitives *currentPrimitive
            ) const override;

        private:
            Math::Vector3D _direction;
    };
}
