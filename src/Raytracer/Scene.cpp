#include "Scene.hpp"

#include <thread>
#include <sstream>
#include <fstream>
#include <chrono>
#include "../Primitives/Plane.hpp"

namespace Raytracer {

    Color getSkyColor(Ray &ray)
    {
        Math::Vector3D dir = ray.direction.normalized();
        double t = 0.5 * (dir.y + 1.0);
        Color base = Color(135, 206, 235) * t + Color(255, 255, 255) * (1 - t);

        Math::Vector3D sunDir = Math::Vector3D(0.0, 1.0, 1.0).normalized();
        double sunStrength = std::max(0.0, dir.dot(sunDir));
        sunStrength = pow(sunStrength, 512);

        base = base + Color(255, 240, 200) * sunStrength;
        return base;
    }

    Scene::Scene()
        : camera(
            Math::Point3D(0, 0, 5),
            Math::Rectangle3D(
                Math::Point3D(-1, 1, 4),
                Math::Vector3D(2, 0, 0),
                Math::Vector3D(0, -2, 0)))
    {
        spacePartition = nullptr;
    }

    Scene::~Scene()
    {
        //TODO delete everything
        if (spacePartition)
            delete(spacePartition);
    }

    void Scene::destroyContent()
    {
        for (IPrimitives *primitive : Primitives) {
            delete(primitive);
        }
        for (ILight *light : Lights) {
            delete (light);
        }
    }

    void Scene::setNewCamera(Camera newCamera)
    {
        camera = newCamera;
    }

    Scene& Scene::addPrimitive(IPrimitives *primitive)
    {
        Primitives.push_back(primitive);
        return *this;
    }

    Scene& Scene::addLight(ILight *light) {
        Lights.push_back(light);
        return *this;
    }

    Color Scene::getPixelColor(Ray ray, int depth)
    {
        if (depth <= 0) {
            return Color(0, 0, 0);
        }

        Color color(0, 0, 0);
        HitInfo info;
        HitInfo closestInfo;

        double closestT = std::numeric_limits<double>::max();
        IPrimitives *closestPrimitive = nullptr;

        const int maxDepth = 64;
        const BVHNode *stack[maxDepth];
        int top = 0;

        stack[top++] = spacePartition;

        while (top > 0) {
            const BVHNode *node = stack[--top];
            if (!node->bbox.hit(ray)) {
                continue;
            }
            if (node->isLeaf) {
                for (auto primitive : node->content) {
                    if (primitive->hit(ray, info)) {
                        if (info.t < closestT) {
                            closestT = info.t;
                            closestPrimitive = primitive;
                            closestInfo = info;
                        }
                    }
                }
            } else {
                if (node->left)
                    stack[top++] = node->left;
                if (node->right)
                    stack[top++] = node->right;
                if (!node->right && !node->left)
                    break;
            }
            if (top >= maxDepth)
                break;
        }

        if (closestPrimitive) {
            Math::Vector3D directionDotT = ray.direction * closestT;
            Math::Point3D intersectionPoint = directionDotT + ray.origin;
            Math::Vector3D normal = info.normal;
            if (normal.x == 0 && normal.y == 0 && normal.z == 0)
                normal = closestPrimitive->getNormalAtPoint(intersectionPoint);

            Color lightColor(0, 0, 0);
            for (auto *light : Lights) {
                Raytracer::Color lightContribution = light->computeLighting(intersectionPoint, normal, spacePartition, closestPrimitive);
                lightColor = lightColor + lightContribution;
            }

            Color accumulatedColor = lightColor;

            for (std::shared_ptr<Material> material : closestPrimitive->getMaterials()) {
                if (material) {
                    double percentage = material->getPercentage();

                    switch (material->getType()) {
                        case TRANSPARENCY:
                            if (material->scatter(ray)) {
                                Ray transparencyRay(intersectionPoint + ray.direction * 1e-6, ray.direction);
                                Color transparencyColor = getPixelColor(transparencyRay, depth - 1);
                                accumulatedColor = accumulatedColor * (1.0 - percentage) + transparencyColor * percentage;
                            }
                            break;

                        case REFRACTION:
                        if (material->scatter(ray)) {
                            double eta = material->getPercentage();
                            Math::Vector3D n = normal;
                            double cosI = -n.dot(ray.direction);
                            
                            if (cosI < 0) {
                                n = n * -1;
                                eta = 1.0 / eta;
                                cosI = -n.dot(ray.direction);
                            }
                    
                            double sinT2 = eta * eta * (1.0 - cosI * cosI);
                            if (sinT2 > 1.0) {
                                break;
                            }
                    
                            double cosT = sqrt(1.0 - sinT2);
                            Math::Vector3D refractedDir = ray.direction * eta + n * (eta * cosI - cosT);
                            Ray refractedRay(intersectionPoint + refractedDir * 1e-6, refractedDir);
                    
                            Color refractedColor = getPixelColor(refractedRay, depth - 1);
                            accumulatedColor = refractedColor;
                        }
                        break;

                        case REFLECTION:
                            if (material->scatter(ray)) {
                                Ray transparencyRay(intersectionPoint + ray.direction * 1e-6, ray.direction);
                                Math::Vector3D reflectedDir = ray.direction - normal * 2.0 * ray.direction.dot(normal);
                                reflectedDir.normalize();
                                Ray reflectedRay(intersectionPoint + normal * 1e-6, reflectedDir);
                                Color reflectionColor = getPixelColor(reflectedRay, depth - 1);
                                accumulatedColor = accumulatedColor * (1.0 - percentage) + reflectionColor * percentage;
                            }
                            break;
                    }
                }
            }

            color = accumulatedColor;
        }
        else {
            color = getSkyColor(ray);
        }

        return color;
    }

