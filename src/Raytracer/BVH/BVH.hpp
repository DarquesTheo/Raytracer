#pragma once

#include <memory>
#include <vector>
#include <cmath>

#include "../../Primitives/IPrimitives.hpp"
#include "../../Math/Point3D.hpp"
#include "../HitInfo.hpp"

namespace Raytracer {
    class AABB {
    public:
        Math::Point3D pt1;
        Math::Point3D pt2;

        AABB() = default;
        AABB(Math::Point3D pt1, Math::Point3D pt2) : pt1(pt1), pt2(pt2) {};
        AABB(const AABB &box0, const AABB &box1);
        ~AABB() = default;
        
        AABB &setPoint(Math::Point3D pt1, Math::Point3D pt2);
        bool hit(const Ray &ray) const;
        Math::Point3D getCentoid();
    private:

    };

    class BVHNode {
    public:
        BVHNode(BVHNode *subNode1, BVHNode *subNode2);
        BVHNode(IPrimitives *primitive1, IPrimitives *primitive2);
        BVHNode(IPrimitives *primitive1);
        ~BVHNode();
        BVHNode *left;
        BVHNode *right;
        std::vector<IPrimitives*> content;
        bool isLeaf;
        bool hit(const Ray &ray, HitInfo &info);
        AABB bbox;
    private:
    };
}