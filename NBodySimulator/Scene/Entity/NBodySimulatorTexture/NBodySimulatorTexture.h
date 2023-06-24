#ifndef NBODY_SIMULATOR_TEXTURE_H
#define NBODY_SIMULATOR_TEXTURE_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../Entity.h"

class NBodySimulatorTexture : public Entity {
private:
    static const char* const VertexShaderPhysicSource;
    static const char* const FragmentShaderPhysicSource;

    //    static const char* const VertexShaderRenderSource;
    //    static const char* const FragmentShaderRenderSource;

    //    static constexpr float const QuadVertices[18];

    GLuint VAO, VBO;
    Shader shader;

    GLuint texture1;
    GLuint texture2;

    GLuint FBO;
    GLuint renderTexture;

    //    Shader physicShader;
    //    Shader renderShader;

    //    GLuint texturePositionBuffer[2];
    //    GLuint textureVelocityBuffer[2];
    //
    //    GLuint FBO;
    //
    //    GLuint pingPongIndex = 0;

    size_t particlesCount = 0;

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

private:
    //    void initTexture(GLuint& texture, const size_t& count, const glm::vec4& color);
};

#endif // NBODY_SIMULATOR_TEXTURE_H
