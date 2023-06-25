#ifndef NBODY_SIMULATOR_H
#define NBODY_SIMULATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../Entity.h"

class NBodySimulatorUBO : public Entity {
private:
    static const char* const VertexShaderSource;
    static const char* const FragmentShaderSource;

    Shader shader;

    GLuint VAO, VBO;

    struct Particle {
        glm::vec3 position;
        float offset1;
        glm::vec3 velocity;
        float offset2;

        Particle() : position(glm::vec3(0.0F)), velocity(glm::vec3(0.0F)) {}
    };

    std::vector<Particle> particles;
    std::vector<glm::vec3> colors;

public:
    float spawnRadius = 3.0F;
    float gravity = 1.0F;
    float particleMass = 1.0F;
    float softening = 10.0F;
    float damping = 0.99F;

public:
    explicit NBodySimulatorUBO(int particleCount = 100000);

    NBodySimulatorUBO(const NBodySimulatorUBO&) = delete;
    auto operator=(const NBodySimulatorUBO&) -> NBodySimulatorUBO& = delete;
    NBodySimulatorUBO(NBodySimulatorUBO&&) = delete;
    auto operator=(NBodySimulatorUBO&&) -> NBodySimulatorUBO& = delete;

    ~NBodySimulatorUBO() override;

public:
    void update(const float& deltaTime) override;

    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;

    void reset();

private:
    void randomizeParticles();

public:
    void setParticlesCount(const size_t& count);

    [[nodiscard]] auto getParticlesCount() const -> size_t;
};

#endif // NBODY_SIMULATOR_H
