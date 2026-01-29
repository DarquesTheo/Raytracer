/*
** EPITECH PROJECT, 2025
** B-OOP-400-TLS-4-1-raytracer-antonin.veau
** File description:
** triangle
*/

#pragma once

#include "APrimitives.hpp"
#include "../Math/Point3D.hpp"
#include "../Math/Vector3D.hpp"
#include "../Raytracer/Ray.hpp"
#include "../Raytracer/BVH/BVH.hpp"

namespace Primitive {

    class Triangle : public APrimitives {
    public:
        Triangle(Math::Point3D &v0, Math::Point3D &v1, Math::Point3D &v2);
        ~Triangle() override = default;

        bool hit(Raytracer::Ray &ray, HitInfo &info) override;
        void translate(const Math::Vector3D &translation) override;
        Math::Vector3D getNormalAtPoint(const Math::Point3D &point) const override;
        Raytracer::AABB boundingBox() const override;

    private:
        Math::Point3D _v0, _v1, _v2;
        Math::Vector3D _normal;
        Raytracer::AABB bbox;
    };

}