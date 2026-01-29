/*
** EPITECH PROJECT, 2025
** B-OOP-400-TLS-4-1-raytracer-antonin.veau
** File description:
** parsing
*/

#pragma once
#include "Raytracer/Camera.hpp"
#include "Raytracer/Scene.hpp"
#include "Primitives/Plane.hpp"
#include "Primitives/OBJ.hpp"
#include "Raytracer/SceneBuilder.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>

int parsingfile(char *file, Raytracer::Scene &scene);