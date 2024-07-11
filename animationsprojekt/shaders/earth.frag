#version 330 core

in vec3 interpNormal;
in vec2 interpTexCoord;

out vec3 fragColor;

uniform sampler2D heightmap;

uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uColor = vec3(1.0);
uniform vec3 oceanColor;
uniform vec3 landColor;

void main() {
    float height = texture(heightmap, interpTexCoord).r;

    vec3 color = height > 0.15 ? landColor : oceanColor;

    vec3 normal = normalize(interpNormal);
    vec3 lighting = color * max(dot(normal, uLightDir), 0.05);
    fragColor = lighting;
}
