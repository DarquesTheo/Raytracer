#include "APrimitives.hpp"
#include "../Math/Point3D.hpp"
#include "../Raytracer/Ray.hpp"
#include "../Math/Vector3D.hpp"
#include "../Math/Rectangle3D.hpp"
#include "Triangle.hpp"
#include "../Raytracer/BVH/BVH.hpp"

#pragma once

namespace Primitive {

    class OBJ : public APrimitives {
    public:
        OBJ(std::string filename, Math::Point3D origin);
        ~OBJ();

        bool hit(Raytracer::Ray &ray, HitInfo &info) override;
        void translate(const Math::Vector3D &translation) override;
        Math::Vector3D getNormalAtPoint(const Math::Point3D &point) const;

        Math::Point3D getPoint() const { return _origin; }
        Math::Vector3D getNormal() const { return _normal; }
        bool isPlane() const override { return false; }
        Raytracer::AABB boundingBox() const override;

    private:
        void parse(const std::string &file);
        Raytracer::BVHNode *buildSpacePartitioning(int start, int end);
        std::vector<IPrimitives*> Primitives;
        Math::Point3D _origin;
        Raytracer::AABB bbox;
        Raytracer::BVHNode *trianglePartition;
    };

}