#include "operations.hpp"
#include "scene.hpp"
#include "classes/hermite.hpp"
#include "framework/gl/texture.hpp"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <glm/gtx/string_cast.hpp>


Scene03::Scene03() {
    program.load("Scene3.vert", "Scene3.frag");
    ssaoProgram.load("ssao.vert", "ssao.frag");
    walls.load("meshes/sc3_waende.obj");
    boden.load("meshes/sc3_boden.obj");
    holz.load("meshes/sc3_stuehle_pult.obj");
    beamer.load("meshes/sc3_beamer.obj");
    tuerLinks.load("meshes/sc3_tuer_links.obj");
    tuerRechts.load("meshes/sc3_tuer_rechts.obj");
    sphere.load("meshes/highpolysphere.obj");
    hullin.load("meshes/hullin.obj");
    suzanne.load("meshes/suzanne.obj");
    bunny.load("meshes/bunny.obj");
    laptopDeckel.load("meshes/sc3_laptopdeckel.obj");
    laptopTastatur.load("meshes/sc3_laptoptastatur.obj");
    laptopScreen.load("meshes/plane.obj");
    folien.load("meshes/plane.obj");

    holztexture.load(Texture::Format::SRGB8, "textures/Wood.png", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    bodenTex.load(Texture::Format::SRGB8, "textures/Boden.jpg", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    wallTex.load(Texture::Format::SRGB8, "textures/Wand.jpg", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    hullinTex.load(Texture::Format::SRGB8, "textures/Hullin.png", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    hullinTex.load(Texture::Format::SRGB8, "textures/Hullin.png", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));
    float metallness = 0.0f;
    bool useOrenNayar = true;
    vec3 lightPos = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
    setupGBuffer();
    program.set("uLightPos", lightPos);
    program.set("uUseOrenNayar", useOrenNayar);
    program.set("uMetallness", metallness);
    camera_path_points = {
        // Der Fall nach unten
        quintic_hermite_point{vec3(0.02f, 0.0f, 0.0f), vec3(0.0f, 0.0f, PI/16), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, 0.0f, PI/16), vec3(0.0f, 0.0f, PI/16), vec3(0.0f, 0.0f, PI/2)},
        quintic_hermite_point{vec3(0.02f, 0.0f, PI/14), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        quintic_hermite_point{vec3(0.02f, PI/32, -PI/32), vec3(0.0f, PI/8, -PI/16), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/32, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/32, -PI/16), vec3(0.0f, PI/16, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/16, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/16, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/16, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/16, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/16, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        // Blick auf den Laptop
        quintic_hermite_point{vec3(0.02f, PI/32, PI/32), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/32, PI/32), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, -PI/24, PI/20), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, -PI/24, PI/20), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, -PI/24, PI/20), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, -PI/24, PI/20), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        // Flug auf den Laptop zu
        quintic_hermite_point{vec3(0.019f, -PI/24, PI/20), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.016f, -PI/24, PI/20), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.016f, -PI/24, PI/20), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    };
    view_path_points = {
        // Flug an den Platz
        quintic_hermite_point{vec3(1.416f, 4.277f, 9.275f), vec3(-1.424f, -0.5f, -3.283f), vec3(-0.2f, -0.3f, -0.4f)},
        //quintic_hermite_point{vec3(-0.208f, -0.281f, -3.208f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 2.949f, 3.121f), vec3(0.0f, -2.224f, -6.2f), vec3(0.0f, -0.5f, 3.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        // Blick auf den Laptop
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        // Flug auf den Laptop zu
        quintic_hermite_point{vec3(-0.083978f, -0.050709f, -1.565814f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.083978f, -0.050709f, -1.565814f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.083978f, -0.050709f, -1.565814f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},

    };


    // ======================================================================================================


    /** 
     *  NOTE:  Hullin-Pfad
    */
    this -> hullinPathPoints = {
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        // Hullin schaut kurz rein
        hermite_point{vec3( 5.8298f, -1.5f, -10.8305f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 3.8298f, -1.5f, -10.8305f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 3.8298f, -1.5f, -10.8305f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 5.8298f, -1.5f, -10.8305f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3( 6.7341f, -1.5f, -12.1000f), vec3(0.0f,  0.0f, 0.0f)},
        // Hullin läuft rein
        hermite_point{vec3( 3.1000f, -1.5f, -11.8246f), vec3(0.0f,  0.8f, 0.0f)},
        hermite_point{vec3( 2.0000f, -1.5f, -09.0236f), vec3(0.0f, -0.8f, 0.0f)},
        hermite_point{vec3( 0.9000f, -1.5f, -09.0236f), vec3(0.0f,  0.8f, 0.0f)},
        hermite_point{vec3(-0.2000f, -1.5f, -09.0236f), vec3(0.0f, -0.8f, 0.0f)},
        hermite_point{vec3(-1.3000f, -1.5f, -09.0236f), vec3(0.0f,  0.8f, 0.0f)},
        hermite_point{vec3(-2.4000f, -1.5f, -09.0236f), vec3(0.0f, -0.8f, 0.0f)},
        hermite_point{vec3(-3.5000f, -1.3f, -09.0236f), vec3(0.0f,  0.8f, 0.0f)},
        // Hullin nimmt seinen rechtmäßigen Platz ein
        hermite_point{vec3(-4.9081f, -1.3f, -09.0236f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3(-4.9081f, -0.8f, -08.9779f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3(-4.9081f, -0.8f, -08.9779f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.0f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.0f, 0.0f)},

        // Hullin hält Vorlesung
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        // Vorlesung geht weiter
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  0.2f, 0.0f)},
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.4f, 0.0f)},
        
        // Hullin hält Vorlesung 2 electric boogaloo
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        // Vorlesung geht weiter 2 electric boogaloo
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  -0.4f, 0.0f)},
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  0.2f, 0.0f)},
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        
        // Hullin hält Vorlesung 3 
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        // Vorlesung geht weiter 3
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  -0.4f, 0.0f)},
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  0.2f, 0.0f)},
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},

        // Hullin hält Vorlesung 4 schon so spät? 
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        // Vorlesung geht weiter 4 schon so spät?
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  -0.4f, 0.0f)},
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  0.2f, 0.0f)},
        hermite_point{vec3(-0.8194f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  -0.2f, 0.0f)},
        hermite_point{vec3(-6.3081f, -0.8f, -08.2779f), vec3(0.0f,  0.4f, 0.0f)},

    };
    this -> hullinPath.set_path(hullinPathPoints);

    this -> hullinRotationPathPoints = {
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        // Hullin schaut kurz rein
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, PI/2, PI/8), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, PI/2, PI/8), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, PI/2+PI/16, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        // Hullin läuft rein
        hermite_point{vec3(0.0f, PI/2+PI/4, 0.0f), vec3(0.0f, 0.0f,  PI/8)},
        hermite_point{vec3(0.0f, PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f,  PI/8)},
        hermite_point{vec3(0.0f, 5*PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, 5*PI/2, 0.0f), vec3(0.0f, 0.0f,  PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f, -PI/8)},
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f,  PI/8)},
        // Hullin nimmt seinen rechtmäßigen Platz ein
        hermite_point{vec3(0.0f, PI/2, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, -5*PI/2, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, -5*PI/2, 0.0f), vec3(PI/16, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(-PI/16, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  0.0f)},

        // Hullin hält Vorlesung
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, -1*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        // Vorlesung geht weiter
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},

        // Hullin hält Vorlesung 2 electric boogaloo
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, -1*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        // Vorlesung geht weiter 2 electric boogaloo
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/23)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},

        // Hullin hält Vorlesung 3
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, -1*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        // Vorlesung geht weiter 3
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/23)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},

        // Hullin hält Vorlesung 4 schon so spät?
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, -1*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        // Vorlesung geht weiter 4 schon so spät?
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/32)},
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},
        hermite_point{vec3(0.0f, PI/2-PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/16)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  PI/23)},
        hermite_point{vec3(0.0f, -5*PI/2+PI/8, 0.0f), vec3(0.0f, 0.0f,  -PI/32)},


    };
    this -> hullinRotationPath.set_path(hullinRotationPathPoints);
    //std::cout << to_string(hullinRotationPath.get_path()[1].pos) << std::endl;



    // NOTE: Tuer-Pfad und Laptop-Pfad
    
    std::vector<hermite_point> tuerPathPoints = {
        hermite_point{vec3(0.0f, PI/4, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
    };
    this -> tuerPath.set_path(tuerPathPoints);
    
    std::vector<hermite_point> laptopPathPoints = {
        hermite_point{vec3(-9*PI/16, 0.0f, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
    };
    this -> laptopPath.set_path(laptopPathPoints);

}

int get_folie(int time) {
    if      (time < 14) return 0;
    else if (time < 18) return 1;
    else if (time < 22) return 2;
    else if (time < 26) return 3;
    else if (time < 30) return 4;
    else if (time < 34) return 5;
    else if (time < 38) return 6;
    else if (time < 42) return 7;
    else if (time < 46) return 8;
    return 0;
}

int get_laptop_screen(int time) {
    if      (time < 4) return 0;
    else if (time < 7) return 1;
    else if (time < 10) return 2;
    else if (time < 12) return 3;
    else if (time < 13) return 0;
    else if (time < 20) return 4;
    else if (time < 22) return 0;
    else if (time < 28) return 5;
    else if (time < 30) return 0;
    else if (time < 88) return 6;
    return 0;
}



GLuint gBuffer;
GLuint gPosition, gNormal;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void Scene03::setupGBuffer() {
    // G-buffer setup
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // Position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // Normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // Create and attach depth buffer (renderbuffer)
    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    // Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // SSAO FBO setup
    glGenFramebuffers(1, &ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

    // SSAO color buffer
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

    // Check if SSAO framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ssaoKernel.resize(64);
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
    std::default_random_engine generator;
    for (unsigned int i = 0; i < 64; ++i) {
        glm::vec3 sample(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator)
        );
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0;
        scale = glm::mix(0.1f, 1.0f, scale * scale);
        ssaoKernel[i] = sample;
    }
    std::vector<glm::vec3> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++) {
        glm::vec3 noise(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            0.0f);
        ssaoNoise.push_back(noise);
    }
    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


int Scene03::render(int frame, float time, MovableCamera& camera, bool DEBUG) {
    if (!DEBUG) {
        camera.setViewDirAlongSpline(time / 2);
        camera.setPosAlongSpline(time / 2);
    }
    // 1. Geometry Pass: Render the scene's geometry and store relevant data in the G-buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.bind();
    camera.updateIfChanged();
    //renderSceneObjects(program, camera, time);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. SSAO Pass: Compute SSAO using the G-buffer and store the result in an SSAO texture.
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoProgram.bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    ssaoProgram.set("gPosition", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    ssaoProgram.set("gNormal", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    ssaoProgram.set("texNoise", 2);
    ssaoProgram.set("projection", camera.projectionMatrix);
    ssaoProgram.set("samples", ssaoKernel);

    // Render a screen-filling quad to apply SSAO effect
    renderQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //combine with other Shader
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.bind();


    // Bind other necessary textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    program.set("gPosition", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    program.set("gNormal", 1);

        // Set the SSAO texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    program.set("ssaoTexture", 2);

    // Render the final scene with SSAO applied
    renderSceneObjects(program, camera, time);

    if (DEBUG) render_debug_objects(program, camera.projectionMatrix * camera.viewMatrix, camera.getViewDirAlongSpline(time / 2), camera.target);

    if (time >= 35.2f) return 1;
    return 0;
}


void Scene03::renderSceneObjects(Program& program, MovableCamera& camera, float time) {
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    program.bind();
    //camera.updateIfChanged();
    glm::vec3 cameraPos = camera.cartesianPosition;
    program.set("uCameraPos", cameraPos);
    
    vec3 hoersaalOffset = vec3(0.0f, -3.4f, 0.0f);
    float laptop_start_time = 21.5;


    // NOTE: Beamer
    mat4 localToWorld = translate(mat4(1.0f), hoersaalOffset);
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);
    program.set("uUseTexture", false);
    program.set("uColor", vec3(0.4f));
    beamer.draw();


    // NOTE: Tür
    program.set("uUseTexture", false);
    program.set("uColor", vec3(0.4));
    program.set("uMetallness", 0.8f);
    program.set("uRoughness", 0.9f);
    mat4 tuerTransformation = mat4(Operations::get_rotation_matrix(tuerPath.evaluateSpline((time-9))));
    vec3 tuerPos = hoersaalOffset + vec3(1.0466f, 0.0f, -10.4436f);
    mat4 tuerLocalToWorld = scale(translate(mat4(1.0f), tuerPos), vec3(1.0f));
    program.set("uLocalToWorld", tuerLocalToWorld);
    program.set("uLocalToClip", worldToClip * tuerLocalToWorld * tuerTransformation);
    tuerLinks.draw();
    program.set("uLocalToWorld", tuerLocalToWorld);
    program.set("uLocalToClip", worldToClip * tuerLocalToWorld);
    tuerRechts.draw();
    

    // NOTE: Suzanne
    program.set("uColor", vec3(0.248f, 0.180f, 0.126f));
    vec3 suzannePos = hoersaalOffset + vec3(-3.02f, 2.822f + 0.05f * sin(time), -2.5f);
    // vec3 suzannePos = debugPos;
    localToWorld = rotate(scale(translate(mat4(1.0f), suzannePos), vec3(0.6f)), glm::pi<float>(), vec3(0, 1, 0));
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);

    suzanne.draw();


    // NOTE: Bunny
    program.set("uColor", vec3(0.84f, 0.72f, 0.66f));
    vec3 bunnyPos = hoersaalOffset + vec3(1.142f, 2.822f, -3.854f);
    localToWorld = rotate(scale(translate(mat4(1.0f), bunnyPos), vec3(0.4f)), time / 2, vec3(0, 1, 0));
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);

    bunny.draw();

    
    // NOTE: Laptop
    program.set("uColor", vec3(0.1f));
    vec3 laptopPos = hoersaalOffset + vec3(-0.122f, 3.080f, -1.509f);
    mat4 laptopTransformation = mat4(Operations::get_rotation_matrix(laptopPath.evaluateSpline((time-laptop_start_time)/3)));
    localToWorld = rotate(translate(mat4(1.0f), laptopPos), 3.f, vec3(0, 1, 0)) * laptopTransformation;
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);

    laptopDeckel.draw();
    localToWorld = rotate(translate(mat4(1.0f), laptopPos), 3.f, vec3(0, 1, 0));
    program.set("uLocalToClip", worldToClip * localToWorld);
    program.set("uColor", vec3(0.7f));
    laptopTastatur.draw();

    localToWorld = translate(mat4(1.0f), hoersaalOffset);
    worldToClip = camera.projectionMatrix * camera.viewMatrix;
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);

    program.set("uUseTexture", true);

    holztexture.bind(Texture::Type::TEX2D, 4);
    program.set("uTexture", 4);
    program.set("uMetallness", 0.f);
    program.set("uRoughness", 0.2f);
    holz.draw();

    bodenTex.bind(Texture::Type::TEX2D, 5);
    program.set("uTexture", 5);
    program.set("uRoughness", 1.f);
    boden.draw();

    wallTex.bind(Texture::Type::TEX2D, 6);
    program.set("uTexture", 6);
    walls.draw();
    

    // NOTE: Folien
    program.set("uMetallness", 0.4f);
    program.set("uRoughness", 0.2f);
    folienzahl = get_folie(floor(time));
    folienTex.load(Texture::Format::SRGB8, "textures/folien/" + std::to_string(folienzahl) + ".png", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    folienTex.bind(Texture::Type::TEX2D, 4);
    program.set("uTexture", 4);
    //std::cout << "ESSS:  " << to_string(mat4(Operations::get_rotation_matrix(vec3(PI/2, 0.0f, 0.0f)))) << std::endl;
    this->drawMesh(2.0f, hoersaalOffset+vec3(-3.84588f, 5.0f, -10.0f), program, folien, worldToClip ,
                   mat4(
                     1.33141f, 0.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     0.0f,-1.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f
                   )
    );
 

    // NOTE: Laptop-Screen
    if (time > laptop_start_time + 3.5f) {
        laptopzahl = get_laptop_screen(floor(4*time - 4*laptop_start_time - 4*3.5f));
        laptopScreenTex.load(Texture::Format::SRGB8, "textures/laptop/" + std::to_string(laptopzahl) + ".png", 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        laptopScreenTex.bind(Texture::Type::TEX2D, 5);
        localToWorld = rotate(translate(mat4(1.0f), laptopPos), 3.f, vec3(0, 1, 0));
        program.set("uLocalToWorld", localToWorld);
        program.set("uLocalToClip", worldToClip * localToWorld);
        program.set("uTexture", 5);
        this->drawMesh(0.21f, laptopPos+vec3(0.034f, 0.28f, -0.055f), program, laptopScreen, worldToClip ,
                       mat4(Operations::get_rotation_matrix(vec3(-0.18f, -0.145f, 0.0f))) * mat4(
                         1.82184f, 0.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f,
                         0.0f,-1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f
                       )
        );
    }


    // NOTE: Prof. Hullin
    localToWorld = translate(mat4(1.0f), hoersaalOffset);
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);
    hullinTex.bind(Texture::Type::TEX2D, 3);
    program.set("uTexture", 3);
    hullinPos = hullinPath.evaluateSplineAllowLoop(time*2);
    mat4 hullinMatrix = mat4(Operations::get_rotation_matrix(hullinRotationPath.evaluateSplineAllowLoop(time*2)));
    this->drawMesh(1.5f, hullinPos, program, hullin, worldToClip, hullinMatrix);



}

void Scene03::renderQuad() {
    // Render a full-screen quad for SSAO computation
    static unsigned int quadVAO = 0;
    static unsigned int quadVBO;
    if (quadVAO == 0) {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 4);
    glBindVertexArray(0);
}

void Scene03::render_debug_objects(Program& program, mat4 worldToClip, vec3 playerPosition, vec3 target) {
    program.bind();
    program.set("uUseTexture", false);
    program.set("uColor", vec3(0.65f, 0.00f, 0.4f));
    for (int i = 0; i < view_path_points.size(); i++) {
        drawMesh(0.10f, view_path_points[i].pos, program, sphere, worldToClip);
    }
    
    program.set("uColor", vec3(0.6f, 0.6f, 0.6f));
    this->drawMesh(0.1f, vec3(0.0f, 0.0f, 0.0f), program, sphere, worldToClip);
    program.set("uColor", vec3(1.0f, 0.0f, 0.0f));
    this->drawMesh(0.1f, vec3(0.12f, 0.0f, 0.0f), program, sphere, worldToClip);
    program.set("uColor", vec3(0.0f, 1.0f, 0.0f));
    this->drawMesh(0.1f, vec3(0.0f, 0.12f, 0.0f), program, sphere, worldToClip);
    program.set("uColor", vec3(0.0f, 0.0f, 1.0f));
    this->drawMesh(0.1f, vec3(0.0f, 0.0f, 0.12f), program, sphere, worldToClip);
    program.set("uColor", vec3(0.0f, 1.0f, 1.0f));
    this->drawMesh(0.03f, playerPosition, program, sphere, worldToClip);
    program.set("uColor", vec3(1.0f, 1.0f, 0.0f));
    this->drawMesh(0.01f, target, program, sphere, worldToClip);
}


Scene03::~Scene03() {}
