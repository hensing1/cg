#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

uniform sampler2D hullin;

uniform mat4 uLocalToClip = mat4(1.0);
uniform mat4 uLocalToWorld = mat4(1.0);

out vec3 interpNormal;
out vec2 interpTexCoord;
out vec3 interpPosition;

void main() {
    //gl_Position = uLocalToClip * vec4(position, 1.0);
    //interpNormal = (uLocalToWorld * vec4(normal, 0.0)).xyz;
    //interpPosition = position;

    
    vec3 newPosition = normal * (1 + 0.055f * texture(hullin, texCoord).r);
    gl_Position = uLocalToClip * vec4(newPosition, 1.0);
    interpNormal = (uLocalToWorld * vec4(normal, 0.0)).xyz;
    interpTexCoord = texCoord;

}
