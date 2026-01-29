/*
** EPITECH PROJECT, 2025
** B-OOP-400-TLS-4-1-raytracer-antonin.veau
** File description:
** paring
*/

#include "parsing.hpp"

using ParseFunc = std::function<int(std::ifstream&, Raytracer::SceneBuilder&)>;

extern std::unordered_map<std::string, ParseFunc> parserMap;

int parse_camera(std::ifstream &file, Raytracer::SceneBuilder &builder)
{
    std::string line;
    Math::Point3D position;
    int width = 0, height = 0;
    bool antialiasing = false;
    Raytracer::Resolution resolution;
    Math::Point3D Pos;
    Math::Vector3D Vec1;
    Math::Vector3D Vec2;

    bool hasPosition = false, hasScreen = false, hasResolution = false, hasAntialiasing = false;

    while (std::getline(file, line) && parserMap.find(line) == parserMap.end()) {
        std::istringstream iss(line);
        std::string label;
        if (!(iss >> label))
            continue;

        if (label == "position") {
            if (!(iss >> position.x >> position.y >> position.z)) {
                std::cerr << "Error: Invalid position format for CAMERA" << std::endl;
                return 1;
            }
            hasPosition = true;
        } else if (label == "Screen") {
            if (!(iss >> Pos.x >> Pos.y >> Pos.z)) {
                std::cerr << "Error: Invalid screen position format for CAMERA" << std::endl;
                return 1;
            }
            iss.ignore(1, ',');
            if (!(iss >> Vec1.x >> Vec1.y >> Vec1.z)) {
                std::cerr << "Error: Invalid screen vector 1 format for CAMERA" << std::endl;
                return 1;
            }
            iss.ignore(1, ',');
            if (!(iss >> Vec2.x >> Vec2.y >> Vec2.z)) {
                std::cerr << "Error: Invalid screen vector 2 format for CAMERA" << std::endl;
                return 1;
            }
            hasScreen = true;
        } else if (label == "resolution") {
            if (!(iss >> resolution.width >> resolution.height)) {
                std::cerr << "Error: Invalid resolution format for CAMERA" << std::endl;
                return 1;
            }
            hasResolution = true;
        } else if (label == "Antialiasing") {
            std::string value;
            if (!(iss >> value)) {
                std::cerr << "Error: Invalid antialiasing format for CAMERA" << std::endl;
                return 1;
            }
            antialiasing = (value == "True" || value == "true");
            hasAntialiasing = true;
        } else {
            std::cerr << "Unknown camera property: " << label << std::endl;
            return 1;
        }
    }

    if (!hasPosition || !hasScreen || !hasResolution || !hasAntialiasing) {
        std::cerr << "Error: Missing mandatory properties for CAMERA" << std::endl;
        return 1;
    }

    if (!line.empty() && parserMap.find(line) != parserMap.end())
        file.seekg(-((int)line.size()) - 1, std::ios_base::cur);

    Math::Rectangle3D screen(Pos, Vec1, Vec2);
    builder.buildCamera(position, screen, resolution, antialiasing);
    return 0;
}

std::shared_ptr<Material> getMaterialName(const std::string &name, double percentage)
{
    static std::unordered_map<std::string, std::shared_ptr<Material>> materialMap = {
        {"transparency", std::make_shared<Material>(TRANSPARENCY, percentage)},
        {"refraction", std::make_shared<Material>(REFRACTION, percentage)},
        {"reflection", std::make_shared<Material>(REFLECTION, percentage)}
        //{"NAME", TYPE(NAME, PERCENTAGE)}
    };

    auto it = materialMap.find(name);
    if (it != materialMap.end()) {
        return it->second;
    }
    return nullptr;
}

