#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 normal;


uniform sampler2D holztexture;
uniform mat4 uLocalToClip = mat4(1.0);
uniform mat4 uLocalToWorld = mat4(1.0);


out vec3 interpNormal;
out vec2 interpTexCoords;
out vec3 worldPos;

void main() {
    gl_Position = uLocalToClip * vec4(position, 1.0);
    interpNormal = (uLocalToWorld * vec4(normal, 0.0)).xyz;
    vec4 worldPosition = uLocalToWorld * vec4(position, 1.0);
    worldPos = worldPosition.xyz;
    interpTexCoords = TexCoords;
}