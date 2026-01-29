#pragma once

#include "ILight.hpp"


namespace Raytracer
{
    class ALight : public ILight {
        public:
            ALight(const Math::Point3D &position, double intensity, const Raytracer::Color &color);

            ~ALight() override = default;

            Math::Point3D getPosition() const override { return _position; }
            double getIntensity() const override { return _intensity; }
            Raytracer::Color getColor() const override { return _color; }

        protected:
            Math::Point3D _position;
            double _intensity;
            Raytracer::Color _color;
    };
}
