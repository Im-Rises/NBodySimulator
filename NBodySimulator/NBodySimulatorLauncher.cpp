#ifndef NBODY_SIMULATOR_LAUNCHER_H
#define NBODY_SIMULATOR_LAUNCHER_H

#include "NBodySimulatorLauncher.h"

#include "InputManager.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "Scene/Scene.h"
#include <iostream>
#include <chrono>
#include <thread>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __EMSCRIPTEN__
#include "imgui/libs/emscripten/emscripten_mainloop_stub.h"
#include <emscripten/html5.h>
#endif

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

NBodySimulatorLauncher::NBodySimulatorLauncher() {
    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() == 0)
        exit(1);

// Decide GL+GLSL versions
#if defined(__EMSCRIPTEN__)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only
#endif

    // Set display size
#ifdef __EMSCRIPTEN__
    // According to canvas
    emscripten_get_canvas_element_size("#canvas", &displayWidth, &displayHeight);
#else
    // According to init windowSize
    displayWidth = windowWidth;
    displayHeight = windowHeight;
#endif

    // Create window with graphics context
    std::string windowTitle = PROJECT_NAME.data();
    windowTitle += " (U to show/hide UI)";
    window = glfwCreateWindow(displayWidth, displayHeight, windowTitle.c_str(), nullptr, nullptr);
    if (window == nullptr)
        exit(1);
    glfwMakeContextCurrent(window);
    //    glfwSwapInterval(1); // Enable vsync
    glfwSwapInterval(0); // Disable vsync
    //    glfwWindowHint(GLFW_REFRESH_RATE, 0); // Disable refresh rate
    //    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE); // Disable refresh rate
    //    glfwWindowHint(GLFW_REFRESH_RATE, 60);

    // Callbacks
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, InputManager::key_callback);

#ifdef __EMSCRIPTEN__
    // Initialize OpenGL loader
    if (gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
        exit(1);
#else
    // Initialize OpenGL loader
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
        exit(1);
#endif

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
    {
        style.WindowRounding = 0.0F;
        style.Colors[ImGuiCol_WindowBg].w = 1.0F;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

#ifdef __EMSCRIPTEN__
    // Register emscripten callbacks
    emscripten_set_touchstart_callback("#canvas", (void*)&InputManager::dragMovementData, true, InputManager::touchStart_callback);
    emscripten_set_touchmove_callback("#canvas", (void*)&InputManager::dragMovementData, true, InputManager::touchMove_callback);
    emscripten_set_touchend_callback("#canvas", (void*)&InputManager::dragMovementData, true, InputManager::touchEnd_callback);
#endif

#ifndef __EMSCRIPTEN__
    centerWindow();
#endif

    // Setup OpenGL state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifndef __EMSCRIPTEN__
    glPointSize(pointSize);
#endif

    // Same line as above but with C++ string
    std::cout << "OpenGL vendor: " << getOpenGLVendor() << std::endl
              << "OpenGL version: " << getOpenGLVersion() << std::endl
              << "GLSL version: " << getGLSLVersion() << std::endl
              << "GLFW version: " << getGLFWVersion() << std::endl
              << "Glad version: " << getGladVersion() << std::endl
              << "ImGui version: " << getImGuiVersion() << std::endl
              << "GLM version: " << getGLMVersion() << std::endl;
}

NBodySimulatorLauncher::~NBodySimulatorLauncher() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void NBodySimulatorLauncher::start() {
    // Create the scene
    scene = std::make_unique<Scene>(displayWidth, displayHeight);

#ifdef _WIN32
    timeBeginPeriod(1);
#endif

    // Variables for the main loop
    std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
    float deltaTime = 0.0F;

#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (glfwWindowShouldClose(window) == 0)
#endif
    {
        auto currentTime = std::chrono::high_resolution_clock::now();

        deltaTime = std::chrono::duration<float>(currentTime - previousTime).count();

        auto startMs = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());

        handleInputs();

        handleUi(deltaTime);

        updateGame(fixedDeltaTime);

        updateScreen();

        auto endMs = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
        auto delayMs = fixedDeltaTime - std::chrono::duration_cast<std::chrono::duration<float>>(endMs - startMs).count();
        if (delayMs > 0.0F)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delayMs * 1000.0F)));
        }

        previousTime = currentTime;
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

