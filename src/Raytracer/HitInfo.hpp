#pragma once

#include "../Math/Point3D.hpp"
#include "../Math/Vector3D.hpp"
#include "../Rendering/Material.hpp"

#include <memory>
#include <vector>

class HitInfo
{
public:
    Math::Point3D p;
    Math::Vector3D normal;
    double t;
    std::vector<std::shared_ptr<Material>> materials;
};