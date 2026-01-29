#pragma once

#include <iostream>
#include <algorithm>

namespace Raytracer {
    class Color {
        public:
            Color();
            Color(int r, int g, int b);

            int getRed() const;
            int getGreen() const;
            int getBlue() const;

            void setRed(int r);
            void setGreen(int g);
            void setBlue(int b);

            Color operator+(const Color &other) const;

            Color operator*(double scalar) const;
            Color operator*(const Color &other) const;
            Color operator/(double scalar) const;

            std::string toStringRGB();
            void writeColor(std::ostream &out) const;

        private:
            int _r;
            int _g;
            int _b;
    };
}