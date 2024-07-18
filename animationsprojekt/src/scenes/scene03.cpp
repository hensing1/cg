#include "operations.hpp"
#include "scene.hpp"
#include "classes/hermite.hpp"
#include "framework/gl/texture.hpp"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include <glm/gtx/string_cast.hpp>


Scene03::Scene03() {
    program.load("Scene3.vert", "Scene3.frag");
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
    hullinTex.load(Texture::Format::SRGB8, "textures/Hullin.png", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));
    float metallness = 0.0f;
    bool useOrenNayar = true;
    float roughness = 0.5f;
    program.set("uLightDir", lightDir);
    program.set("uRoughness",roughness);
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
        quintic_hermite_point{vec3(0.02f, PI/16, -PI/16), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        // Blick auf den Laptop
        quintic_hermite_point{vec3(0.02f, PI/32, PI/32), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/32, PI/32), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(0.02f, PI/32, PI/32), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
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
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        // Blick auf den Laptop
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
        quintic_hermite_point{vec3(-0.281f, 0.355f, -0.010f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},

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

    };
    this -> hullinRotationPath.set_path(hullinRotationPathPoints);
    //std::cout << to_string(hullinRotationPath.get_path()[1].pos) << std::endl;



    // NOTE: Tuer-Pfad
    
    std::vector<hermite_point> tuerPathPoints = {
        hermite_point{vec3(0.0f, PI/4, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
        hermite_point{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f,  0.0f)},
    };
    this -> tuerPath.set_path(tuerPathPoints);

}

int get_folie(int time) {
    if      (time < 14) return 0;
    else if (time < 18) return 1;
    else if (time < 22) return 2;
    return 0;
}


int Scene03::render(int frame, float time, MovableCamera& camera, bool DEBUG) {
    if (!DEBUG) {
        camera.setViewDirAlongSpline(time / 2);
        camera.setPosAlongSpline(time / 2);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.bind();

    camera.updateIfChanged();
    glm::vec3 cameraPos = camera.cartesianPosition;
    program.set("uCameraPos", cameraPos);

    vec3 hoersaalOffset = vec3(0.0f, -3.4f, 0.0f);
    mat4 localToWorld = translate(mat4(1.0f), hoersaalOffset);
    mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);

    program.set("uUseTexture", false);

    program.set("uColor", vec3(0.4f));
    beamer.draw();


    program.set("uColor", vec3(0.2f));
    mat4 tuerTransformation = mat4(Operations::get_rotation_matrix(tuerPath.evaluateSpline((time-9))));
    vec3 tuerPos = hoersaalOffset + vec3(1.0466f, 0.0f, -10.4436f);
    mat4 tuerLocalToWorld = scale(translate(mat4(1.0f), tuerPos), vec3(1.0f));
    program.set("uLocalToWorld", tuerLocalToWorld);
    program.set("uLocalToClip", worldToClip * tuerLocalToWorld * tuerTransformation);
    tuerLinks.draw();
    //tuerPos = hoersaalOffset + vec3(3.5108f, 0.0f, -10.4436f);
    //tuerLocalToWorld = scale(translate(mat4(1.0f), tuerPos), vec3(1.0f));
    program.set("uLocalToWorld", tuerLocalToWorld);
    program.set("uLocalToClip", worldToClip * tuerLocalToWorld);
    tuerRechts.draw();
    
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);
    vec3 suzannePos = hoersaalOffset + vec3(-3.02f, 2.822f + 0.05f * sin(time), -2.5f);
    // vec3 suzannePos = debugPos;
    localToWorld = rotate(scale(translate(mat4(1.0f), suzannePos), vec3(0.6f)), glm::pi<float>(), vec3(0, 1, 0));
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);

    suzanne.draw();

    vec3 bunnyPos = hoersaalOffset + vec3(1.142f, 2.822f, -3.854f);
    localToWorld = rotate(scale(translate(mat4(1.0f), bunnyPos), vec3(0.4f)), time / 2, vec3(0, 1, 0));
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);

    bunny.draw();

    program.set("uColor", vec3(0.1f));

    vec3 laptopPos = hoersaalOffset + vec3(0.312f, 3.058f, -0.629f);
    localToWorld = rotate(translate(mat4(1.0f), laptopPos), 3.f, vec3(0, 1, 0));
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);

    laptopDeckel.draw();
    program.set("uColor", vec3(0.7f));
    laptopTastatur.draw();

    localToWorld = translate(mat4(1.0f), hoersaalOffset);
    program.set("uLocalToWorld", localToWorld);
    program.set("uLocalToClip", worldToClip * localToWorld);


    program.set("uColor", vec3(0.4f));
    beamer.draw();

    program.set("uUseTexture", true);

    holztexture.bind(Texture::Type::TEX2D, 0);
    program.set("uTexture", 0);
    holz.draw();

    bodenTex.bind(Texture::Type::TEX2D, 1);
    program.set("uTexture", 1);
    boden.draw();

    wallTex.bind(Texture::Type::TEX2D, 2);
    program.set("uTexture", 2);
    walls.draw();
    
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

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    hullinTex.bind(Texture::Type::TEX2D, 3);
    program.set("uTexture", 3);
    hullinPos = hullinPath.evaluateSplineAllowLoop(time*2);
    mat4 hullinMatrix = mat4(Operations::get_rotation_matrix(hullinRotationPath.evaluateSplineAllowLoop(time*2)));
    this->drawMesh(1.5f, hullinPos, program, hullin, worldToClip, hullinMatrix);
    //glDisable(GL_BLEND);


    if (DEBUG) render_debug_objects(program, worldToClip, camera.getViewDirAlongSpline(time / 2), camera.target);
    return 0;
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
