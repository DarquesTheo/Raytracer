#pragma once

#include "Camera.hpp"
#include "../Primitives/Sphere.hpp"
#include "../Math/Rectangle3D.hpp"
#include "../Math/Vector3D.hpp"
#include "Light/ILight.hpp"
#include "Color/Color.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace Raytracer {
    class Scene {
    public:
        Scene();
        ~Scene();
        void setNewCamera(Camera newCamera);
        Scene& addPrimitive(IPrimitives *primitive);
        Scene& addLight(ILight *light);
        BVHNode *buildSpacePartitioning(int start, int end);
        Camera &getCamera() { return camera; }

        Color getPixelColor(Ray ray, int depth);
        Color getSampledPixelColor(int x, int y, int depth);
        std::ostringstream computeRow(int startingRow, int numRow);
        void computeRowPixels(int startingRow, int numRow, uint8_t *pixels);
        void fastComputeImage();
        std::vector<uint8_t> computeTextureData();
        std::vector<IPrimitives*> getPrimitives();
        void destroyContent();
    private:
        Camera camera;
        std::vector<IPrimitives*> Primitives;
        std::vector<IPrimitives*> sortedPrimitives;
        std::vector<ILight *> Lights;
        BVHNode *spacePartition;
    };
}