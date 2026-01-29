/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** APrimitives
*/

#include "APrimitives.hpp"

APrimitives::APrimitives(): _position(Math::Point3D()), _color(Raytracer::Color(0, 0, 0)), _normal(Math::Vector3D())
{
    addMaterial(std::make_shared<Material>(FLAT, 1.0));
}

void APrimitives::rotate(float xDeg, float yDeg, float zDeg) {
    Math::Matrix4x4 toOrigin = Math::Matrix4x4::translate(-_position.toVector());
    
    Math::Matrix4x4 rotX = Math::Matrix4x4::rotation(Math::Vector3D(1,0,0), xDeg);
    Math::Matrix4x4 rotY = Math::Matrix4x4::rotation(Math::Vector3D(0,1,0), yDeg);
    Math::Matrix4x4 rotZ = Math::Matrix4x4::rotation(Math::Vector3D(0,0,1), zDeg);
    
    Math::Matrix4x4 fromOrigin = Math::Matrix4x4::translate(_position.toVector());
    
    _rotation = fromOrigin * rotZ * rotY * rotX * toOrigin;
    _invRotation = _rotation.inverse();
}