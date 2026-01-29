/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ray
*/

#pragma once

#include "../Math/Vector3D.hpp"
#include "../Math/Point3D.hpp"

namespace Raytracer {
    class Ray {
        public:
            Ray();
            Ray(Math::Point3D origin, Math::Vector3D direction);
            ~Ray();
            Math::Point3D origin;
            Math::Vector3D direction;
    };
}
