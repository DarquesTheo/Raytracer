#include "BVH.hpp"

namespace Raytracer {
    AABB &AABB::setPoint(Math::Point3D pt1, Math::Point3D pt2)
    {
        this->pt1 = pt1;
        this->pt2 = pt2;
        return *this;
    }

    bool AABB::hit(const Ray &ray) const
    {
        double txmin = (pt1.x - ray.origin.x) / ray.direction.x;
        double txmax = (pt2.x - ray.origin.x) / ray.direction.x;
        if (txmin > txmax) std::swap(txmin, txmax);

        double tymin = (pt1.y - ray.origin.y) / ray.direction.y;
        double tymax = (pt2.y - ray.origin.y) / ray.direction.y;
        if (tymin > tymax) std::swap(tymin, tymax);

        double tzmin = (pt1.z - ray.origin.z) / ray.direction.z;
        double tzmax = (pt2.z - ray.origin.z) / ray.direction.z;
        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        double tmin = std::max({ txmin, tymin, tzmin });
        double tmax = std::min({ txmax, tymax, tzmax });

        return tmax >= tmin;
    }

    AABB::AABB(const AABB &box0, const AABB &box1)
    {
        pt1.x = std::min(box0.pt1.x, box1.pt1.x);
        pt1.y = std::min(box0.pt1.y, box1.pt1.y);
        pt1.z = std::min(box0.pt1.z, box1.pt1.z);

        pt2.x = std::max(box0.pt2.x, box1.pt2.x);
        pt2.y = std::max(box0.pt2.y, box1.pt2.y);
        pt2.z = std::max(box0.pt2.z, box1.pt2.z);
    }

    Math::Point3D AABB::getCentoid()
    {
        return Math::Point3D((pt1.x + pt2.x) * 0.5, (pt1.y + pt2.y) * 0.5, (pt1.z + pt2.z) * 0.5);
    }

    bool BVHNode::hit(const Ray &ray, HitInfo &info)
    {
        if (!bbox.hit(ray))
            return false;
        
        return true;
    }

    BVHNode::BVHNode(IPrimitives *primitive1, IPrimitives *primitive2)
    {
        left = nullptr;
        right = nullptr;
        content.push_back(primitive1);
        content.push_back(primitive2);
        bbox = AABB(primitive1->boundingBox(), primitive2->boundingBox());
        isLeaf = true;
    }

    BVHNode::BVHNode(IPrimitives *primitive1)
    {
        left = nullptr;
        right = nullptr;
        content.push_back(primitive1);
        bbox = primitive1->boundingBox();
        isLeaf = true;
    }

    BVHNode::BVHNode(BVHNode *subNode1, BVHNode *subNode2)
    {
        right = subNode1;
        left = subNode2;
        bbox = AABB(subNode1->bbox, subNode2->bbox);
        isLeaf = false;
    }

    BVHNode::~BVHNode()
    {
        delete(left);
        delete(right);
    }
}

