#include "mainapp.hpp"

#include <glad/glad.h>

#include <vector>

#include "config.hpp"
#include "framework/app.hpp"

// The framework enables sRGB rendering by default, this means that we can output colors in
// linear space in our fragment shader and the framebuffer will automatically convert them
// to sRGB using hardware acceleration.
// sRGB scales brightness in a way that is more similar to how humans perceive light.
// Linear space on the other hand is physically accurate and thus better suited to perform lighting calculations.
// Therefore note that colors we input are treated as linear and may appear different than expected.
const std::vector<float> VERTICES = {
    -0.6f, -0.5f, 0.0f, 1.f, 0.f, 0.f,
     0.6f, -0.5f, 0.0f, 0.f, 1.f, 0.f,
     0.0f,  0.5f, 0.0f, 0.f, 0.f, 1.f,
};

const std::vector<unsigned int> INDICES = {
    0, 1, 2,
};

/** 
 * In the constructor we load all assets, the members' default constructors
 * implicitly take care of the allocation of OpenGL resources.
 */
MainApp::MainApp() : App(600, 600) {
    setTitle(Config::PROJECT_NAME);

    // Load, link and compile shader program
    shaderProgram.load("colors.vert", "colors.frag");

    // Get the location of the uniform variable "uTime"
    uTime = glGetUniformLocation(shaderProgram.handle, "uTime");
    // Or using the framework
    // uTime = shaderProgram.uniform("uTime");

    // Load the mesh data into buffers
    // The VertexBufferObject (VBO) holds the vertex data
    vbo.load(Buffer::Type::ARRAY_BUFFER, VERTICES);
    // The ElementBufferObject (EBO) holds the indices
    ebo.load(Buffer::Type::INDEX_BUFFER, INDICES);

    // Bind buffers to a VertexArrayObject (VAO)
    vao.bind();
    vbo.bind(Buffer::Type::ARRAY_BUFFER);
    ebo.bind(Buffer::Type::INDEX_BUFFER);
    size_t stride = 6 * sizeof(float);
    // Tell OpenGL how to interpret the vertex data
    // glVertexAttribPointer(location, size, type, normalized, stride, offset)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) (0 * sizeof(float)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    vao.unbind();
}

/* Initialization of OpenGL flags */
void MainApp::init() {
    // Set background color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

/* Render loop */
void MainApp::render() {
    // Fill screen with the background color (clearing the window)
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the time uniform
    glProgramUniform1f(shaderProgram.handle, uTime, static_cast<float>(glfwGetTime()));
    // Or using the framework
    // shaderProgram.set(uTime, this->time);

    // Use our shader program
    shaderProgram.bind();
    // Bind our vertex data
    vao.bind();
    glDrawElements(GL_TRIANGLES, INDICES.size(), GL_UNSIGNED_INT, (void*) 0);
}