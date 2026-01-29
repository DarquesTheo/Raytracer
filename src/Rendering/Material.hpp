#pragma once

#include "../Raytracer/Color/Color.hpp"
#include "../Raytracer/Ray.hpp"

typedef enum EMaterialType
{
    FLAT,
    TRANSPARENCY,
    REFRACTION,
    REFLECTION
} EMaterialType;

class Material
{
public:
    Material(EMaterialType type, double percentage)
        : _type(type), _percentage(percentage) {}
    ~Material() = default;

    bool scatter(Raytracer::Ray &ray);

    EMaterialType getType();

    double getPercentage();

private:
    EMaterialType _type;
    double _percentage;
};