int parse_sphere(std::ifstream &file, Raytracer::SceneBuilder &builder)
{
    std::string line;
    Math::Point3D position;
    std::shared_ptr<Material> material;
    double radius = 0;
    int red = 0, green = 0, blue = 0;

    bool hasPosition = false, hasRadius = false, hasColor = false;

    while (std::getline(file, line) && parserMap.find(line) == parserMap.end()) {
        std::istringstream iss(line);
        std::string label;
        if (!(iss >> label))
            continue;

        if (label == "position") {
            if (!(iss >> position.x >> position.y >> position.z)) {
                std::cerr << "Error: Invalid position format for SPHERE" << std::endl;
                return 1;
            }
            hasPosition = true;
        } else if (label == "radius") {
            if (!(iss >> radius)) {
                std::cerr << "Error: Invalid radius format for SPHERE" << std::endl;
                return 1;
            }
            hasRadius = true;
        } else if (label == "color") {
            if (!(iss >> red >> green >> blue)) {
                std::cerr << "Error: Invalid color format for SPHERE" << std::endl;
                return 1;
            }
            hasColor = true;
        } else if (label == "material") {
            std::string materialName;
            double percentage;
            iss >> materialName >> percentage;
            material = getMaterialName(materialName, percentage);
            if (!material) {
                std::cerr << "Error: Unknown material '" << materialName << "' for SPHERE" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Unknown sphere property: " << label << std::endl;
            return 1;
        }
    }

    if (!hasPosition || !hasRadius || !hasColor) {
        std::cerr << "Error: Missing mandatory properties for SPHERE" << std::endl;
        return 1;
    }

    if (!line.empty() && parserMap.find(line) != parserMap.end())
        file.seekg(-((int)line.size()) - 1, std::ios_base::cur);

    Raytracer::Color color;
    color.setRed(red);
    color.setGreen(green);
    color.setBlue(blue);

    builder.buildSphere(position, radius, color, material);
    return 0;
}

int parse_plane(std::ifstream &file, Raytracer::SceneBuilder &builder)
{
    std::string line;
    Math::Point3D position;
    Math::Vector3D normal;
    std::shared_ptr<Material> material;
    int red = 0, green = 0, blue = 0;

    bool hasPosition = false, hasNormal = false, hasColor = false;

    while (std::getline(file, line) && parserMap.find(line) == parserMap.end()) {
        std::istringstream iss(line);
        std::string label;
        if (!(iss >> label))
            continue;

        if (label == "position") {
            if (!(iss >> position.x >> position.y >> position.z)) {
                std::cerr << "Error: Invalid position format for PLANE" << std::endl;
                return 1;
            }
            hasPosition = true;
        } else if (label == "normal") {
            if (!(iss >> normal.x >> normal.y >> normal.z)) {
                std::cerr << "Error: Invalid normal format for PLANE" << std::endl;
                return 1;
            }
            hasNormal = true;
        } else if (label == "color") {
            if (!(iss >> red >> green >> blue)) {
                std::cerr << "Error: Invalid color format for PLANE" << std::endl;
                return 1;
            }
            hasColor = true;
        } else if (label == "material") {
            std::string materialName;
            double percentage;
            iss >> materialName >> percentage;
            material = getMaterialName(materialName, percentage);
            if (!material) {
                std::cerr << "Error: Unknown material '" << materialName << "' for PLANE" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Unknown plane property: " << label << std::endl;
            return 1;
        }
    }

    if (!hasPosition || !hasNormal || !hasColor) {
        std::cerr << "Error: Missing mandatory properties for PLANE" << std::endl;
        return 1;
    }

    if (!line.empty() && parserMap.find(line) != parserMap.end())
        file.seekg(-((int)line.size()) - 1, std::ios_base::cur);

    Raytracer::Color color;
    color.setRed(red);
    color.setGreen(green);
    color.setBlue(blue);

    builder.buildPlane(position, normal, color, material);
    return 0;
}

int parse_cylinder(std::ifstream &file, Raytracer::SceneBuilder &builder)
{
    std::string line;
    Math::Point3D position;
    std::shared_ptr<Material> material;
    double radius = 0.0;
    double height = 0.0;
    int red = 0, green = 0, blue = 0;

    bool hasPosition = false, hasRadius = false, hasHeight = false, hasColor = false;

    while (std::getline(file, line) && parserMap.find(line) == parserMap.end()) {
        std::istringstream iss(line);
        std::string label;
        if (!(iss >> label))
            continue;

        if (label == "position") {
            if (!(iss >> position.x >> position.y >> position.z)) {
                std::cerr << "Error: Invalid position format for CYLINDER" << std::endl;
                return 1;
            }
            hasPosition = true;
        } else if (label == "radius") {
            if (!(iss >> radius)) {
                std::cerr << "Error: Invalid radius format for CYLINDER" << std::endl;
                return 1;
            }
            hasRadius = true;
        } else if (label == "height") {
            if (!(iss >> height)) {
                std::cerr << "Error: Invalid height format for CYLINDER" << std::endl;
                return 1;
            }
            hasHeight = true;
        } else if (label == "color") {
            if (!(iss >> red >> green >> blue)) {
                std::cerr << "Error: Invalid color format for CYLINDER" << std::endl;
                return 1;
            }
            hasColor = true;
        } else if (label == "material") {
            std::string materialName;
            double percentage;
            iss >> materialName >> percentage;
            material = getMaterialName(materialName, percentage);
            if (!material) {
                std::cerr << "Error: Unknown material '" << materialName << "' for CYLINDER" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Unknown cylinder property: " << label << std::endl;
            return 1;
        }
    }

    if (!hasPosition || !hasRadius || !hasHeight || !hasColor) {
        std::cerr << "Error: Missing mandatory properties for CYLINDER" << std::endl;
        return 1;
    }

    if (!line.empty() && parserMap.find(line) != parserMap.end())
        file.seekg(-((int)line.size()) - 1, std::ios_base::cur);

    Raytracer::Color color;
    color.setRed(red);
    color.setGreen(green);
    color.setBlue(blue);

    builder.buildCylinder(position, radius, height, color, material);
    return 0;
}

int parse_triangle(std::ifstream &file, Raytracer::SceneBuilder &builder)
{
    std::string line;
    Math::Point3D v0, v1, v2;
    std::shared_ptr<Material> material;
    int red = 0, green = 0, blue = 0;

    bool hasV0 = false, hasV1 = false, hasV2 = false, hasColor = false;

    while (std::getline(file, line) && parserMap.find(line) == parserMap.end()) {
        std::istringstream iss(line);
        std::string label;
        if (!(iss >> label))
            continue;

        if (label == "v0") {
            if (!(iss >> v0.x >> v0.y >> v0.z)) {
                std::cerr << "Error: Invalid v0 format for TRIANGLE" << std::endl;
                return 1;
            }
            hasV0 = true;
        } else if (label == "v1") {
            if (!(iss >> v1.x >> v1.y >> v1.z)) {
                std::cerr << "Error: Invalid v1 format for TRIANGLE" << std::endl;
                return 1;
            }
            hasV1 = true;
        } else if (label == "v2") {
            if (!(iss >> v2.x >> v2.y >> v2.z)) {
                std::cerr << "Error: Invalid v2 format for TRIANGLE" << std::endl;
                return 1;
            }
            hasV2 = true;
        } else if (label == "color") {
            if (!(iss >> red >> green >> blue)) {
                std::cerr << "Error: Invalid color format for TRIANGLE" << std::endl;
                return 1;
            }
            hasColor = true;
        } else if (label == "material") {
            std::string materialName;
            double percentage;
            iss >> materialName >> percentage;
            material = getMaterialName(materialName, percentage);
            if (!material) {
                std::cerr << "Error: Unknown material '" << materialName << "' for TRIANGLE" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Unknown triangle property: " << label << std::endl;
            return 1;
        }
    }

    if (!hasV0 || !hasV1 || !hasV2 || !hasColor) {
        std::cerr << "Error: Missing mandatory properties for TRIANGLE" << std::endl;
        return 1;
    }

    if (!line.empty() && parserMap.find(line) != parserMap.end())
        file.seekg(-((int)line.size()) - 1, std::ios_base::cur);

    Raytracer::Color color;
    color.setRed(red);
    color.setGreen(green);
    color.setBlue(blue);

    builder.buildTriangle(v0, v1, v2, color, material);
    return 0;
}

int parse_cone(std::ifstream &file, Raytracer::SceneBuilder &builder)
{
    std::string line;
    Math::Point3D baseCenter;
    Math::Vector3D axis;
    std::shared_ptr<Material> material;
    float height = 0.0f, baseRadius = 0.0f;
    int red = 0, green = 0, blue = 0;

    bool hasBaseCenter = false, hasAxis = false, hasHeight = false, hasBaseRadius = false, hasColor = false;

    while (std::getline(file, line) && parserMap.find(line) == parserMap.end()) {
        std::istringstream iss(line);
        std::string label;
        if (!(iss >> label))
            continue;

        if (label == "position") {
            if (!(iss >> baseCenter.x >> baseCenter.y >> baseCenter.z)) {
                std::cerr << "Error: Invalid baseCenter format for CONE" << std::endl;
                return 1;
            }
            hasBaseCenter = true;
        } else if (label == "axis") {
            if (!(iss >> axis.x >> axis.y >> axis.z)) {
                std::cerr << "Error: Invalid axis format for CONE" << std::endl;
                return 1;
            }
            hasAxis = true;
        } else if (label == "height") {
            if (!(iss >> height)) {
                std::cerr << "Error: Invalid height format for CONE" << std::endl;
                return 1;
            }
            hasHeight = true;
        } else if (label == "Radius") {
            if (!(iss >> baseRadius)) {
                std::cerr << "Error: Invalid baseRadius format for CONE" << std::endl;
                return 1;
            }
            hasBaseRadius = true;
        } else if (label == "color") {
            if (!(iss >> red >> green >> blue)) {
                std::cerr << "Error: Invalid color format for CONE" << std::endl;
                return 1;
            }
            hasColor = true;
        } else if (label == "material") {
            std::string materialName;
            double percentage;
            iss >> materialName >> percentage;
            material = getMaterialName(materialName, percentage);
            if (!material) {
                std::cerr << "Error: Unknown material '" << materialName << "' for CONE" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Unknown cone property: " << label << std::endl;
            return 1;
        }
    }

    if (!hasBaseCenter || !hasAxis || !hasHeight || !hasBaseRadius || !hasColor) {
        std::cerr << "Error: Missing mandatory properties for CONE" << std::endl;
        return 1;
    }

    if (!line.empty() && parserMap.find(line) != parserMap.end())
        file.seekg(-((int)line.size()) - 1, std::ios_base::cur);

    Raytracer::Color color;
    color.setRed(red);
    color.setGreen(green);
    color.setBlue(blue);

    builder.buildCone(baseCenter, axis, height, baseRadius, color, material);
    return 0;
}

int parse_obj(std::ifstream &file, Raytracer::SceneBuilder &builder)
{
    std::string line;
    std::string filename;
    Math::Point3D origin;
    std::shared_ptr<Material> material;
    int red = 0, green = 0, blue = 0;

    bool hasFilename = false, hasOrigin = false, hasColor = false;

    while (std::getline(file, line) && parserMap.find(line) == parserMap.end()) {
        std::istringstream iss(line);
        std::string label;
        if (!(iss >> label))
            continue;

        if (label == "filename") {
            if (!(iss >> filename)) {
                std::cerr << "Error: Invalid filename format for OBJ" << std::endl;
                return 1;
            }
            hasFilename = true;
        } else if (label == "position") {
            if (!(iss >> origin.x >> origin.y >> origin.z)) {
                std::cerr << "Error: Invalid origin format for OBJ" << std::endl;
                return 1;
            }
            hasOrigin = true;
        } else if (label == "color") {
            if (!(iss >> red >> green >> blue)) {
                std::cerr << "Error: Invalid color format for OBJ" << std::endl;
                return 1;
            }
            hasColor = true;
        } else if (label == "material") {
            std::string materialName;
            double percentage;
            iss >> materialName >> percentage;
            material = getMaterialName(materialName, percentage);
            if (!material) {
                std::cerr << "Error: Unknown material '" << materialName << "' for OBJ" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Unknown OBJ property: " << label << std::endl;
            return 1;
        }
    }

    if (!hasFilename || !hasOrigin || !hasColor) {
        std::cerr << "Error: Missing mandatory properties for OBJ" << std::endl;
        return 1;
    }

    if (!line.empty() && parserMap.find(line) != parserMap.end())
        file.seekg(-((int)line.size()) - 1, std::ios_base::cur);

    Raytracer::Color color;
    color.setRed(red);
    color.setGreen(green);
    color.setBlue(blue);

    builder.buildObject(filename, origin, color, material);
    return 0;
}

int parse_light_a(std::ifstream &file, Raytracer::SceneBuilder &builder)
{
    std::string line;
    Math::Point3D position;
    int red = 0, green = 0, blue = 0;
    double intensity = 0.0;

    bool hasPosition = false, hasIntensity = false, hasColor = false;

    while (std::getline(file, line) && parserMap.find(line) == parserMap.end()) {
        std::istringstream iss(line);
        std::string label;
        if (!(iss >> label))
            continue;

        if (label == "position") {
            if (!(iss >> position.x >> position.y >> position.z)) {
                std::cerr << "Error: Invalid position format for LIGHT_A" << std::endl;
                return 1;
            }
            hasPosition = true;
        } else if (label == "intensity") {
            if (!(iss >> intensity)) {
                std::cerr << "Error: Invalid intensity format for LIGHT_A" << std::endl;
                return 1;
            }
            hasIntensity = true;
        } else if (label == "color") {
            if (!(iss >> red >> green >> blue)) {
                std::cerr << "Error: Invalid color format for LIGHT_A" << std::endl;
                return 1;
            }
            hasColor = true;
        } else {
            std::cerr << "Unknown ambient light property: " << label << std::endl;
            return 1;
        }
    }

    if (!hasPosition || !hasIntensity || !hasColor) {
        std::cerr << "Error: Missing mandatory properties for LIGHT_A" << std::endl;
        return 1;
    }

    if (!line.empty() && parserMap.find(line) != parserMap.end())
        file.seekg(-((int)line.size()) - 1, std::ios_base::cur);

    Raytracer::Color color;
    color.setRed(red);
    color.setGreen(green);
    color.setBlue(blue);

    builder.buildAmbientLight(position, intensity, color);
    return 0;
}

int parse_light_d(std::ifstream &file, Raytracer::SceneBuilder &builder)
{
    std::string line;
    Math::Vector3D direction;
    int red = 0, green = 0, blue = 0;
    double intensity = 0.0;

    bool hasDirection = false, hasIntensity = false, hasColor = false;

    while (std::getline(file, line) && parserMap.find(line) == parserMap.end()) {
        std::istringstream iss(line);
        std::string label;
        if (!(iss >> label))
            continue;

        if (label == "direction") {
            if (!(iss >> direction.x >> direction.y >> direction.z)) {
                std::cerr << "Error: Invalid direction format for LIGHT_D" << std::endl;
                return 1;
            }
            hasDirection = true;
        } else if (label == "intensity") {
            if (!(iss >> intensity)) {
                std::cerr << "Error: Invalid intensity format for LIGHT_D" << std::endl;
                return 1;
            }
            hasIntensity = true;
        } else if (label == "color") {
            if (!(iss >> red >> green >> blue)) {
                std::cerr << "Error: Invalid color format for LIGHT_D" << std::endl;
                return 1;
            }
            hasColor = true;
        } else {
            std::cerr << "Unknown directional light property: " << label << std::endl;
            return 1;
        }
    }

    if (!hasDirection || !hasIntensity || !hasColor) {
        std::cerr << "Error: Missing mandatory properties for LIGHT_D" << std::endl;
        return 1;
    }

    if (!line.empty() && parserMap.find(line) != parserMap.end())
        file.seekg(-((int)line.size()) - 1, std::ios_base::cur);

    Raytracer::Color color;
    color.setRed(red);
    color.setGreen(green);
    color.setBlue(blue);

    builder.buildDirectionalLight(direction, intensity, color);
    return 0;
}

int parse_light_p(std::ifstream &file, Raytracer::SceneBuilder &builder)
{
    std::string line;
    Math::Point3D position;
    int red = 0, green = 0, blue = 0;
    double intensity = 0.0;

    bool hasPosition = false, hasIntensity = false, hasColor = false;

    while (std::getline(file, line) && parserMap.find(line) == parserMap.end()) {
        std::istringstream iss(line);
        std::string label;
        if (!(iss >> label))
            continue;

        if (label == "position") {
            if (!(iss >> position.x >> position.y >> position.z)) {
                std::cerr << "Error: Invalid position format for LIGHT_P" << std::endl;
                return 1;
            }
            hasPosition = true;
        } else if (label == "intensity") {
            if (!(iss >> intensity)) {
                std::cerr << "Error: Invalid intensity format for LIGHT_P" << std::endl;
                return 1;
            }
            hasIntensity = true;
        } else if (label == "color") {
            if (!(iss >> red >> green >> blue)) {
                std::cerr << "Error: Invalid color format for LIGHT_P" << std::endl;
                return 1;
            }
            hasColor = true;
        } else {
            std::cerr << "Unknown point light property: " << label << std::endl;
            return 1;
        }
    }

    if (!hasPosition || !hasIntensity || !hasColor) {
        std::cerr << "Error: Missing mandatory properties for LIGHT_P" << std::endl;
        return 1;
    }

    if (!line.empty() && parserMap.find(line) != parserMap.end())
        file.seekg(-((int)line.size()) - 1, std::ios_base::cur);

    Raytracer::Color color;
    color.setRed(red);
    color.setGreen(green);
    color.setBlue(blue);

    builder.buildPointLight(position, intensity, color);
    return 0;
}

std::unordered_map<std::string, ParseFunc> parserMap = {
    { "CAMERA", parse_camera },
    { "SPHERE", parse_sphere },
    { "PLANE", parse_plane },
    { "CYLINDER", parse_cylinder },
    { "TRIANGLE", parse_triangle },
    { "CONE", parse_cone },
    { "OBJECT", parse_obj },
    { "LIGHT_A", parse_light_a },
    { "LIGHT_D", parse_light_d },
    { "LIGHT_P", parse_light_p }
    //{ "NAME", Func }
};

int parsingfile(char *file, Raytracer::Scene &scene)
{
    std::ifstream configFile(file);
    if (!configFile.is_open()) {
        std::cerr << "Error: cannot open file " << file << std::endl;
        return 1;
    }

    Raytracer::SceneBuilder builder;
    std::string line;
    while (std::getline(configFile, line)) {
        if (line.empty()) continue;

        auto it = parserMap.find(line);
        if (it == parserMap.end()) {
            std::cerr << "Unknown section: " << line << std::endl;
            return 1;
        }

        if (it->second(configFile, builder) != 0) {
            std::cerr << "Error while parsing section: " << line << std::endl;
            return 1;
        }
    }
    scene = *builder.getScene();
    return 0;
}