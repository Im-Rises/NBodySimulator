#include "NBodySimulatorPThreads.h"

#include <random>

#include <pthread.h>
#include <iostream>

#include "../../../Utility/piDeclaration.h"

const char* const NBodySimulatorPThreads::VertexShaderSource =
    R"(#version 300 es

        precision highp float;

        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec3 a_velocity;
        layout(location = 2) in vec3 a_color;

        uniform mat4 u_mvp;

        out vec3 v_color;

        void main()
        {
            gl_Position = u_mvp * vec4(a_position, 1.0);
            v_color = a_color;
            gl_PointSize = 2.0f;
        }
)";

const char* const NBodySimulatorPThreads::FragmentShaderSource =
    R"(#version 300 es

        precision highp float;

        in vec3 v_color;

        out vec4 o_fragColor;

        void main() {
//            vec3 v_color = vec3(min(v_velocity.y, 0.8f), max(v_velocity.x, 0.5f), min(v_velocity.z, 0.5f));
//            o_fragColor = vec4(v_color, 1.0f);
            o_fragColor = vec4(v_color, 1.0f);
        }
)";

NBodySimulatorPThreads::NBodySimulatorPThreads(int particleCount) : shader(VertexShaderSource, FragmentShaderSource, false) {
    // Resize the particles vector
    particles.resize(particleCount);
    sumForces.resize(particleCount);

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
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
    glEnableVertexAttribArray(2);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

NBodySimulatorPThreads::~NBodySimulatorPThreads() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

struct ThreadData {
    NBodySimulatorPThreads* simulator;
    size_t start;
    size_t end;
    pthread_barrier_t* barrier;
};

void* updateParticlesThread(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    NBodySimulatorPThreads* simulator = data->simulator;

    const int particlesInteractionCount = simulator->particles.size() * simulator->interactionPercent;
    for (size_t i = data->start; i < data->end; ++i)
    {
        for (size_t j = 0; j < particlesInteractionCount; ++j)
        {
            if (i == j)
                continue;

            glm::vec3 const direction = simulator->particles[j].position - simulator->particles[i].position;
            float const distance = glm::length(direction);
            float const magnitude = (simulator->gravity * simulator->particleMass * simulator->particleMass) /
                                    ((distance * distance) + simulator->softening);
            simulator->sumForces[i] += magnitude * glm::normalize(direction);
        }
    }

    pthread_barrier_wait(data->barrier);

    for (size_t i = data->start; i < data->end; ++i)
    {
        const auto acceleration = simulator->sumForces[i] / simulator->particleMass;
        const float deltaTime = simulator->getDeltaTime();
        simulator->particles[i].position += deltaTime * simulator->particles[i].velocity +
                                            0.5F * deltaTime * deltaTime * acceleration;
        simulator->particles[i].velocity += deltaTime * acceleration;
        simulator->particles[i].velocity *= simulator->damping;
        simulator->sumForces[i] = glm::vec3(0.0F);
    }

    pthread_exit(NULL);
}

void NBodySimulatorPThreads::update(const float& deltaTime) {
    if (isPaused)
        return;

    // Update delta time
    this->deltaTime = deltaTime;

    // Calculate the number of particles per thread
    const size_t particlesPerThread = particles.size() / numThreads;

    // Create threads
    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];

    // Create barrier
    pthread_barrier_t barrier;

    // Init barrier
    pthread_barrier_init(&barrier, NULL, numThreads);

    // Create threads for update
    for (size_t i = 0; i < numThreads; ++i)
    {
        threadData[i].simulator = this;
        threadData[i].start = i * particlesPerThread;
        threadData[i].end = (i == numThreads - 1) ? particles.size() : (i + 1) * particlesPerThread;
        threadData[i].barrier = &barrier;

        pthread_create(&threads[i], NULL, updateParticlesThread, &threadData[i]);
    }

    // Wait for update threads to finish
    for (size_t i = 0; i < numThreads; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy barrier
    pthread_barrier_destroy(&barrier);
}

void NBodySimulatorPThreads::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
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

void NBodySimulatorPThreads::reset() {
    randomizeParticles();
}

void NBodySimulatorPThreads::randomizeParticles() {
    // Init the random engine
    std::random_device rd;
    std::mt19937 randomEngine(rd());
    std::uniform_real_distribution<float> randomAngle(0.0F, static_cast<float>(2.0F * M_PI));
    std::uniform_real_distribution<float> randomColorValue(0.0F, 1.0F);

    // Init the particles as a sphere
    for (auto& particle : particles)
    {
        const float angle1 = randomAngle(randomEngine);
        const float angle2 = randomAngle(randomEngine);
        const float x = spawnRadius * std::sin(angle1) * std::cos(angle2);
        const float y = spawnRadius * std::sin(angle1) * std::sin(angle2);
        const float z = spawnRadius * std::cos(angle1);
        particle.position = glm::vec3(x, y, z) + position;
        particle.velocity = glm::vec3(0.0F, 0.0F, 0.0F);
        particle.color = glm::vec3(randomColorValue(randomEngine), randomColorValue(randomEngine), randomColorValue(randomEngine));
    }
}

void NBodySimulatorPThreads::setParticlesCount(const size_t& count) {
    particles.resize(count);
    sumForces.resize(count);
    randomizeParticles();
}

auto NBodySimulatorPThreads::getParticlesCount() const -> size_t {
    return particles.size();
}

auto NBodySimulatorPThreads::getDeltaTime() const -> float {
    return deltaTime;
}
