#include "NBodySimulatorTexture.h"

#include <random>

#include "../../../Utility/piDeclaration.h"

const char* const NBodySimulatorTexture::VertexShaderSource =
    R"(#version 300 es

        precision highp float;

        uniform sampler2D u_positionTexture;
        uniform sampler2D u_velocityTexture;

        uniform mat4 u_mvp;
        uniform float u_deltaTime;
        uniform float u_damping;
        uniform float u_particleMass;
        uniform float u_gravity;
        uniform float u_softening;
        uniform float u_isRunning;

        out vec3 v_velocity;

        void main()
        {
            vec3 sumForces = vec3(0.0f, 0.0f, 0.0f);

            // Get the current particle position
            vec3 position = texture(u_positionTexture, gl_VertexID / 10000.0f).xyz;
            vec3 velocity = texture(u_velocityTexture, gl_VertexID / 10000.0f).xyz;

//            gl_Position = u_mvp * vec4(a_position, 1.0);
//            v_velocity = a_velocity;
            gl_PointSize = 1.0f;
        }
)";

const char* const NBodySimulatorTexture::FragmentShaderSource =
    R"(#version 300 es

        precision highp float;

        in vec3 v_velocity;

        out vec4 o_fragColor;

        void main() {
            vec3 v_color = vec3(min(v_velocity.y, 0.8f), max(v_velocity.x, 0.5f), min(v_velocity.z, 0.5f));
            o_fragColor = vec4(v_color, 1.0f);
        }
)";

NBodySimulatorTexture::NBodySimulatorTexture(int particleCount) : shader(VertexShaderSource, FragmentShaderSource, false) {
    // Resize the particles vector
    particles.resize(particleCount);

    // Init the particles
    randomizeParticles();

    // Init the VAO
    glGenVertexArrays(1, &VAO);

    // Init the VBO
    glGenBuffers(1, &VBO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Set the VBO data
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_STATIC_DRAW);

    // Set the VAO attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
    glEnableVertexAttribArray(1);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

NBodySimulatorTexture::~NBodySimulatorTexture() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void NBodySimulatorTexture::update(const float& deltaTime) {
    if (isPaused)
        return;

    for (auto& particle : particles)
    {
        glm::vec3 sumForces = glm::vec3(0.0F, 0.0F, 0.0F);
        for (auto& otherParticle : particles)
        {
            if (&particle == &otherParticle)
            {
                continue;
            }

            // Calculate the distance between the particle and the point of gravity
            const glm::vec3 r = otherParticle.position - particle.position;
            const float rSquared = glm::dot(r, r) + softening;

            // Calculate the force
            sumForces += ((gravity * particleMass * particleMass * glm::normalize(r)) / rSquared) * isAttracting;
        }

        // Calculate the acceleration
        const glm::vec3 acceleration = sumForces / particleMass;

        // Calculate the velocity
        particle.velocity += acceleration * deltaTime;

        // Damp the velocity
        particle.velocity *= damping;

        // Calculate the position
        particle.position += particle.velocity * deltaTime + 0.5F * acceleration * deltaTime * deltaTime;
    }
}

void NBodySimulatorTexture::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Set the VBO data
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_STATIC_DRAW);

    // Bind the shader
    shader.use();

    // Set the uniform variables
    shader.setMat4("u_mvp", cameraProjectionMatrix * cameraViewMatrix);

    // Draw the particles
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(particles.size()));

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NBodySimulatorTexture::reset() {
    randomizeParticles();
}

void NBodySimulatorTexture::randomizeParticles() {
    // Init the random engine
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> randomFloats(0.0F, 2.0F * M_PI);

    // Init the particles as a sphere
    for (auto& particle : particles)
    {
        const float angle1 = randomFloats(randomEngine);
        const float angle2 = randomFloats(randomEngine);
        const float x = spawnRadius * std::sin(angle1) * std::cos(angle2);
        const float y = spawnRadius * std::sin(angle1) * std::sin(angle2);
        const float z = spawnRadius * std::cos(angle1);
        particle.position = glm::vec3(x, y, z) + position;
        particle.velocity = glm::vec3(0.0F, 0.0F, 0.0F);
    }
}

void NBodySimulatorTexture::setParticlesCount(const size_t& count) {
    particles.resize(count);
    randomizeParticles();
}

auto NBodySimulatorTexture::getParticlesCount() const -> size_t {
    return particles.size();
}
