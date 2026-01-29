#include "DirectionalLight.hpp"

namespace Raytracer {
    DirectionalLight::DirectionalLight(const Math::Vector3D &direction, double intensity, const Raytracer::Color &color)
        : ALight(Math::Point3D(0, 0, 0), intensity, color), _direction(direction) {
        _direction.normalize();
    }

    Raytracer::Color DirectionalLight::computeLighting(
        const Math::Point3D &intersectionPoint,
        const Math::Vector3D &normal,
        const BVHNode *spacePartition,
        const IPrimitives *currentPrimitive
    ) const {
        const double epsilon = 1e-6;
        HitInfo info;

        Math::Vector3D lightDir = _direction * -1;
        lightDir.normalize();

        double intensity = std::max(0.0, lightDir.dot(normal)) * getIntensity();
        Math::Point3D shadowOrigin = lightDir * epsilon + intersectionPoint;
        Raytracer::Ray shadowRay(shadowOrigin, lightDir);

        const int maxDepth = 64;
        const BVHNode *stack[maxDepth];
        int top = 0;

        stack[top++] = spacePartition;

        Raytracer::Color accumulatedColor = getColor();

        while (top > 0) {
            const BVHNode *node = stack[--top];
            if (!node->bbox.hit(shadowRay)) {
                continue;
            }
            if (node->isLeaf) {
                for (auto primitive : node->content) {
                    if (primitive == currentPrimitive) {
                        continue;
                    }

                    if (primitive->hit(shadowRay, info) && info.t > epsilon) {
                        bool isTransparent = false;

                        for (const auto &material : primitive->getMaterials()) {
                            if (material->getType() == TRANSPARENCY) {
                                double transparency = material->getPercentage();
                                accumulatedColor = accumulatedColor * transparency;
                                isTransparent = true;
                                break;
                            }
                        }

                        if (!isTransparent) {
                            return Raytracer::Color(0, 0, 0);
                        }
                    }
                }
            } else {
                if (node->left)
                    stack[top++] = node->left;
                if (node->right)
                    stack[top++] = node->right;
            }
            if (top >= maxDepth)
                break;
        }
        return accumulatedColor * intensity * currentPrimitive->getColor();
    }
}
