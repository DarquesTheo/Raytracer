#include "GlobalLight.hpp"

namespace Raytracer {
    GlobalLight::GlobalLight(const Math::Point3D &position, double intensity, const Raytracer::Color &color)
        : ALight(position, intensity, color) {}

    Raytracer::Color GlobalLight::computeLighting(
        const Math::Point3D &intersectionPoint,
        const Math::Vector3D &normal,
        const BVHNode *spacePartition,
        const IPrimitives *currentPrimitive
    ) const {
        return getColor() * getIntensity() * currentPrimitive->getColor();
    }
}