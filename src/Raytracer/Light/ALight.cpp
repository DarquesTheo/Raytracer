/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ALight
*/

#include "ALight.hpp"

namespace Raytracer
{
    ALight::ALight(const Math::Point3D &position, double intensity, const Raytracer::Color &color): _position(position), _intensity(intensity), _color(color)
    {
    }
}