#ifdef _WIN32
    timeEndPeriod(1);
#endif

    scene.reset();
}

void NBodySimulatorLauncher::handleInputs() {
    glfwPollEvents();

    /* Read keyboard inputs and update states (buffers) */
    if (InputManager::isLeftKeyPressed(window))
        scene->camera.moveLeft();

    if (InputManager::isRightKeyPressed(window))
        scene->camera.moveRight();

    if (InputManager::isForwardKeyPressed(window))
        scene->camera.moveForward();

    if (InputManager::isBackwardKeyPressed(window))
        scene->camera.moveBackward();

    if (InputManager::isUpKeyPressed(window) || InputManager::isShiftKeyPressed(window))
        scene->camera.moveUp();

    if (InputManager::isDownKeyPressed(window) || InputManager::isControlKeyPressed(window))
        scene->camera.moveDown();

    /* Read and update mouse controls */
    // Get mouse position or drag position
    double posX = 0, posY = 0;
#ifdef __EMSCRIPTEN__
    if (InputManager::dragMovementData.isUsingDrag)
    {
        posX = InputManager::dragMovementData.dragX;
        posY = InputManager::dragMovementData.dragY;
    }
    else
    {
#endif
        InputManager::getMousePosition(window, posX, posY);
#ifdef __EMSCRIPTEN__
    }
#endif

    // Get movement delta
    double mouseDeltaX = 0, mouseDeltaY = 0;
    calculateMouseMovement(posX, posY, mouseDeltaX, mouseDeltaY);

    // Read mouse inputs and update camera
    if (InputManager::isKeyMouseMovementPressed(window))
    {
        scene->camera.processMouseMovement(static_cast<float>(mouseDeltaX), static_cast<float>(mouseDeltaY));
    }
}

