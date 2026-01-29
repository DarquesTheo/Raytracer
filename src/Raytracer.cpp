#include "Raytracer.hpp"

rtRaytracer::rtRaytracer(Raytracer::Scene &scene) :
    _scene(scene), _camera(scene.getCamera())
{
    _resolution = _camera.getResolution();
    cameraSpeed = 1.0;
    cameraRotationSpeed = 5.0;
    _texture.create(_resolution.width, _resolution.height);
    _sprite.setTexture(_texture);
    _font.loadFromFile("res/font.ttf");
    _text.setFont(_font);
    _text.setCharacterSize(10);
    numOfPrimitives = _scene.getPrimitives().size();
}

rtRaytracer::~rtRaytracer()
{
}

bool rtRaytracer::cameraRotation(sf::Event event, Math::Vector3D &rot)
{
    Math::Vector3D rotation(0, 0, 0);
    bool rotated = false;
    if (event.key.code == sf::Keyboard::Right) {
        rotation = _camera.getScreen().left_side.normalized() * cameraRotationSpeed;
        rotated = true;
    }
    if (event.key.code == sf::Keyboard::Left) {
        rotation = _camera.getScreen().left_side.normalized() * -cameraRotationSpeed;
        rotated = true;
    }

    if (event.key.code == sf::Keyboard::Up) {
        rotation = _camera.getScreen().bottom_side.normalized() * cameraRotationSpeed;
        rotated = true;
    }
    if (event.key.code == sf::Keyboard::Down) {
        rotation = _camera.getScreen().bottom_side.normalized() * -cameraRotationSpeed;
        rotated = true;
    }
    if (event.key.code == sf::Keyboard::Space) {
        lockView = (lockView == true) ? false : true;
        return true;
    }

    if (rotated) {
        rot = rotation;
        return true;
    }
    return false;
}

bool rtRaytracer::cameraMovement(sf::Event event, Math::Vector3D &dir)
{
    Math::Vector3D direction(0, 0, 0);
    bool moved = false;
    if (event.key.code == sf::Keyboard::P) {
        cameraSpeed += 0.1;
    }
    if (event.key.code == sf::Keyboard::M) {
        cameraSpeed -= 0.1;
    }
    if (event.key.code == sf::Keyboard::D) {
        direction = direction + (_camera.getScreen().bottom_side * 0.5) * cameraSpeed;
        moved = true;
    }
    if (event.key.code == sf::Keyboard::Q) {
        direction = direction - (_camera.getScreen().bottom_side * 0.5) * cameraSpeed;
        moved = true;
    }
    if (event.key.code == sf::Keyboard::E) {
        direction = direction + (_camera.getScreen().left_side * 0.5) * cameraSpeed;
        moved = true;
    }
    if (event.key.code == sf::Keyboard::A) {
        direction = direction - (_camera.getScreen().left_side * 0.5) * cameraSpeed;
        moved = true;
    }
    Math::Vector3D forward = _camera.forwardDirection();
    if (event.key.code == sf::Keyboard::Z) {
        direction = direction + forward * cameraSpeed;
        moved = true;
    }
    if (event.key.code == sf::Keyboard::S) {
        direction = direction - forward * cameraSpeed;
        moved = true;
    }
    if (moved) {
        dir = direction;
        return true;
    }
    return false;
}

void rtRaytracer::handleSelection(sf::Event event)
{
    if (event.key.code == sf::Keyboard::R && !confirmSelect) {
        selected--;
        if (selected < 0)
            selected = numOfPrimitives - 1;
    }
    if (event.key.code == sf::Keyboard::F && !confirmSelect) {
        selected++;
        if (selected >= numOfPrimitives)
            selected = 0;
    }
    if (event.key.code == sf::Keyboard::Enter) {
        confirmSelect = (confirmSelect == true) ? false : true;

    }
}

bool rtRaytracer::pollEvent()
{
    sf::Event event;
    bool exit = false;
    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window->close();
            return false;
        }
        if (event.type == sf::Event::KeyPressed) {
            Math::Vector3D direction(0, 0, 0);
            Math::Vector3D rot(0, 0, 0);
            bool movement = cameraMovement(event, direction);
            bool rotation = cameraRotation(event, rot);
            handleSelection(event);
            if (movement) {
                if (!confirmSelect)
                    _camera.translate(direction);
                else
                    _scene.getPrimitives()[selected]->translate(direction);
                exit = true;
            }
            if (rotation) {
                if (!confirmSelect)
                    _camera.rotate(rot.x, rot.y, rot.z);
                exit = true;
            }
            if (lockView) {
                _camera.lookAt(_scene.getPrimitives()[selected]->getPosition());
                exit = true;
            }
            if (exit)
                return exit;
        }
    }
    return false;
}

void rtRaytracer::writePrimitiveData()
{
    int i = 0;
    Math::Point3D pos;

    std::string cameraInfo = "Camera(" + std::to_string(_camera.origin.x) + ", " + std::to_string(_camera.origin.y) + ", " + std::to_string(_camera.origin.z) + ")";
    cameraInfo += " Speed : " + std::to_string(cameraSpeed);
    _text.setFillColor(sf::Color(0, 0, 0));
    _text.setString(cameraInfo);
    _text.setPosition(sf::Vector2f(2, 0));
    _window->draw(_text);

    for (IPrimitives *primitive : _scene.getPrimitives())
    {
        pos = primitive->getPosition();
        std::string primitiveInfo = primitive->getName() + "(" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + ")";
        _text.setFillColor(sf::Color(0, 0, 0));
        if (i == selected) {
            primitiveInfo = "-> " + primitiveInfo;
            if (confirmSelect)
                _text.setFillColor(sf::Color(255, 255, 0));
        }
        _text.setString(primitiveInfo);
        _text.setPosition(sf::Vector2f(2, 15 * (i + 1)));
        _window->draw(_text);
        i++;
    }
}

void rtRaytracer::startRealTime()
{
    _window = new sf::RenderWindow(sf::VideoMode(_resolution.width, _resolution.height), "Raytracer");
    bool changed = true;

    while (_window->isOpen()) {
        changed = (pollEvent() == true) ? true : changed;
        if (clock.getElapsedTime().asSeconds() > 0.02) { // 50fps limit
            if (changed) {
                std::vector<uint8_t> pixels = _scene.computeTextureData();
                _texture.update(pixels.data());
                changed = false;
            }

            _window->clear();
            _window->draw(_sprite);
            writePrimitiveData();
            _window->display();
            clock.restart();
        }
    }
}
