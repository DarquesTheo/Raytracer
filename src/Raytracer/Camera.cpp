#include "Camera.hpp"

#include <iostream>
#include <cmath>

namespace Raytracer
{

    Ray Camera::ray(double u, double v)
    {
        Math::Point3D point = screen.pointAt(u, v);
        Math::Vector3D direction(point.x - origin.x, point.y - origin.y, point.z - origin.z);
        direction.normalize();

        return Ray(origin, direction);
    }

    void Camera::setResolution(int width, int height)
    {
        resolution.width = width;
        resolution.height = height;
    }

    void Camera::translate(const Math::Vector3D &offset)
    {
        origin = origin + offset;
        screen.translate(offset);
    }

    Math::Rectangle3D Camera::getScreen()
    {
        return screen;
    }

    void Camera::rotate(float xDeg, float yDeg, float zDeg) 
    {
        Math::Point3D screenCenter = screen.pointAt(0.5, 0.5);
        
        Math::Vector3D forwardDir = forwardDirection();
        float distance = (screenCenter - origin).getLength();
        
        float screenWidth = screen.bottom_side.getLength();
        float screenHeight = screen.left_side.getLength();
        
        Math::Matrix4x4 rotMatrix = Math::Matrix4x4::rotation(Math::Vector3D(1, 0, 0), xDeg) *
                                    Math::Matrix4x4::rotation(Math::Vector3D(0, 1, 0), yDeg) *
                                    Math::Matrix4x4::rotation(Math::Vector3D(0, 0, 1), zDeg);
        
        forwardDir = rotMatrix * forwardDir;
        forwardDir.normalize();
        
        Math::Point3D newScreenCenter = origin + forwardDir * distance;
        
        Math::Vector3D worldUp(0.0, 1.0, 0.0);
        
        if (std::abs(forwardDir.dot(worldUp)) > 0.99) {
            worldUp = Math::Vector3D(0.0, 0.0, 1.0);
        }
        
        Math::Vector3D rightDir = forwardDir.cross(worldUp);
        rightDir.normalize();
        
        Math::Vector3D upDir = rightDir.cross(forwardDir);
        upDir.normalize();
        
        Math::Vector3D newBottomSide = rightDir * screenWidth;
        Math::Vector3D newLeftSide = upDir * screenHeight;

        newLeftSide.y = - newLeftSide.y;
        
        Math::Point3D newScreenOrigin = newScreenCenter - newBottomSide * 0.5 - newLeftSide * 0.5;
        
        screen = Math::Rectangle3D(newScreenOrigin, newBottomSide, newLeftSide);
    }

    void Camera::lookAt(const Math::Point3D& target)
    {
        Math::Vector3D newForward(target.x - origin.x,
                                 target.y - origin.y,
                                 target.z - origin.z);
        newForward.normalize();
        
        Math::Vector3D worldUp(0, 1, 0);
        
        if (std::abs(newForward.dot(worldUp)) > 0.99) {
            worldUp = Math::Vector3D(0, 0, 1);
        }
        
        Math::Vector3D right = newForward.cross(worldUp).normalized();
        Math::Vector3D up = right.cross(newForward).normalized();
        
        float screenWidth = screen.bottom_side.getLength();
        float screenHeight = screen.left_side.getLength();
        
        Math::Point3D currentCenter = screen.pointAt(0.5, 0.5);
        float distance = (currentCenter - origin).getLength();
        
        Math::Point3D newScreenCenter = origin + newForward * distance;
        
        Math::Vector3D newBottomSide = right * screenWidth;
        Math::Vector3D newLeftSide = up * screenHeight;

        newLeftSide.y = - newLeftSide.y;
        
        Math::Point3D newScreenOrigin = newScreenCenter - newBottomSide * 0.5 - newLeftSide * 0.5;
        
        screen = Math::Rectangle3D(newScreenOrigin, newBottomSide, newLeftSide);
    }

    Math::Vector3D Camera::forwardDirection() const
    {
        Math::Point3D center = screen.pointAt(0.5, 0.5);
        Math::Vector3D dir(center.x - origin.x,
                          center.y - origin.y,
                          center.z - origin.z);
        dir.normalize();
        return dir;
    }
}
