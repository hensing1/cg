#pragma once

#include "classes/hermite.hpp"
#include "classes/movable_camera.hpp"
#include "classes/operations.hpp"
#include <glm/gtc/constants.hpp>
#include "classes/halfedge.hpp"

#include "framework/camera.hpp"
#include "framework/gl/program.hpp"
#include "framework/gl/texture.hpp"
#include "framework/mesh.hpp"

class Scene {
  public:
    Scene(); // constructor loads all required objects
    Scene(MovableCamera& camera, Program& program);
    virtual int render(int frame, float time, Program& program, MovableCamera& camera, bool DEBUG) = 0;  // NOTE: returns next scene
    virtual void init(MovableCamera& camera);
    virtual ~Scene() = 0; // destructor unloads all objects
  protected:
    std::vector<quintic_hermite_point> view_path_points;
    std::vector<quintic_hermite_point> camera_path_points;
    void drawMesh(float size, const vec3& pos, Program& program, Mesh& mesh, const mat4& worldToClip);
    void drawMesh(float size, const vec3& pos, Program& program, Mesh& mesh, const mat4& worldToClip, const mat4& transformation);
};

class TestScene : public Scene {
  public:
    TestScene(MovableCamera& camera, Program& program);
    int render(int frame, float time, Program& program, MovableCamera& camera, bool DEBUG) override;
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
        Scene01(MovableCamera& camera, Program& program);
        int render(int frame, float time, Program& program, MovableCamera& camera, bool DEBUG) override;
        ~Scene01();
    private:

  private:
    HDS generateIcosahedron();
    Mesh earth;
    // std::vector<Mesh::VertexPCN> earthVertices;
    // std::vector<unsigned int> earthIndices;
};

class Scene02 : public Scene {
    public:
        Scene02(MovableCamera& camera, Program& program);
        int render(int frame, float time, Program& program, MovableCamera& camera, bool DEBUG) override;
        // virtual void init(MovableCamera& camera) override;
        ~Scene02();
    private:
        void render_debug_objects(Program& program, mat4 worldToClip, vec3 playerPosition);
        Mesh campus;
        Mesh sphere;
};

class Scene03 : public Scene {
    public:
        Scene03(MovableCamera& camera, Program& program);
        int render(int frame, float time, Program& program, MovableCamera& camera, bool DEBUG) override;
        ~Scene03();
    private:
        void render_debug_objects(Program& program, mat4 worldToClip, vec3 playerPosition, vec3 target);
        Mesh hoersaal;
        Mesh laptop;
        Texture hullin;
        Mesh sphere;

};
