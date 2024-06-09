#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

uniform mat4 uWorldToClip = mat4(1.0);
uniform mat4 uLocalToWorld = mat4(1.0);

out vec3 interpNormal;
out vec3 worldPos;

void main() {
    vec4 homWorldPos = uLocalToWorld * vec4(position, 1.0);
    worldPos = homWorldPos.xyz / homWorldPos.w;
    gl_Position = uWorldToClip * homWorldPos;
    interpNormal = (uLocalToWorld * vec4(normal, 0.0)).xyz;
}