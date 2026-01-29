# Raytracer Project

This is a project made during my 2nd year at Epitech,
the goal of the project was to make a simple raytracer in cpp with limited external library (standard lib and SFML only).

This project allow you to create scene file and visualize them either by outputing a image file (PPM format) or in real time with our SFML visualizer.

it even go beyond the initial requirement and allow you to load simple OBJ file (material not supported).

The program will simulate how light will interact with object in the environment and render realistic visuals

## Technical Implementation
- **Primitives** Support sphere, plane, triangle, cylinder and cone
- **Optimizations** Multithreading, BVH,
- **Parsing** Custom parsing for simple OBJ and scene file

## How to Build

To clone the repository and build the project, follow these steps:

1. Clone the repository:
    ```bash
    git clone https://github.com/username/raytracer.git
    cd raytracer
    ```

2. Build the project using `make`:
    ```bash
    make
    ```

## How to Execute

To run the raytracer, use the following command:

```bash
./raytracer [-rt] filename
```

- `-rt`: Optional flag to enable real-time rendering mode.
- `filename`: Path to the scene description file.

## How to View Documentation

To view the project documentation, use the following script:

```bash
./Doc_bash.sh
```

This script will display the documentation in a user-friendly format. Make sure the script has execution permissions before running it:

```bash
chmod +x Doc_bash.sh
```

## Controls

Here are the controls for navigating and interacting with the raytracer:

- **Z** - Move forward
- **Q** - Move left
- **D** - Move right
- **S** - Move backward
- **A** - Move down
- **E** - Move up
- **R** - Select up
- **F** - Select down
- **Enter** - Confirm selection
- **Up Arrow** - Look up
- **Down Arrow** - Look down
- **Right Arrow** - Look right
- **Left Arrow** - Look left
- **Space** - Toggle focus mode
- **P** - Increase camera speed
- **M** - Decrease camera speed

Enjoy exploring and rendering your 3D scenes!
