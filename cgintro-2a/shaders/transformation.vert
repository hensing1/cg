#version 330 core
layout (location = 0) in vec3 position;

uniform float uTime;

// Translation
mat4 mov = mat4( 1. , 0. , 0. , sin(5*uTime)/3,
                 0. , 1. , 0. , cos(5*uTime)/3,
                 0. , 0. , 1. , 0.,
                 0. , 0. , 0. , 1.); // DONE?

// Scaling
mat4 sca = mat4( (sin(uTime)+4)/6 , 0. , 0. , 0.,
                 0. , (sin(uTime)+4)/6, 0. , 0.,
                 0. , 0. , 1. , 0.,
                 0. , 0. , 0. , 1.); // DONE?

// Rotation
mat4 rot = mat4( cos(uTime/2), -sin(uTime/2) , 0. , 0.,
                 sin(uTime/2),  cos(uTime/2) , 0. , 0.,
                 0. , 0. , 1. , 0.,
                 0. , 0. , 0. , 1.); // DONE?


void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0) * rot * sca * mov;
}
