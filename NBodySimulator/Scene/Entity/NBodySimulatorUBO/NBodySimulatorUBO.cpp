#include "NBodySimulatorUBO.h"

#include <random>

#include "../../../Utility/piDeclaration.h"

#ifdef __EMSCRIPTEN_PTHREADS__
#include <pthread.h>
#endif

const char* const NBodySimulatorUBO::VertexShaderSource =
    R"(#version 300 es

        precision highp float;

        #define MAX_PARTICLES 100

        layout(location = 0) in vec3 a_color;

        uniform mat4 u_mvp;
        uniform float u_deltaTime;
        uniform float u_damping;
        uniform float u_particleMass;
        uniform float u_gravity;
        uniform float u_softening;
        uniform float u_isRunning;

        layout(std140) uniform BodyData {
            vec3 positions[MAX_PARTICLES];
            float offsets1[MAX_PARTICLES];
            vec3 velocities[MAX_PARTICLES];
            float offsets2[MAX_PARTICLES];
        };

        out vec3 v_color;

        void main() {
            int index = gl_VertexID;

            vec4 position = positions[index];
            vec4 velocity = velocities[index];

            vec3 sumForces = vec3(0.0f);
            for (int i = 0; i < MAX_PARTICLES; i++) {
                if (i == index) continue;

                vec4 otherPosition = positions[i];
                vec4 otherVelocity = velocities[i];

                vec4 r = otherPosition - position;
                float rSquared = dot(r, r) + u_softening;

                sumForces += normalize(r) * (u_gravity * u_particleMass * u_particleMass ) / (rSquared);
            }

            sumForces = mix(vec3(0.0, 0.0, 0.0), sumForces, u_isRunning);

            vec3 acceleration = sumForces / u_particleMass;
            vec3 position = particle.position + (particle.velocity * u_deltaTime + 0.5 * acceleration * u_deltaTime * u_deltaTime) * u_isRunning;
            vec3 velocity = particle.velocity + acceleration * u_deltaTime;

            velocity = mix(velocity, velocity * u_damping, u_isRunning);

            memoryBarrier();

            positions[index] = vec4(position, 1.0);
            velocities[index] = vec4(velocity, 1.0);

            gl_Position = u_mvp * vec4(position, 1.0);
            v_color = a_color;
        }
)";

const char* const NBodySimulatorUBO::FragmentShaderSource =
    R"(#version 300 es

        precision highp float;

        in vec3 v_color;

        out vec4 o_fragColor;

        void main() {
            o_fragColor = vec4(v_color, 1.0f);
        }
)";

NBodySimulatorUBO::NBodySimulatorUBO(int particleCount) : shader(VertexShaderSource, FragmentShaderSource, false) {
    // Resize the particles vector
    colors.resize(particleCount);

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

    //    // Set the VBO data
    //    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_STATIC_DRAW);
    //
    //    // Set the VAO attributes
    //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
    //    glEnableVertexAttribArray(0);
    //    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
    //    glEnableVertexAttribArray(1);
    //    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
    //    glEnableVertexAttribArray(2);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

NBodySimulatorUBO::~NBodySimulatorUBO() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

struct ThreadData {
    NBodySimulatorUBO* simulator;
    size_t start;
    size_t end;
};

void NBodySimulatorUBO::update(const float& deltaTime) {
    if (isPaused)
        return;
}

void NBodySimulatorUBO::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //    // Set the VBO data
    //    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_STATIC_DRAW);
    //
    //    // Bind the shader
    //    shader.use();
    //
    //    // Set the uniform variables
    //    shader.setMat4("u_mvp", cameraProjectionMatrix * cameraViewMatrix);
    //
    //    // Draw the particles
    //    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(particles.size()));

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NBodySimulatorUBO::reset() {
    randomizeParticles();
}

void NBodySimulatorUBO::randomizeParticles() {
    // Init the random engine
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> randomAngle(0.0F, static_cast<float>(2.0F * M_PI));
    std::uniform_real_distribution<float> randomColorValue(0.0F, 1.0F);

    // Init the particles as a sphere
    for (int i = 0; i < particles.size(); i++)
    {
        const float angle1 = randomAngle(randomEngine);
        const float angle2 = randomAngle(randomEngine);
        const float x = spawnRadius * std::sin(angle1) * std::cos(angle2);
        const float y = spawnRadius * std::sin(angle1) * std::sin(angle2);
        const float z = spawnRadius * std::cos(angle1);
        particles[i].position = glm::vec3(x, y, z) + position;
        particles[i].velocity = glm::vec3(0.0F, 0.0F, 0.0F);
        colors[i] = glm::vec3(randomColorValue(randomEngine), randomColorValue(randomEngine), randomColorValue(randomEngine));
    }
}

void NBodySimulatorUBO::setParticlesCount(const size_t& count) {
    randomizeParticles();
}

auto NBodySimulatorUBO::getParticlesCount() const -> size_t {
    return particles.size();
}
