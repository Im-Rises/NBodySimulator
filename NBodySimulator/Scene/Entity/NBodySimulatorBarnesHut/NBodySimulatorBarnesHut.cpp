#include "NBodySimulatorBarnesHut.h"

#include <random>

#include "../../../Utility/piDeclaration.h"

const char* const NBodySimulatorBarnesHut::VertexShaderSource =
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

const char* const NBodySimulatorBarnesHut::FragmentShaderSource =
    R"(#version 300 es

        precision highp float;

        in vec3 v_color;

        out vec4 o_fragColor;

        void main() {
            o_fragColor = vec4(v_color, 1.0f);
        }
)";

NBodySimulatorBarnesHut::NBodySimulatorBarnesHut(int particleCount) : shader(VertexShaderSource, FragmentShaderSource, false) {

}

NBodySimulatorBarnesHut::~NBodySimulatorBarnesHut() {
}

void NBodySimulatorBarnesHut::update(const float& deltaTime) {
}

void NBodySimulatorBarnesHut::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
}

void NBodySimulatorBarnesHut::reset() {
}

void NBodySimulatorBarnesHut::randomizeParticles() {
}

void NBodySimulatorBarnesHut::setParticlesCount(const size_t& count) {
}

auto NBodySimulatorBarnesHut::getParticlesCount() const -> size_t {
    return 0;
}
