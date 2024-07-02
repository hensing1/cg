#pragma once

#include "classes/hermite.hpp"
#include "classes/operations.hpp"

#include "framework/gl/program.hpp"
#include "framework/camera.hpp"
#include "framework/mesh.hpp"



#define PI 3.141592653589793


class Scene {
  public:
    Scene(); // constructor loads all required objects
    virtual void render(int frame, float time, Program& program, Camera& camera) = 0;
    virtual ~Scene() = 0; // destructor unloads all objects
  protected:
    void drawMesh(float size, const vec3& pos, Program& program, Mesh& mesh, const mat4& worldToClip);
    void drawMesh(float size, const vec3& pos, Program& program, Mesh& mesh, const mat4& worldToClip, const mat4& transformation);
    
    Operations operations;
};

class TestScene : public Scene {
  public:
    TestScene();
    void render(int frame, float time, Program& program, Camera& camera) override;
    ~TestScene();

  private:
    Mesh donut;
    Mesh cube;
    Mesh sphere;

    std::vector<hermite_point> donut_path_points;
    std::vector<hermite_point> cube_path_points;
    std::vector<hermite_point> sphere_path_points;
    std::vector<quintic_hermite_point> smooth_sphere_path_points;
    std::vector<hermite_point> rotation_path_points;

    Hermite donut_path;
    Hermite cube_path;
    Hermite sphere_path;
    QuinticHermite smooth_sphere_path;
    Hermite rotation_path;
};

class Scene01 : public Scene {
    public:
        Scene01();
        void render(int frame, float time, Program& program, Camera& camera) override;
        ~Scene01();
    private:

};

class Scene02 : public Scene {
    public:
        Scene02();
        void render(int frame, float time, Program& program, Camera& camera) override;
        ~Scene02();
    private:
        Mesh campus;
};

class Scene03 : public Scene {
    public:
        Scene03();
        void render(int frame, float time, Program& program, Camera& camera) override;
        ~Scene03();
    private:
        Mesh hoersaal;
        Mesh laptop;
};