    Color Scene::getSampledPixelColor(int x, int y, int depth)
    {
        const int samplesPerAxis = camera.samplesPerAxis;
        const int totalSamples = samplesPerAxis * samplesPerAxis;
        int rSum = 0;
        int gSum = 0;
        int bSum = 0;

        double uStep = 1.0 / camera.getResolution().width;
        double vStep = 1.0 / camera.getResolution().height;
        std::vector<Color> samples;

        for (int sx = 0; sx < samplesPerAxis; ++sx) {
            for (int sy = 0; sy < samplesPerAxis; ++sy) {
                double offsetU = (sx + 0.5) / samplesPerAxis;
                double offsetV = (sy + 0.5) / samplesPerAxis;

                double u = (x + offsetU) * uStep;
                double v = (y + offsetV) * vStep;

                Ray ray = camera.ray(u, v);
                Color tmp = getPixelColor(ray, depth);
                samples.push_back(tmp);
                rSum += tmp.getRed();
                gSum += tmp.getGreen();
                bSum += tmp.getBlue();
            }
        }

        Color colorSum(rSum, gSum, bSum);
        Color colorAvg = colorSum / static_cast<double>(totalSamples);
        bool adaptSample = false;

        for (auto &c : samples) {
            double distR = c.getRed() - colorAvg.getRed();
            double distB = c.getBlue() - colorAvg.getBlue();
            double distG = c.getGreen() - colorAvg.getGreen();
            double dist = distR * distR + distG * distG + distB * distB;

            if (dist >= 225) {
                adaptSample = true;
                break;
            }
        }

        if (!adaptSample)
            return colorAvg;
        
        //Adaptative supersampling
        static const int extraSamples = 5;
        static const double offsets[5][2] = {{0.25, 0.25}, {0.75, 0.25}, {0.5, 0.5}, {0.25, 0.75}, {0.75, 0.75}};

        for (int i = 0; i < extraSamples; i++) {
            double u = (x + offsets[i][0]) * uStep;
            double v = (y + offsets[i][1]) * vStep;
            Ray ray = camera.ray(u, v);
            Color tmp = getPixelColor(ray, depth);
            rSum += tmp.getRed();
            gSum += tmp.getGreen();
            bSum += tmp.getBlue();
        }

        int total = totalSamples + extraSamples;
        return Color(rSum, gSum, bSum) / static_cast<double>(total);
    }

    std::ostringstream Scene::computeRow(int startingRow, int numRow)
    {
        double uStep = 1.0 / camera.getResolution().width;
        double vStep = 1.0 / camera.getResolution().height;

        std::ostringstream imgStream;

        const int samplesPerAxis = 2;
        const int totalSamples = samplesPerAxis * samplesPerAxis;

        for (int i = startingRow; i < startingRow + numRow; ++i) {
            for (int j = 0; j < camera.getResolution().width; ++j) {
                Color color;
                
                if (camera.antialiasing)
                    color = getSampledPixelColor(j, i, 3);
                else {
                    double u = j * uStep;
                    double v = i * vStep;

                    Ray ray = camera.ray(u, v);
                    color = getPixelColor(ray, 3);
                }
                color.writeColor(imgStream);
            }
        }
        return imgStream;
    }

    void Scene::computeRowPixels(int startingRow, int numRow, uint8_t *pixels)
    {
        double uStep = 1.0 / camera.getResolution().width;
        double vStep = 1.0 / camera.getResolution().height;

        const int samplesPerAxis = 2;
        const int totalSamples = samplesPerAxis * samplesPerAxis;

        for (int i = startingRow; i < startingRow + numRow; ++i) {
            for (int j = 0; j < camera.getResolution().width; ++j) {
                Color color;
                
                if (camera.antialiasing)
                    color = getSampledPixelColor(j, i, 3);
                else {
                    double u = j * uStep;
                    double v = i * vStep;

                    Ray ray = camera.ray(u, v);
                    color = getPixelColor(ray, 3);
                }
                int offset = (i * camera.getResolution().width + j) * 4;
                pixels[offset] = color.getRed();
                pixels[offset + 1] = color.getGreen();
                pixels[offset + 2] = color.getBlue();
                pixels[offset + 3] = 255;
            }
        }
    }

