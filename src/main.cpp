/*
** EPITECH PROJECT, 2025
** B-OOP-400-TLS-4-1-raytracer-antonin.veau
** File description:
** main
*/

#include "parsing.hpp"
#include "Raytracer.hpp"

void debug_test(Raytracer::Scene &scene, bool a)
{
    if (a) {
        printf("Camera Origin: (%f, %f, %f)\n", scene.getCamera().origin.x, scene.getCamera().origin.y, scene.getCamera().origin.z);
        printf("Camera Resolution: (%d, %d)\n", scene.getCamera().getResolution().width, scene.getCamera().getResolution().height);
        printf("Camera Screen: (%f, %f, %f), (%f,%f,%f), (%f,%f,%f)\n", scene.getCamera().getScreen().origin.x, scene.getCamera().getScreen().origin.y, scene.getCamera().getScreen().origin.z, scene.getCamera().getScreen().bottom_side.x, scene.getCamera().getScreen().bottom_side.y, scene.getCamera().getScreen().bottom_side.z, scene.getCamera().getScreen().left_side.x, scene.getCamera().getScreen().left_side.y, scene.getCamera().getScreen().left_side.z);
        printf("Camera Antialiasing: %d\n", scene.getCamera().antialiasing);
    }
}

void help_message()
{
    printf("USAGE: ./raytracer [-rt] <SCENE_FILE>\n SCENE_FILE: scene configuration\n");
}

int main(int ac, char **av)
{
    if (ac == 2 && strcmp(av[1], "-help") == 0) {
        help_message();
        return 0;
    }
    if (ac != 2 && ac != 3) {
        help_message();
        return 84;
    }
    bool realTime = false;
    if (ac == 3) {
        if (strcmp(av[1], "-rt") == 0)
            realTime = true;
        else {
            help_message();
            return 84;
        }
    }

    Raytracer::Scene scene;
    if (parsingfile(av[ac - 1], scene) == 1) {
        std::cerr << "Config file error stopping..." << std::endl;
        return 84;
    }
    if (realTime) {
        rtRaytracer raytracer(scene);
        raytracer.startRealTime();
    }else
        scene.fastComputeImage();
    scene.destroyContent();
    return 0;
}
