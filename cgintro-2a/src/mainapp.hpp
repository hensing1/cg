#pragma once

#include "framework/app.hpp"
#include "framework/gl/buffer.hpp"
#include "framework/gl/vertexarray.hpp"
#include "framework/gl/program.hpp"

class MainApp : public App {
   public:
    MainApp();

   protected:
    void init() override;
    void render() override;

   private:
    Program shaderProgram;
    VertexArray vao;
    Buffer vbo;
    Buffer ebo;
    GLuint uTime;
};