    void Scene::fastComputeImage()
    {
        int height = camera.getResolution().height;
        int width = camera.getResolution().width;

        sortedPrimitives = Primitives;
        spacePartition = buildSpacePartitioning(0, Primitives.size());

        int cores = std::thread::hardware_concurrency();
        if (cores == 0)
            cores = 1;

        double uStep = 1.0 / width;
        double vStep = 1.0 / height;

        std::vector<std::thread> threads;
        std::vector<std::ostringstream> img(cores);
        
        int rowsPerThread = height / cores;
        int remainingRows = height % cores;

        auto startTime = std::chrono::high_resolution_clock::now();

        int curRow = 0;
        for (int i = 0; i < cores; i++) {
            int rows = rowsPerThread + (i < remainingRows ? 1 : 0);
            int startingRow = curRow;
            threads.emplace_back([this, &img, i, startingRow, rows]() {
                img[i] = computeRow(startingRow, rows);
            });
            curRow += rows;
        }

        for (auto &thread : threads) {
            thread.join();
        }

        auto endRender = std::chrono::high_resolution_clock::now();

        std::ostringstream finalImage;
        for (const auto &row : img) {
            finalImage << row.str();
        }

        std::ofstream outFile("img.ppm", std::ios::binary);
        if (!outFile) {
            std::cerr << "Error: Cannot open output file." << std::endl;
            return;
        }

        outFile << "P6\n" << width << " " << height << "\n" << "255\n\n";
        outFile << finalImage.str();
        outFile.close();

        auto endWrite = std::chrono::high_resolution_clock::now();

        Raytracer::Resolution resolution = camera.getResolution();
        std::chrono::duration<double> elapsed = endRender - startTime;
        std::chrono::duration<double> elapsedWrite = endWrite - endRender;

        std::cout << "===== Rendering Stats =====" << std::endl << "Resolution:\t" << resolution.width << "x" << resolution.height << "\t(" << resolution.width * resolution.height << " pixels)" << std::endl;
        if (camera.antialiasing)
            std::cout << "Samples per pixel:\t 4 (9 on edge)" << std::endl;
        std::cout << "Total render time:\t" << elapsed.count() << " seconds (" << elapsed.count() / (resolution.width * resolution.height) << " seconds per pixel)"  << std::endl << "Total writing time:\t" << elapsedWrite.count() << " seconds" << std::endl;
        std::cout << "===========================" << std::endl;
    }

    std::vector<uint8_t> Scene::computeTextureData()
    {
        int height = camera.getResolution().height;
        int width = camera.getResolution().width;

        std::vector<uint8_t> pixels(width * height * 4, 0);

        sortedPrimitives = Primitives;
        spacePartition = buildSpacePartitioning(0, Primitives.size());

        int cores = std::thread::hardware_concurrency();
        if (cores == 0)
            cores = 1;

        double uStep = 1.0 / width;
        double vStep = 1.0 / height;

        std::vector<std::thread> threads;
        
        int rowsPerThread = height / cores;
        int remainingRows = height % cores;

        int curRow = 0;
        for (int i = 0; i < cores; i++) {
            int rows = rowsPerThread + (i < remainingRows ? 1 : 0);
            int startingRow = curRow;
            threads.emplace_back([this, i, startingRow, rows, &pixels]() {
                computeRowPixels(startingRow, rows, pixels.data());
            });
            curRow += rows;
        }

        for (auto &thread : threads) {
            thread.join();
        }

        return pixels;
    }

    BVHNode *Scene::buildSpacePartitioning(int start, int end)
    {
        int size = end - start;
        if (size == 1) {
            return new BVHNode(sortedPrimitives.at(start));
        } else if (size == 2) {
            return new BVHNode(sortedPrimitives.at(start), sortedPrimitives.at(start + 1));
        }

        AABB sceneBounds = sortedPrimitives.at(start)->boundingBox();
        for (int i = start + 1; i < end; i++)
            sceneBounds = AABB(sceneBounds, sortedPrimitives.at(i)->boundingBox());
        
        Math::Vector3D sceneSize(sceneBounds.pt1, sceneBounds.pt2);

        int axis = 0; //axis: x
        if (sceneSize.y > sceneSize.x) axis = 1;
        if (sceneSize.z > sceneSize.y && sceneSize.z > sceneSize.x) axis = 2;

        std::sort(sortedPrimitives.begin() + start, sortedPrimitives.begin() + end,
            [axis](IPrimitives* a, IPrimitives* b) {
                return a->boundingBox().getCentoid()[axis] < b->boundingBox().getCentoid()[axis];
            });
        
        int mid = start + size / 2;
        BVHNode *left = buildSpacePartitioning(start, mid);
        BVHNode *right = buildSpacePartitioning(mid, end);
        
        return new BVHNode(right, left);
    }

    std::vector<IPrimitives*> Scene::getPrimitives()
    {
        return Primitives;
    }
}