void NBodySimulatorLauncher::handleUi(float deltaTime) {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (isUiVisible)
    {
        {
            ImGui::Begin("Information");
            ImGui::Text("Author: %s", PROJECT_AUTHOR.data());
            ImGui::Text("Project: %s", PROJECT_NAME.data());
            ImGui::Text("Version: %s", PROJECT_VERSION.data());
            ImGui::Text("%.3f ms/frame (%.1f FPS)", deltaTime, 1.0F / deltaTime);
            ImGui::Text("Window width: %d", displayWidth);
            ImGui::Text("Window height: %d", displayHeight);
            ImGui::Text("GPU: %s", getOpenGLVendor().data());
            ImGui::Text("OpenGL version: %s", getOpenGLVersion().data());
            ImGui::Text("GLSL version: %s", getGLSLVersion().data());
            ImGui::End();
        }

        {
            ImGui::Begin("Camera settings");

            ImGui::Text("Position:");
            ImGui::DragFloat3("##position", reinterpret_cast<float*>(&scene->camera.position));

            ImGui::Text("Reset position:");
            ImGui::DragFloat3("##resetPosition", reinterpret_cast<float*>(&scene->camera.initPosition));
            ImGui::Button("Reset position");
            if (ImGui::IsItemClicked())
            {
                scene->camera.reset();
            }

            ImGui::NewLine();
            ImGui::Text("Pitch:");
            ImGui::Checkbox("Pitch constrained", &scene->camera.constrainPitch);
            ImGui::DragFloat("##pitch", &scene->camera.pitch);

            ImGui::Text("Yaw:");
            ImGui::DragFloat("##yaw", &scene->camera.yaw);

            ImGui::NewLine();
            ImGui::Text("FOV:");
            ImGui::DragFloat("##fov", &scene->camera.fov, 0.1F, 1.0F, 180.0F);

            ImGui::NewLine();
            ImGui::Text("Near plane:");
            ImGui::DragFloat("##near", &scene->camera.nearPlane, 0.1F);

            ImGui::Text("Far plane:");
            ImGui::DragFloat("##far", &scene->camera.farPlane, 0.1F);

            ImGui::NewLine();
            ImGui::Text("Speed:");
            ImGui::DragFloat("##speed", &scene->camera.movementSpeed);

            ImGui::Text("Sensitivity: ");
            ImGui::DragFloat("##sensitivity", &scene->camera.rotationSpeed, 0.1F);

            ImGui::End();
        }

        {
            ImGui::Begin("NBody simulator settings");

            // Selector to select version of the simulator
            ImGui::Text("NBody Simulation Version selection:");
            ImGui::Selectable("CPU##versionCPU", scene->getIsNbodySimulationType(NBodySimulatorType::CPU));
            if (ImGui::IsItemClicked())
            {
                scene->useCPUNbodySimulator();
            }
#ifndef __EMSCRIPTEN__
            ImGui::Selectable("GPU##versionGPU", scene->getIsNbodySimulationType(NBodySimulatorType::GPU));
            if (ImGui::IsItemClicked())
            {
                scene->useGPUNbodySimulator();
            }
#endif
            ImGui::Selectable("Barnes-Hut##versionBarnesHut", scene->getIsNbodySimulationType(NBodySimulatorType::BARNES_HUT));
            if (ImGui::IsItemClicked())
            {
                scene->useBarnesHutNbodySimulator();
            }
#if defined(__unix__) && !defined(__APPLE__)
            ImGui::Selectable("PThreads##versionPThreads", scene->getIsNbodySimulationType(NBodySimulatorType::PTHREADS));
            if (ImGui::IsItemClicked())
            {
                scene->usePThreadsNbodySimulator();
            }
#endif
            ImGui::NewLine();

            ImGui::Text("Particle count: %s", std::to_string(scene->nbodySimulator->getParticlesCount()).c_str());
            static int particlesCount = static_cast<int>(scene->nbodySimulator->getParticlesCount());
            ImGui::DragInt("##particlesCount", &particlesCount, 1, 1, MAX_PARTICLES_COUNT);
            ImGui::Button("Validate##ParticlesCountSetterButton");
            if (ImGui::IsItemClicked())
            {
                scene->nbodySimulator->setParticlesCount(particlesCount);
            }
            ImGui::NewLine();

            ImGui::Text("Reset simulation:");
            ImGui::Button("Reset##ResetBtn");
            if (ImGui::IsItemClicked())
            {
                resetScene();
            }
            ImGui::NewLine();

            ImGui::Text("Toggle pause:");
            ImGui::Button(scene->getIsPaused() ? "Resume##TogglePAuseBtn" : "Pause##TogglePAuseBtn");
            if (ImGui::IsItemClicked())
            {
                scene->togglePause();
            }
            ImGui::NewLine();

#ifndef __EMSCRIPTEN__
            ImGui::Text("Particle point size:");
            ImGui::DragFloat("##pointSize", &pointSize, 0.1F, 1.0F, 100.0F);
            ImGui::Button("Validate##PointSizeSetterButton");
            if (ImGui::IsItemClicked())
            {
                glPointSize(pointSize);
            }
            ImGui::NewLine();
#endif

            ImGui::Text("Spawn position:");
            ImGui::DragFloat3("##spawnPosition", reinterpret_cast<float*>(&scene->nbodySimulator->position));
            ImGui::NewLine();

            ImGui::Text("Spawn radius:");
            ImGui::DragFloat("##spawnRadius", &scene->nbodySimulator->spawnRadius, 0.1F, 0.1F, 100.0F);
            ImGui::NewLine();

            ImGui::Text("Particle mass:");
            ImGui::DragFloat("##particleMass", &scene->nbodySimulator->particleMass, 0.1F, 0.1F, 100.0F);
            ImGui::NewLine();

            ImGui::Text("Gravity:");
            ImGui::DragFloat("##gravity", &scene->nbodySimulator->gravity, 0.1F, 0.1F, 100.0F);
            ImGui::NewLine();

            ImGui::Text("Softening:");
            ImGui::DragFloat("##softening", &scene->nbodySimulator->softening, 0.1F, 0.1F, 100.0F);
            ImGui::NewLine();

            ImGui::Text("Damping:");
            ImGui::DragFloat("##damping", &scene->nbodySimulator->damping, 0.0F, 0.0F, 1.0F);
            ImGui::NewLine();

            ImGui::Text("Interaction percentage (GPU and CPU):");
            ImGui::DragFloat("##interactionPercent", &scene->nbodySimulator->interactionPercent, 0.0F, 0.0F, 1.0F);

            ImGui::Text("Theta (Barnes-Hut):");
            ImGui::DragFloat("##theta", &scene->nbodySimulator->theta, 0.0F, 0.0F, 1.0F);

            ImGui::End();
        }
    }

    ImGui::Render();
}

