#include "OBJ.hpp"
#include <fstream>
#include <sstream>

namespace Primitive {

    void OBJ::translate(const Math::Vector3D &translation)
    {
        return;
    }

    OBJ::~OBJ()
    {
        for (IPrimitives *triangle : Primitives) {
            delete(triangle);
        }
        delete(trianglePartition);
    }

    Math::Vector3D OBJ::getNormalAtPoint(const Math::Point3D &point) const
    {
        return Math::Vector3D(0, 0, 0);
    }

    Raytracer::AABB OBJ::boundingBox() const
    {
        if (trianglePartition)
            return trianglePartition->bbox;
        else 
            return Raytracer::AABB(Math::Point3D(0, 0, 0), Math::Point3D(0, 0, 0));
    }

    OBJ::OBJ(std::string filename, Math::Point3D origin)
    {
        _origin = origin;
        parse(filename);
        _name = filename;
    }

    bool OBJ::hit(Raytracer::Ray &ray, HitInfo &info)
    {
        HitInfo closestInfo;

        double closestT = std::numeric_limits<double>::max();
        IPrimitives *closestPrimitive = nullptr;
        const int maxDepth = 64;
        const Raytracer::BVHNode *stack[maxDepth];
        int top = 0;
        
        stack[top++] = trianglePartition;
        while (top > 0) {
            const Raytracer::BVHNode *node = stack[--top];
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
        info = closestInfo;
        if (closestPrimitive)
            return true;
        return false;
    }

    void OBJ::parse(const std::string &filename)
    {
        std::ifstream inFile(filename);

        if (!inFile.is_open()) {
            std::cerr << "Error: Failed to open OBJ file: " << filename << " skipping primitive creation..." << std::endl;
            trianglePartition = nullptr;
            return;
        }

        std::string line;
        std::vector<Math::Point3D> vertices;

        while (std::getline(inFile, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "v") { //Vertex
                double x, y, z;
                iss >> x >> y >> z;
                vertices.emplace_back(x + _origin.x, y + _origin.y, z + _origin.z);
            } else if (type == "f") { //Face
                std::vector<int> faceIndices;

                std::string val;
                while (iss >> val) {
                    int vIdx = 0;
                    size_t firstSlash = val.find('/');
                    if (firstSlash == std::string::npos) {
                        vIdx = std::stoi(val);
                    } else {
                        vIdx = std::stoi(val.substr(0, firstSlash));
                    }

                    vIdx--;
                    if (vIdx >= 0 && vIdx < static_cast<int>(vertices.size()))
                        faceIndices.push_back(vIdx);
                    else
                        std::cerr << "Warning: Invalid vertex index in face: " << vIdx << std::endl;
                }

                // Triangulate if >= 3 vertices
                for (size_t i = 1; i + 1 < faceIndices.size(); ++i) {
                    int v0 = faceIndices[0];
                    int v1 = faceIndices[i];
                    int v2 = faceIndices[i + 1];
                    Primitives.push_back(new Triangle(vertices[v0], vertices[v1], vertices[v2]));
                }
            }
        }

        inFile.close();

        if (Primitives.empty()) {
             std::cerr << "Warning: No valid triangles found in OBJ file: " << filename << std::endl;
             trianglePartition = nullptr;
             return;
        }

        std::cout << "OBJ " << filename << " created with " << Primitives.size() << " triangles" << std::endl;

        trianglePartition = buildSpacePartitioning(0, Primitives.size());
    }

    Raytracer::BVHNode *OBJ::buildSpacePartitioning(int start, int end)
    {
        int size = end - start;
        if (size == 1) {
            return new Raytracer::BVHNode(Primitives.at(start));
        } else if (size == 2) {
            return new Raytracer::BVHNode(Primitives.at(start), Primitives.at(start + 1));
        }

        Raytracer::AABB sceneBounds = Primitives.at(start)->boundingBox();
        for (int i = start + 1; i < end; i++)
            sceneBounds = Raytracer::AABB(sceneBounds, Primitives.at(i)->boundingBox());
        
        Math::Vector3D sceneSize(sceneBounds.pt1, sceneBounds.pt2);

        int axis = 0; //axis: x
        if (sceneSize.y > sceneSize.x) axis = 1;
        if (sceneSize.z > sceneSize.y && sceneSize.z > sceneSize.x) axis = 2;

        std::sort(Primitives.begin() + start, Primitives.begin() + end,
            [axis](IPrimitives* a, IPrimitives* b) {
                return a->boundingBox().getCentoid()[axis] < b->boundingBox().getCentoid()[axis];
            });
        
        int mid = start + size / 2;
        Raytracer::BVHNode *left = buildSpacePartitioning(start, mid);
        Raytracer::BVHNode *right = buildSpacePartitioning(mid, end);
        
        return new Raytracer::BVHNode(right, left);
    }
}