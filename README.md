# NBodySimulator

<p align="center">
      <img src="https://user-images.githubusercontent.com/59691442/183268126-b3d19e66-8f2d-463a-805e-ae6ef7cc6c01.png" alt="cmakeLogo" style="height:60px;"/>
      <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="cppLogo" style="height:60px;"/>
      <img src="https://img.shields.io/badge/OpenGL-FFFFFF?style=for-the-badge&logo=opengl" alt="openglLogo" style="height:60px;"/>
      <img src="https://user-images.githubusercontent.com/59691442/190315147-ec9dc33f-0090-4f0d-98ab-514eb1463e01.png" alt="glfwLogo" style="height:60px;"/>
</p>

## Description

This is a simple n-body simulator using bruteforce made with OpenGL in C++ with the help of the ImGui library for the
UI.

You can choose between different types of simulation:

- CPU
- GPU
- Barnes-Hut algorithm

The different algorithm still need some work to be optimized, but they are currently working.

You can also test the webgl version 🚀🚀 [here](https://im-rises.github.io/nbody-simulator-webgl/) 🚀🚀

The source code of the webgl version is available [here](https://github.com/Im-Rises/nbody-simulator-webgl).

## Videos

https://github.com/Im-Rises/nbody-simulator-docker/assets/59691442/622717a8-b508-4221-a79e-f00c07475979

Longer video 🚀🚀 [here](https://www.youtube.com/watch?v=OisxyKE_ioU) 🚀🚀

## Dependencies

- OpenGL version: 4.6.0
- GLSL version: 4.60
- GLFW version: 3.3.8
- Glad version: 0.1.36
- ImGui version: 1.89.4 WIP
- GLM version: 0.9.8

## How to use

To download the app, you can click one of the icons below (depending on your operating system). You can also click the
release section of the GitHub page.

Depending on you `operating system` you will need to install some libs, they are installed differently depending on your
system, please follow one of the section below `Windows` or `Linux` or `MacOs`.

For the compilation, follow the section `Compilation`.

The controls are described in the section `Controls`.

> **Warning**
> Be sure to put the `imgui.ini` file in the same folder as the executable.
> You can find it in the `root` of the project. If you don't do this, the UI will not be displayed correctly.

### Windows

<a href="https://github.com/Im-Rises/NBodySimulatorSSBO/releases/latest"><img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="cmakeLogo" style="height:40px;"/></a>

For Windows users you don't need to install the libs. You can just download the app and run it.

```bash
.\NBodySimulator.exe
```

### Linux

<a href="https://github.com/Im-Rises/NBodySimulatorSSBO/releases/latest"><img src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black" alt="cmakeLogo" style="height:40px;"/></a>

For Linux users, you need to install the GLFW lib, to do so type one of the following commands:

```bash
sudo apt-get install libglfw3
```

or if you're a developer and want to compile the app, please install this version of GLFW:

```bash
sudo apt-get install libglfw3-dev
```

Then you can start by double-clicking the executable of typing the following command next to it:

```bash
./NBodySimulatorLauncher
```

### MacOs

<a href="https://github.com/Im-Rises/NBodySimulatorSSBO/releases/latest"><img src="https://img.shields.io/badge/mac%20os-000000?style=for-the-badge&logo=apple&logoColor=white" alt="cmakeLogo" style="height:40px;"/></a>

For macOS users you will need to install Brew, please follow the instruction in the link below:  
<https://brew.sh>

Once it is installed, you can type the following command to install GLFW.

```bash
brew install glfw
```

Then you can start by double-clicking the executable of typing the following command next to it:

```bash
./NBodySimulatorLauncher
```

## Compilation

To compile the app, the first thing you need to do is install a C++ compiler:

- Visual Studio (MSVC)
- Mingw
- ...

You also need to install Cmake:  
<https://cmake.org>

Once your environment is set up, depending on your operating system you'll need to install some libs before compiling
the project. Refer to the section below `Windows` or `Linux` or `MacOs`.

### Windows

Windows users can directly compile the project by typing the following command at the project root folder:

```bash
cmake -B . -DCMAKE_BUILD_TYPE=Release
```

then

```bash
cmake --build . --config Release
```

### Linux

Linux's users need to install some libs before compiling the project:

First thing to do is to install CMake, type the following command to install it.

```bash
sudo apt-get install cmake
```

You also need to install the GLFW lib. Type the following command at the project root.

```bash
sudo apt-get install libglfw3-dev
```

You are now able to compile the project. Go to the project root and type the following command:

```bash
cmake -B . -DCMAKE_BUILD_TYPE=Release
```

then

```bash
cmake --build . --config Release
```

### MacOs

For macOS user, you should install brew package manager by following the instructions in the link below:  
<https://brew.sh>

Then type the following command to install cmake:

```bash
brew install cmake
```

and this one to install GLFW

```bash
brew install glfw
```

You are now able to compile the project. Go to the project root and type the following command:

```bash
cmake -B . -DCMAKE_BUILD_TYPE=Release
```

then

```bash
cmake --build . --config Release
```

## Controls

The speed and some parameters can be modified directly in the ImGui windows.

| Action              | Key                                 |
|---------------------|-------------------------------------|
| Translate camera    | WASD (← → ↑ ↓)                      |
| Pause/Resume        | P                                   |
| Move up/down camera | SPACE/SHIFT                         |
| Rotate camera       | Right Mouse Button + Mouse movement |
| Fullscreen          | F11                                 |
| Exit app            | ESC                                 |
| Show/Hide UI        | U                                   |

> **Note**
> You can the inputs by changing their attribution in the `InputManager.cpp` file,
> if so you'll need to compile the project.

## Github-Actions

[![CodeQL](https://github.com/Im-Rises/NBodySimulator/actions/workflows/codeql.yml/badge.svg?branch=main)](https://github.com/Im-Rises/NBodySimulator/actions/workflows/codeql.yml)
[![CMake](https://github.com/Im-Rises/NBodySimulator/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/Im-Rises/NBodySimulator/actions/workflows/cmake.yml)
[![Cpp Cmake Publish](https://github.com/Im-Rises/NBodySimulator/actions/workflows/cpp-cmake-publish.yml/badge.svg?branch=main)](https://github.com/Im-Rises/NBodySimulator/actions/workflows/cpp-cmake-publish.yml)
[![flawfinder](https://github.com/Im-Rises/NBodySimulator/actions/workflows/flawfinder.yml/badge.svg?branch=main)](https://github.com/Im-Rises/NBodySimulator/actions/workflows/flawfinder.yml)
[![cpp-linter](https://github.com/Im-Rises/NBodySimulator/actions/workflows/cpp-linter.yml/badge.svg?branch=main)](https://github.com/Im-Rises/NBodySimulator/actions/workflows/cpp-linter.yml)

The project is set with a set of different scripts:

- CodeQL: This script is used to check the code for security issues.
- CMake: This script is used to build the project.
- Cpp Cmake Publish: This script is used to publish the project on GitHub.
- Flawfinder: This script is used to check the code for security issues.
- Cpp Linter: This script is used to check the code for security issues.

## Libraries

glfw:  
<https://www.glfw.org/docs/latest/>

glm:  
<https://glm.g-truc.net/0.9.9/index.html>

glad:  
<https://glad.dav1d.de/>

Dear ImGui:  
<https://github.com/ocornut/imgui>

OpenGL:  
<https://www.opengl.org/>

glm:  
<https://glm.g-truc.net/0.9.9/index.html>

## Documentation

learnopengl (OpenGL tutorial):  
<https://learnopengl.com/In-Practice/2D-Game/Particles>

OpenCL NBody example:  
<https://github.com/KhronosGroup/OpenCL-SDK/tree/f510201a092363b66969888df49c68721ca2c4fb/samples/extensions/khr/nbody>

## Contributors

Quentin MOREL:

- @Im-Rises
- <https://github.com/Im-Rises>

[![GitHub contributors](https://contrib.rocks/image?repo=Im-Rises/NBodySimulator)](https://github.com/Im-Rises/NBodySimulator/graphs/contributors)
