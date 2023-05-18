# NBodySimulator

<p align="center">
      <img src="https://user-images.githubusercontent.com/59691442/183268126-b3d19e66-8f2d-463a-805e-ae6ef7cc6c01.png" alt="cmakeLogo" style="height:60px;"/>
      <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="cppLogo" style="height:60px;"/>
      <img src="https://img.shields.io/badge/OpenGL-FFFFFF?style=for-the-badge&logo=opengl" alt="openglLogo" style="height:60px;"/>
      <img src="https://user-images.githubusercontent.com/59691442/190315147-ec9dc33f-0090-4f0d-98ab-514eb1463e01.png" alt="glfwLogo" style="height:60px;"/>
</p>

## Description

This is a simple nbody simulator made with OpenGL and C++ with the help of the ImGui library for the UI.

## 🚀🚀[you can try the webgl build online by clicking here](https://im-rises.github.io/nbody-simulator-webgl/) 🚀🚀

The source code of the WebGL can be found at the following link:  
<https://github.com/Im-Rises/nbody-simulator-webgl>

> **Note:**   
> The tests are running at 144Hz with 10 000 000 particles on a Windows 11 machine with an Nvidia RTX 2070 6GB.

## TODO

- [ ] Use OpenCL to compute the particles for better performance

## Images

## Videos

## Features

- [x] ImGui UI
- [x] Camera movement
- [x] Camera settings
- [x] Mouse drag of the particles
- [x] Fullscreen toggle
- [x] Pause simulation

## Dependencies

- OpenGL version: 4.6.0
- GLSL version: 4.60
- GLFW version: 3.3.8
- Glad version: 0.1.36
- ImGui version: 1.89.4 WIP
- GLM version: 0.9.8

<!--
## Quickstart

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
.\NBodySimulatorSSBO.exe
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
| Drag particles      | Left Mouse Button + Mouse movement  |

> **Note**
> You can the inputs by changing their attribution in the `InputManager.cpp` file,
> if so you'll need to compile the project.

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

> **Note**  
> If you're using Visual Studio, you can install CMake directly from the IDE (Visual Studio Installer).
> Then you need to open the Project as a CMake Project, not a Visual Studio Project!

> **Warning**  
> The project is set up to be built using CMake and vc2019 for Windows. If you want to modify the
> compiler for vc2022 or other you will need to change the CMakeLists.txt .lib linking file accordingly to your vc20**
> version.

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

-->

## Github-Actions

[![CodeQL](https://github.com/Im-Rises/NBodySimulator/actions/workflows/codeql.yml/badge.svg?branch=main)](https://github.com/Im-Rises/NBodySimulator/actions/workflows/codeql.yml)
[![CMake](https://github.com/Im-Rises/NBodySimulator/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/Im-Rises/NBodySimulator/actions/workflows/cmake.yml)
[![Cpp Cmake Publish](https://github.com/Im-Rises/NBodySimulator/actions/workflows/cpp-cmake-publish.yml/badge.svg?branch=main)](https://github.com/Im-Rises/NBodySimulator/actions/workflows/cpp-cmake-publish.yml)
[![flawfinder](https://github.com/Im-Rises/NBodySimulator/actions/workflows/flawfinder.yml/badge.svg?branch=main)](https://github.com/Im-Rises/NBodySimulator/actions/workflows/flawfinder.yml)
[![Microsoft C++ Code Analysis](https://github.com/Im-Rises/NBodySimulator/actions/workflows/msvc.yml/badge.svg?branch=main)](https://github.com/Im-Rises/NBodySimulator/actions/workflows/msvc.yml)
[![cpp-linter](https://github.com/Im-Rises/NBodySimulator/actions/workflows/cpp-linter.yml/badge.svg?branch=main)](https://github.com/Im-Rises/NBodySimulator/actions/workflows/cpp-linter.yml)

The project is set with a set of different scripts:

- CodeQL: This script is used to check the code for security issues.
- CMake: This script is used to build the project.
- Cpp Cmake Publish: This script is used to publish the project on GitHub.
- Flawfinder: This script is used to check the code for security issues.
- Microsoft C++ Code Analysis: This script is used to check the code for security issues.
- Cpp Linter: This script is used to check the code for security issues.

## Libraries

glfw:  
<https://www.glfw.org/docs/latest/>

glm:  
<https://glm.g-truc.net/0.9.9/index.html>

glad:  
<https://glad.dav1d.de/>

stb (stb_image):  
<https://github.com/nothings/stb/blob/master/stb_image.h>

Dear ImGui:  
<https://github.com/ocornut/imgui>

OpenGL:  
<https://www.opengl.org/>

## Documentation

learnopengl (OpenGL tutorial):  
<https://learnopengl.com/In-Practice/2D-Game/Particles>

unrealistic.dev (Change CMake working directory):  
<https://unrealistic.dev/posts/setting-debug-parameters-with-cmake-tools-for-visual-studio>

BobBaykiller (Newtonian Particle Simulator):  
<https://github.com/BoyBaykiller/Newtonian-Particle-Simulator>

## Contributors

Quentin MOREL:

- @Im-Rises
- <https://github.com/Im-Rises>

[![GitHub contributors](https://contrib.rocks/image?repo=Im-Rises/NBodySimulator)](https://github.com/Im-Rises/NBodySimulator/graphs/contributors)
