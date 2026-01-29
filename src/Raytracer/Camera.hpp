#pragma once

#include "../Math/Point3D.hpp"
#include "../Math/Rectangle3D.hpp"
#include "../Primitives/Sphere.hpp"

namespace Raytracer
{
    struct Resolution
    {
        int width;
        int height;
    };

    class Camera
    {
    public:
        Camera(Math::Point3D origin, Math::Rectangle3D screen)
            : origin(origin), screen(screen) {resolution = {1080, 1080};}
        ~Camera() = default;

        Ray ray(double u, double v);
        void setResolution(int width, int height);
        Resolution getResolution() const { return resolution; }
        void translate(const Math::Vector3D &offset);
        Math::Rectangle3D getScreen();
        void setScreen(Math::Rectangle3D screen) { this->screen = screen; }
        bool antialiasing = false;
        int samplesPerAxis = 2;
        Math::Point3D origin;
        void rotate(float xDeg, float yDeg, float zDeg);
        void lookAt(const Math::Point3D& target);
        Math::Vector3D forwardDirection() const;
    private:
        Math::Rectangle3D screen;
        Resolution resolution;
    };
}