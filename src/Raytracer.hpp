#pragma once

#include "Raytracer/Scene.hpp"
#include "Primitives/Plane.hpp"
#include "Primitives/Cylinder.hpp"
#include "Raytracer/Light/PointLight.hpp"
#include "Raytracer/Light/GlobalLight.hpp"
#include "Raytracer/Light/DirectionalLight.hpp"
#include "Raytracer/Camera.hpp"

#include <iostream>

class rtRaytracer {
public:
    rtRaytracer(Raytracer::Scene &scene);
    ~rtRaytracer();
    void startRealTime();
    Raytracer::Scene &_scene;
private:

    bool pollEvent();
    void writePrimitiveData();
    bool cameraMovement(sf::Event event, Math::Vector3D &dir);
    bool cameraRotation(sf::Event event, Math::Vector3D &rot);
    void handleSelection(sf::Event event);
    Raytracer::Camera &_camera;
    Raytracer::Resolution _resolution;
    sf::RenderWindow *_window;
    sf::Texture _texture;
    sf::Sprite _sprite;
    sf::Font _font;
    sf::Clock clock;
    sf::Text _text;
    int selected = 0;
    bool confirmSelect = false;
    bool lockView = false;
    int numOfPrimitives;
    double cameraSpeed;
    double cameraRotationSpeed;
};