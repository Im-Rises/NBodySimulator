#include "NBodySimulatorTF.h"

#include <random>
#include <iostream>

#include "../../../Utility/piDeclaration.h"

const char* const NBodySimulatorTF::VertexShaderSource =
    R"(#version 300 es

        precision highp float;

        in vec3 a_pos;
        in vec3 a_vel;

        out vec3 out_pos;
        out vec3 out_vel;

        out vec3 v_vel;

        uniform mat4 u_mvp;
        uniform float u_deltaTime;
        uniform float u_damping;
        uniform float u_particleMass;
        uniform float u_gravity;
        uniform float u_softening;
        uniform float u_isRunning;
        uniform int u_particlesCount;

        void main()
        {
            vec3 sumForces = vec3(0.0);

            for (int i = 0; i < u_particlesCount; ++i) {
                if (i == gl_VertexID) continue;

                vec3 otherPosition = a_pos;
                vec3 r = otherPosition -
                float rSquared = dot(r, r) + u_softening;
                sumForces += normalize(r) * (u_gravity * u_particleMass * u_particleMass) / rSquared;
            }

            vec3 acceleration = sumForces / u_particleMass;

            vec3 position = a_pos + (a_vel * u_deltaTime + 0.5 * acceleration * u_deltaTime * u_deltaTime) * u_isRunning;
            vec3 velocity = a_vel + acceleration * u_deltaTime;

            out_pos = position;
            out_vel = velocity * u_damping;

            gl_Position = u_mvp * vec4(position, 1.0);

            v_vel = velocity;
        }
)";

const char* const NBodySimulatorTF::FragmentShaderSource =
    R"(#version 300 es

        precision highp float;

        in vec3 v_vel;

        out vec4 o_fragColor;

        void main()
        {
            vec3 v_color = vec3(min(v_vel.y, 0.8f), max(v_vel.x, 0.5f), min(v_vel.z, 0.5f));
            o_fragColor = vec4(v_color, 1.0f);
        }
)";

NBodySimulatorTF::NBodySimulatorTF(int particlesCount) : shader(VertexShaderSource, FragmentShaderSource, { "out_pos", "out_vel" }, false) {
    // Set the particles count
    this->particlesCount = particlesCount;
    std::vector<Particle> particles(particlesCount);
    randomizeParticles(particles);

    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    glGenTransformFeedbacks(2, TFBO);

    for (int i = 0; i < 2; i++)
    {
        glBindVertexArray(VAO[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_DYNAMIC_COPY);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TFBO[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, VBO[i]);
    }

    glBindVertexArray(0);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    currentVAO = VAO[0];
    currentTFBO = TFBO[1];
}

NBodySimulatorTF::~NBodySimulatorTF() {
    glDeleteVertexArrays(2, VAO);
    glDeleteTransformFeedbacks(2, TFBO);
    glDeleteBuffers(2, VBO);
}


void NBodySimulatorTF::update(const float& deltaTime) {
    this->deltaTime = deltaTime;
}

void NBodySimulatorTF::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    shader.use();
    shader.setMat4("u_mvp", cameraProjectionMatrix * cameraViewMatrix);
    shader.setFloat("u_deltaTime", deltaTime);
    shader.setFloat("u_damping", damping);
    shader.setFloat("u_particleMass", particleMass);
    shader.setFloat("u_gravity", gravity);
    shader.setFloat("u_softening", softening);
    shader.setFloat("u_isRunning", static_cast<float>(!isPaused));
    shader.setInt("u_particlesCount", particlesCount);

    glBindVertexArray(currentVAO);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, currentTFBO);

    if (currentTFBO == TFBO[0])
    {
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, VBO[0]);
    }
    else
    {
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, VBO[1]);
    }

    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, particlesCount);
    glEndTransformFeedback();

    if (currentVAO == VAO[0])
    {
        currentVAO = VAO[1];
        currentTFBO = TFBO[0];
    }
    else
    {
        currentVAO = VAO[0];
        currentTFBO = TFBO[1];
    }

    glBindVertexArray(0);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void NBodySimulatorTF::reset() {
    std::vector<Particle> particles(particlesCount);

    randomizeParticles(particles);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NBodySimulatorTF::randomizeParticles(std::vector<Particle>& particles) {
    // Init the random engine
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> randomFloats(0.0F, 2.0F * M_PI);
    const std::uniform_real_distribution<float> randomFloats2(-1.0F, 1.0F);

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

// void NBodySimulatorTF::setAttractorPosition(const glm::vec3& pos) {
//     attractorPosition = pos;
// }
//
// void NBodySimulatorTF::setIsAttracting(const bool& value) {
//     isAttracting = static_cast<float>(value);
// }
//
// auto NBodySimulatorTF::getIsAttracting() const -> bool {
//     return isAttracting != 0.0F;
// }

void NBodySimulatorTF::setParticlesCount(const int& value) {
    particlesCount = value;
    reset();
}

auto NBodySimulatorTF::getParticlesCount() const -> size_t {
    return particlesCount;
}
