#ifndef NBODY_SIMULATOR_TEXTURE_H
#define NBODY_SIMULATOR_TEXTURE_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../Entity.h"

class NBodySimulatorTexture : public Entity {
private:
    static const char* const VertexShaderSource;
    static const char* const FragmentShaderSource;

    Shader shader;

    GLuint VAO, VBO;

    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;

        Particle() : position(glm::vec3(0.0F)), velocity(glm::vec3(1.0F, 1.0F, 1.0F)) {}
    };

    std::vector<Particle> particles;

    float isAttracting = 1.0F;

public:
    float spawnRadius = 3.0F;
    float gravity = 1.0F;
    float particleMass = 50.0F;
    float softening = 10.0F;
    float damping = 0.99F;

public:
    explicit NBodySimulatorTexture(int particleCount = 100000);

    NBodySimulatorTexture(const NBodySimulatorTexture&) = delete;
    auto operator=(const NBodySimulatorTexture&) -> NBodySimulatorTexture& = delete;
    NBodySimulatorTexture(NBodySimulatorTexture&&) = delete;
    auto operator=(NBodySimulatorTexture&&) -> NBodySimulatorTexture& = delete;

    ~NBodySimulatorTexture() override;

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

#endif // NBODY_SIMULATOR_TEXTURE_H
