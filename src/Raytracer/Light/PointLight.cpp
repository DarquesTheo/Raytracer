#include "PointLight.hpp"

namespace Raytracer {
    PointLight::PointLight(const Math::Point3D &position, double intensity, const Raytracer::Color &color)
        : ALight(position, intensity, color) {}

    Raytracer::Color PointLight::computeLighting(
        const Math::Point3D &intersectionPoint,
        const Math::Vector3D &normal,
        const BVHNode *spacePartition,
        const IPrimitives *currentPrimitive
    ) const {
        const double epsilon = 1e-6;
        Math::Vector3D toLight(intersectionPoint, getPosition());
        double distance = toLight.getLength();
        Math::Vector3D lightDir = toLight;
        lightDir.normalize();
        Math::Point3D shadowOrigin = intersectionPoint + normal * epsilon;
        Raytracer::Ray shadowRay(shadowOrigin, lightDir);
        HitInfo info;
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
                    if (primitive == currentPrimitive)
                        continue;
                    if (primitive->hit(shadowRay, info) && info.t > epsilon) {
                        bool isTransparent = false;

                        for (const auto &material : primitive->getMaterials()) {
                            if (material->getType() == TRANSPARENCY) {
                                double transparency = material->getPercentage();
                                accumulatedColor = accumulatedColor * (1.0 - transparency);
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
        double k_c = 1.0, k_l = 0.1, k_q = 0.01;
        double attenuation = 1.0 / (k_c + k_l * distance + k_q * distance * distance);
        double intensity = std::max(0.0, normal.dot(lightDir)) * getIntensity() * attenuation;
        return accumulatedColor * intensity * currentPrimitive->getColor();
    }
}