#include "mainapp.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <vector>

#include "config.hpp"
#include "framework/app.hpp"

const float outer = 0.5f;
const float inner = outer / 3.0f;

const std::vector<float> VERTICES = {
    // TODO
};

const std::vector<unsigned int> INDICES = {
    // TODO
};

/** 
 * In the constructor we load all assets, the members' default constructors
 * implicitly take care of the allocation of OpenGL resources.
 */
MainApp::MainApp() : App(600, 600) {
    setTitle(Config::PROJECT_NAME);

    // Load, link and compile shader program
    shaderProgram.load("transformation.vert", "white.frag");

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
    size_t stride = 3 * sizeof(float);
    // Tell OpenGL how to interpret the vertex data
    // glVertexAttribPointer(location, size, type, normalized, stride, offset)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) (0 * sizeof(float)));
    glEnableVertexAttribArray(0);
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