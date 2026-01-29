#include "SceneBuilder.hpp"

namespace Raytracer {

    SceneBuilder::SceneBuilder(){
        reset();
    }

    void SceneBuilder::reset() {
        _scene = std::make_unique<Scene>();
    }

    SceneBuilder& SceneBuilder::buildCamera(Math::Point3D origin, Math::Rectangle3D screen, Resolution resolution, bool antialiasing) {
        Camera camera(origin, screen);
        camera.setResolution(resolution.width, resolution.width);
        camera.antialiasing = antialiasing;
        _scene->setNewCamera(camera);
        return *this;
    }

    SceneBuilder& SceneBuilder::buildSphere(Math::Point3D center, double radius, Color color, std::shared_ptr<Material> material) {
        Primitive::Sphere *sphere = new Primitive::Sphere(center, radius);
        sphere->setColor(color);
        if (material) {
            sphere->addMaterial(material);
        }
        _scene->addPrimitive(sphere);
        return *this;
    }

    SceneBuilder &SceneBuilder::buildPlane(Math::Point3D point, Math::Vector3D normal, Color color, std::shared_ptr<Material> material)
    {
        Primitive::Plane *plane = new Primitive::Plane(point, normal);
        plane->setColor(color);
        if (material) {
            plane->addMaterial(material);
        }
        _scene->addPrimitive(plane);
        return *this;
    }

    SceneBuilder &SceneBuilder::buildCylinder(Math::Point3D center, double radius, double height, Color color, std::shared_ptr<Material> material)
    {
        Primitive::Cylinder *cylindre = new Primitive::Cylinder(center, radius, height);
        cylindre->setColor(color);
        if (material) {
            cylindre->addMaterial(material);
        }
        _scene->addPrimitive(cylindre);
        return *this;
    }

    SceneBuilder &SceneBuilder::buildTriangle(Math::Point3D v0, Math::Point3D v1, Math::Point3D v2, Color color, std::shared_ptr<Material> material)
    {
        Primitive::Triangle *triangle = new Primitive::Triangle(v0, v1, v2);
        triangle->setColor(color);
        if (material) {
            triangle->addMaterial(material);
        }
        _scene->addPrimitive(triangle);
        return *this;
    }

    SceneBuilder &SceneBuilder::buildObject(std::string filename, Math::Point3D origin, Color color, std::shared_ptr<Material> material)
    {
        Primitive::OBJ *obj = new Primitive::OBJ(filename, origin);
        obj->setColor(color);
        if (material) {
            obj->addMaterial(material);
        }
        _scene->addPrimitive(obj);
        return *this;
    }

    SceneBuilder &SceneBuilder::buildCone(Math::Point3D& baseCenter, Math::Vector3D& axis, float height, float baseRadius, Color color, std::shared_ptr<Material> material)
    {
        Primitive::Cone *cone = new Primitive::Cone(baseCenter, axis, height, baseRadius);
        cone->setColor(color);
        if (material) {
            cone->addMaterial(material);
        }
        _scene->addPrimitive(cone);
        return *this;
    }

    SceneBuilder &SceneBuilder::buildAmbientLight(const Math::Point3D &position, double intensity, const Raytracer::Color &color)
    {
        GlobalLight *light = new GlobalLight(position, intensity, color);
        _scene->addLight(light);
        return *this;
    }

    SceneBuilder &SceneBuilder::buildDirectionalLight(const Math::Vector3D &direction, double intensity, const Raytracer::Color &color)
    {
        DirectionalLight *light = new DirectionalLight(direction, intensity, color);
        _scene->addLight(light);
        return *this;
    }

    SceneBuilder &SceneBuilder::buildPointLight(const Math::Point3D &position, double intensity, const Raytracer::Color &color)
    {
        PointLight *light = new PointLight(position, intensity, color);
        _scene->addLight(light);
        return *this;
    }

    std::unique_ptr<Scene> SceneBuilder::getScene() {
        std::unique_ptr<Scene> scene = std::move(_scene);
        _scene = std::make_unique<Scene>();

        return scene;
    }

}