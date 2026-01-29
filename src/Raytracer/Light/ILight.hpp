/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ILight
*/

#pragma once

#include "../../Math/Point3D.hpp"
#include "../../Math/Vector3D.hpp"
#include "../Color/Color.hpp"
#include "../../Primitives/IPrimitives.hpp"
#include "../BVH/BVH.hpp"
#include <vector>

namespace Raytracer
{
    class ILight {
        public:
            virtual ~ILight() = default;

            virtual Math::Point3D getPosition() const = 0;

            virtual double getIntensity() const = 0;

            virtual Raytracer::Color getColor() const = 0;

            // Nouvelle méthode pour calculer l'effet de la lumière
            virtual Raytracer::Color computeLighting(
                const Math::Point3D &intersectionPoint,
                const Math::Vector3D &normal,
                const BVHNode *spacePartition,
                const IPrimitives *currentPrimitive
            ) const = 0;
    };
}