void NBodySimulatorLauncher::updateGame(float deltaTime) {
    scene->update(deltaTime);
}

void NBodySimulatorLauncher::updateScreen() {
    if (!isMinimized())
        updateViewport();

    clearScreen();
    scene->render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    const ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
}

void NBodySimulatorLauncher::resetScene() {
    scene->reset();
}

void NBodySimulatorLauncher::toggleScenePause() {
    scene->togglePause();
}

void NBodySimulatorLauncher::updateViewport() {
#ifdef __EMSCRIPTEN__
    emscripten_get_canvas_element_size("#canvas", &displayWidth, &displayHeight);
#else
    glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
    if (!isFullscreen)
    {
        windowWidth = displayWidth;
        windowHeight = displayHeight;
    }
#endif
    scene->updateProjectionMatrix(displayWidth, displayHeight);

    glViewport(0, 0, displayWidth, displayHeight);
}


void NBodySimulatorLauncher::centerWindow() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    windowPosX = (mode->width - displayWidth) / 2;
    windowPosY = (mode->height - displayHeight) / 2;
    glfwSetWindowPos(window, windowPosX, windowPosY);
}

void NBodySimulatorLauncher::toggleFullscreen() {
#ifndef __EMSCRIPTEN__
    if (isFullscreen)
    {
        glfwSetWindowMonitor(window, nullptr, 0, 0, windowWidth, windowHeight, 0);
        displayWidth = windowWidth;
        displayHeight = windowHeight;
        glfwSetWindowPos(window, windowPosX, windowPosY);
        isFullscreen = false;
    }
    else
    {
        glfwGetWindowPos(window, &windowPosX, &windowPosY);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        isFullscreen = true;
    }
#endif
}

void NBodySimulatorLauncher::toggleUiVisibility() {
    isUiVisible = !isUiVisible;
}

void NBodySimulatorLauncher::clearScreen() const {
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
        clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto NBodySimulatorLauncher::isMinimized() const -> bool {
    return glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0;
}

void NBodySimulatorLauncher::calculateMouseMovement(const double& xMouse, const double& yMouse, double& xMovement,
    double& yMovement) {
    static double lastMouseX = 0.0;
    static double lastMouseY = 0.0;

    xMovement = xMouse - lastMouseX;
    yMovement = lastMouseY - yMouse;

    lastMouseX = xMouse;
    lastMouseY = yMouse;
}

auto NBodySimulatorLauncher::getOpenGLVendor() -> std::string_view {
    return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

auto NBodySimulatorLauncher::getOpenGLVersion() -> std::string_view {
    return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

auto NBodySimulatorLauncher::getGLSLVersion() -> std::string_view {
    return reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
}

auto NBodySimulatorLauncher::getGLFWVersion() -> std::string {
    return std::to_string(GLFW_VERSION_MAJOR) + "." + std::to_string(GLFW_VERSION_MINOR) + "." +
           std::to_string(GLFW_VERSION_REVISION);
}

auto NBodySimulatorLauncher::getGladVersion() -> std::string_view {
    return "0.1.36";
}

auto NBodySimulatorLauncher::getImGuiVersion() -> std::string {
    return IMGUI_VERSION;
}

auto NBodySimulatorLauncher::getGLMVersion() -> std::string {
    return std::to_string(GLM_VERSION_MAJOR) + "." + std::to_string(GLM_VERSION_MINOR) + "." +
           std::to_string(GLM_VERSION_PATCH);
}

#endif
