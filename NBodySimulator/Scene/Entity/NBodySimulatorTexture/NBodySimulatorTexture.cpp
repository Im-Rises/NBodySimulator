#include "NBodySimulatorTexture.h"

#include <random>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../../../Utility/piDeclaration.h"

const char* const NBodySimulatorTexture::VertexShaderPhysicSource =
    R"(#version 300 es

    precision highp float;

    in vec3 a_position;

    out vec2 v_texCoord;

    void main() {
        gl_Position = vec4(a_position, 1.0);
        v_texCoord = a_position.xy * 0.5 + 0.5;
    }
)";

const char* const NBodySimulatorTexture::FragmentShaderPhysicSource =
    R"(#version 300 es

    precision highp float;

    uniform sampler2D u_texture1;
    uniform sampler2D u_texture2;

    in vec2 v_texCoord;

    out vec4 o_color;

    void main()
    {
        o_color = mix(texture(u_texture1, v_texCoord), texture(u_texture2, v_texCoord), 0.5);
    }
)";

constexpr float QuadVertices[18] = {
    -1.0F,
    -1.0F,
    0.0F,
    1.0F,
    1.0F,
    0.0F,
    -1.0F,
    1.0F,
    0.0F,
    -1.0F,
    -1.0F,
    0.0F,
    1.0F,
    -1.0F,
    0.0F,
    1.0F,
    1.0F,
    0.0F,
};


NBodySimulatorTexture::NBodySimulatorTexture(int particleCount) : shader(VertexShaderPhysicSource, FragmentShaderPhysicSource, false) /*, renderShader(VertexShaderRenderSource, FragmentShaderRenderSource, false)*/ {

    // Init the VAO
    glGenVertexArrays(1, &VAO);

    // Init the VBO
    glGenBuffers(1, &VBO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Set the VBO data
    glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW);

    // Set the VAO attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Load image, create texture and generate mipmaps
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Load image 2, create texture and generate mipmaps
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Set texture uniforms
    shader.use();
    shader.setInt("u_texture1", 0);
    shader.setInt("u_texture2", 1);

    // set the FBO
    glCreateFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glDrawBuffers(1, &FBO);

    // Create the render texture
    glGenTextures(1, &renderTexture);
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_RGBA, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

    // Unbind the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
    //    if (isPaused)
    //        return;
    //
}

void NBodySimulatorTexture::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Bind the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Bind the shader
    shader.use();

    // Draw the quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Unbind the FBO
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

// void NBodySimulatorTexture::initTexture(GLuint& texture, const size_t& count, const glm::vec4& color) {
//     // Init the texture
//     glBindTexture(GL_TEXTURE_2D, texture);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, static_cast<GLsizei>(count), 1, 0, GL_RGBA, GL_FLOAT, nullptr);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//     //    // Init the texture data
//     //    std::vector<glm::vec4> textureData(count, color);
//     //    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<GLsizei>(count), 1, GL_RGBA, GL_FLOAT, textureData.data());
//
//     // Unbind the texture
//     glBindTexture(GL_TEXTURE_2D, 0);
// }
