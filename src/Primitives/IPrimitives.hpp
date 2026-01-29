/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IPrimitives
*/

#pragma once

#include "../Math/Point3D.hpp"
#include "../Raytracer/Ray.hpp"
#include "../Raytracer/Color/Color.hpp"
#include "../Raytracer/HitInfo.hpp"
#include "../Math/Matrix4x4.hpp"

namespace Raytracer {
    class AABB;
}

class IPrimitives {
    public:
        virtual ~IPrimitives() = default;

        virtual bool hit(Raytracer::Ray &ray, HitInfo &info) = 0;
        virtual Math::Vector3D getNormalAtPoint(const Math::Point3D &point) const = 0;
        virtual void setColor(Raytracer::Color color) = 0;
        virtual Raytracer::Color getColor() const = 0;
        virtual Math::Point3D getPosition() const = 0;
        virtual Math::Vector3D getNormal() const = 0;
        virtual void translate(const Math::Vector3D &translation) = 0;
        virtual bool isPlane() const { return false; }
        virtual Raytracer::AABB boundingBox() const = 0;
        virtual void addMaterial(std::shared_ptr<Material> material) = 0;
        virtual std::vector<std::shared_ptr<Material>> getMaterials() = 0;
        virtual std::string getName() = 0;
        static inline int primitivesHit = 0;
        virtual void rotate(float xDegrees, float yDegrees, float zDegrees) = 0;
};
