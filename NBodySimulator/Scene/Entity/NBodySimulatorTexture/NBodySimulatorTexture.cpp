#include "NBodySimulatorTexture.h"

#include <random>
#include <iostream>

#include "../../../Utility/piDeclaration.h"

const char* const NBodySimulatorTexture::VertexShaderPhysicSource =
    R"(#version 300 es

    precision highp float;

    void main() {
        gl_Position = vec4(0.0);
    }
)";

const char* const NBodySimulatorTexture::FragmentShaderPhysicSource =
    R"(#version 300 es

    precision highp float;

    uniform sampler2D texturePositionBuffer;
    uniform sampler2D textureVelocityBuffer;

    uniform float deltaTime;
    uniform float gravity;
    uniform float particleMass;
    uniform float softening;
    uniform float damping;

    uniform int particleCount;

    void main() {
        vec2 texelSize = vec2(1.0 / float(particleCount), 1.0);

        vec3 position = texture(texturePositionBuffer, gl_VertexID * texelSize).xyz;
        vec3 velocity = texture(textureVelocityBuffer, gl_VertexID * texelSize).xyz;

        vec3 acceleration = vec3(0.0);

        for (int i = 0; i < particleCount; i++) {
            vec3 otherPosition = texture(texturePositionBuffer, i * texelSize).xyz;
            vec3 otherVelocity = texture(textureVelocityBuffer, i * texelSize).xyz;

            vec3 difference = otherPosition - position;

            float distance = length(difference);
            float force = particleMass * particleMass / ((distance * distance) + softening);

            acceleration += difference * force;
        }

        velocity += acceleration * deltaTime * gravity;
        velocity *= damping;

        position += velocity * deltaTime;

        gl_FragColor[0] = position[0];
        gl_FragColor[1] = position[1];
    }
)";

const char* const NBodySimulatorTexture::VertexShaderRenderSource =
    R"(#version 300 es

        precision highp float;

        uniform sampler2D texturePositionBuffer;
        uniform sampler2D textureVelocityBuffer;

        uniform mat4 u_mvp;

        void main() {
            vec2 texelSize = vec2(1.0 / float(particleCount), 1.0);

            vec3 position = texture(texturePositionBuffer, gl_VertexID * texelSize).xyz;
            vec3 velocity = texture(textureVelocityBuffer, gl_VertexID * texelSize).xyz;

            gl_Position = u_mvp * vec4(position, 1.0);
        }
)";

const char* const NBodySimulatorTexture::FragmentShaderRenderSource =
    R"(#version 300 es

        precision highp float;

        void main() {
            gl_FragColor = vec4(1.0);
        }
)";


NBodySimulatorTexture::NBodySimulatorTexture(int particleCount) : physicShader(VertexShaderPhysicSource, FragmentShaderPhysicSource, false), renderShader(VertexShaderRenderSource, FragmentShaderRenderSource, false) {

    // Init the VAO
    glGenVertexArrays(1, &VAO);

    // Init the VBO
    glGenBuffers(1, &VBO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Bind the VAO
    glBindVertexArray(VAO);




    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

NBodySimulatorTexture::~NBodySimulatorTexture() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteFramebuffers(1, &framebuffer);
}

void NBodySimulatorTexture::update(const float& deltaTime) {
    //    if (isPaused)
    //        return;
    //
}

void NBodySimulatorTexture::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


    //    Bind the shader
    //    shader.use();
    //
    //    // Set the uniform variables
    //    shader.setMat4("u_mvp", cameraProjectionMatrix * cameraViewMatrix);

    //    // Draw the particles
    //    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(particles.size()));

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NBodySimulatorTexture::reset() {
    randomizeParticles();
}

void NBodySimulatorTexture::randomizeParticles() {
    //    // Init the random engine
    //    std::mt19937 randomEngine;
    //    std::uniform_real_distribution<float> randomFloats(0.0F, 2.0F * M_PI);
    //
    //    // Init the particles as a sphere
    //    for (auto& particle : particles)
    //    {
    //        const float angle1 = randomFloats(randomEngine);
    //        const float angle2 = randomFloats(randomEngine);
    //        const float x = spawnRadius * std::sin(angle1) * std::cos(angle2);
    //        const float y = spawnRadius * std::sin(angle1) * std::sin(angle2);
    //        const float z = spawnRadius * std::cos(angle1);
    //        particle.position = glm::vec3(x, y, z) + position;
    //        particle.velocity = glm::vec3(0.0F, 0.0F, 0.0F);
    //    }
}

void NBodySimulatorTexture::setParticlesCount(const size_t& count) {
    //    particles.resize(count);
    //    randomizeParticles();
}

auto NBodySimulatorTexture::getParticlesCount() const -> size_t {
    //    return particles.size();
    return 0;
}
void NBodySimulatorTexture::initTexture(GLuint& texture, const size_t& count, const glm::vec4& color) {
    // Init the texture
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, static_cast<GLsizei>(count), 1, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //    // Init the texture data
    //    std::vector<glm::vec4> textureData(count, color);
    //    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<GLsizei>(count), 1, GL_RGBA, GL_FLOAT, textureData.data());

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}
