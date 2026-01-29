/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ray
*/

#include "Ray.hpp"

namespace Raytracer {
    Ray::Ray()
    {
        origin = Math::Point3D();
        direction = Math::Vector3D();
    }

    Ray::Ray(Math::Point3D origin, Math::Vector3D direction): origin(origin), direction(direction) {}

    Ray::~Ray()
    {
    }
}
