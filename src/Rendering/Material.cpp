#include "Material.hpp"

bool Material::scatter(Raytracer::Ray &ray)
{
    if (_type == TRANSPARENCY || _type == REFRACTION || _type == REFLECTION) {
        return true;
    }
    return false;
}

EMaterialType Material::getType()
{
    return _type;
}

double Material::getPercentage()
{
    return _percentage;
}