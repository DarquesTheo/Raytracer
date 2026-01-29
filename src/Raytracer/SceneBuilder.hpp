#pragma once

#include "Scene.hpp"
#include "Camera.hpp"
#include "../Primitives/Cylinder.hpp"
#include "../Primitives/Sphere.hpp"
#include "../Primitives/Plane.hpp"
#include "../Primitives/Triangle.hpp"
#include "../Primitives/Cone.hpp"
#include "../Primitives/OBJ.hpp"
#include "Light/DirectionalLight.hpp"
#include "Light/PointLight.hpp"
#include "Light/GlobalLight.hpp"
#include <memory>
#include <vector>

namespace Raytracer {

class SceneBuilder {
private:
    std::unique_ptr<Scene> _scene;

public:
    SceneBuilder();
    ~SceneBuilder() = default;
    
    // Camera setup
    SceneBuilder& buildCamera(Math::Point3D origin, Math::Rectangle3D screen, Resolution resolution, bool antialiasing);
    
    // Primitives setup
    SceneBuilder& buildSphere(Math::Point3D center, double radius, Color color, std::shared_ptr<Material> material);
    SceneBuilder& buildPlane(Math::Point3D point, Math::Vector3D normal, Color color, std::shared_ptr<Material> material);
    SceneBuilder& buildCylinder(Math::Point3D center, double radius, double height, Color color, std::shared_ptr<Material> material);
    SceneBuilder& buildTriangle(Math::Point3D v0, Math::Point3D v1, Math::Point3D v2, Color Color, std::shared_ptr<Material> material);
    SceneBuilder& buildObject(std::string filename, Math::Point3D origin, Color color, std::shared_ptr<Material> material);
    SceneBuilder& buildCone(Math::Point3D& baseCenter, Math::Vector3D& axis, float height, float baseRadius,  Color color, std::shared_ptr<Material> material);
    // Lights setup
    SceneBuilder& buildAmbientLight(const Math::Point3D &position, double intensity, const Raytracer::Color &color);
    SceneBuilder& buildDirectionalLight(const Math::Vector3D &direction, double intensity, const Raytracer::Color &color);
    SceneBuilder& buildPointLight(const Math::Point3D &position, double intensity, const Raytracer::Color &color);

    // Get the constructed scene
    std::unique_ptr<Scene> getScene() ;
    
    // Reset the builder to start creating a new scene
    void reset() ;
};

} // namespace RayTracer