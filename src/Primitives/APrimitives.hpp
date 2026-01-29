/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** APrimitives
*/

#pragma once

#include "IPrimitives.hpp"
#include "../Rendering/Material.hpp"
#include <vector>

class APrimitives : public IPrimitives {
    public:
        APrimitives();
        ~APrimitives() = default;
        virtual bool hit(Raytracer::Ray &ray, HitInfo &info) override = 0;
        virtual Math::Vector3D getNormalAtPoint(const Math::Point3D &point) const override = 0;
        virtual void translate(const Math::Vector3D &translation) override = 0;;
        void setColor(Raytracer::Color color) { _color = color; }
        Raytracer::Color getColor() const { return _color; }
        Math::Point3D getPosition() const { return _position; }
        Math::Vector3D getNormal() const { return Math::Vector3D(); }
        virtual Raytracer::AABB boundingBox() const = 0;
        void addMaterial(std::shared_ptr<Material> material) {
            _materials.push_back(material);
        }
        std::vector<std::shared_ptr<Material>> getMaterials() {
            return _materials;
        }
        std::string getName() {
            return _name;
        }
        void rotate(float xDegrees, float yDegrees, float zDegrees) override;

    protected:
        std::vector<std::shared_ptr<Material>> _materials;
        Math::Point3D _position;
        Math::Vector3D _normal;
        Raytracer::Color _color;
        std::string _name;
        Math::Matrix4x4 _invRotation;
        Math::Matrix4x4 _rotation;
};
