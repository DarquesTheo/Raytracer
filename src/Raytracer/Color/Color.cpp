#include "Color.hpp"

#include <cmath>

namespace Raytracer {

    Color::Color() : _r(0), _g(0), _b(0) {}

    Color::Color(int r, int g, int b) : _r(r), _g(g), _b(b) {}

    int Color::getRed() const { return _r; }
    int Color::getGreen() const { return _g; }
    int Color::getBlue() const { return _b; }

    void Color::setRed(int r) { _r = r; }
    void Color::setGreen(int g) { _g = g; }
    void Color::setBlue(int b) { _b = b; }

    std::string Color::toStringRGB() {
        return std::to_string(_r) + " " +
               std::to_string(_g) + " " +
               std::to_string(_b);
    }

    void Color::writeColor(std::ostream &out) const
    {
        out << (unsigned char)_g << (unsigned char)_b << (unsigned char)_r;
    }

    Color Color::operator+(const Color &other) const {
        return Color(
            std::clamp(_r + other._r, 0, 255),
            std::clamp(_g + other._g, 0, 255),
            std::clamp(_b + other._b, 0, 255)
        );
    }

    Color Color::operator*(double scalar) const {
        return Color(
            std::clamp(static_cast<int>(_r * scalar), 0, 255),
            std::clamp(static_cast<int>(_g * scalar), 0, 255),
            std::clamp(static_cast<int>(_b * scalar), 0, 255)
        );
    }

    Color Color::operator*(const Color &other) const {
        return Color(
            std::min(255, (_r * other._r) / 255),   
            std::min(255, (_g * other._g) / 255),
            std::min(255, (_b * other._b) / 255)
        );
    }

    Color Color::operator/(double scalar) const {
        return Color(
            static_cast<int>(std::round(_r / scalar)),
            static_cast<int>(std::round(_g / scalar)),
            static_cast<int>(std::round(_b / scalar))
        );
    }
}