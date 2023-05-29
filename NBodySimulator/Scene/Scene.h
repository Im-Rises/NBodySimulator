#ifndef SCENE_H
#define SCENE_H

#include "Camera/Camera.h"

#ifdef __EMSCRIPTEN__
#include "Entity/NBodySimulatorTF/ParticleSimulatorTF.h"
#else
#include "Entity/NBodySimulatorSSBO/NBodySimulatorSSBO.h"
#endif

class Scene {
private:
    bool isPaused = false;

public:
    Camera camera;

#ifdef __EMSCRIPTEN__
    NBodySimulatorTF nbodySimulator;
#else
    NBodySimulatorSSBO nbodySimulator;
#endif

    //    std::vector<Entity*> entities;

public:
    Scene(int display_w, int display_h);

    void update(float deltaTime);

    void render();

public:
    void updateProjectionMatrix(int display_w, int display_h);

    void togglePause();

    void reset();

public:
    [[nodiscard]] auto getIsPaused() const -> bool;
};

#endif // SCENE_H
