#ifndef NBODY_SIMULATOR_BARNES_HUT_H
#define NBODY_SIMULATOR_BARNES_HUT_H

#include "BarnesHutOctree.h"
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../Entity.h"

class NBodySimulatorBarnesHut : public Entity {
private:
    static const char* const VertexShaderSource;
    static const char* const FragmentShaderSource;

    Shader shader;

    GLuint VAO, VBO;

public:
    std::vector<Particle> particles;
    std::vector<glm::vec3> sumForces;
#ifdef __unix__
    float deltaTime = 0.0F;
#endif

public:
    float spawnRadius = 3.0F;
    float gravity = 1.0F;
    float particleMass = 1.0F;
    float softening = 10.0F;
    float damping = 0.99F;
    float interactionPercent = 1.0F;

public:
    explicit NBodySimulatorBarnesHut(int particleCount = 100000);

    NBodySimulatorBarnesHut(const NBodySimulatorBarnesHut&) = delete;
    auto operator=(const NBodySimulatorBarnesHut&) -> NBodySimulatorBarnesHut& = delete;
    NBodySimulatorBarnesHut(NBodySimulatorBarnesHut&&) = delete;
    auto operator=(NBodySimulatorBarnesHut&&) -> NBodySimulatorBarnesHut& = delete;

    ~NBodySimulatorBarnesHut() override;

public:
    void update(const float& deltaTime) final;

    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) final;

    void reset();

private:
    void randomizeParticles();

public:
    void setParticlesCount(const size_t& count);

    [[nodiscard]] auto getParticlesCount() const -> size_t;
};



#endif // NBODY_SIMULATOR_BARNES_HUT_H
