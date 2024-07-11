#version 330 core

in vec3 interpNormal;
in vec2 interpTexCoord;

out vec3 fragColor;

uniform sampler2D heightmap;

uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uColor = vec3(1.0);

void main() {
    vec3 normal = normalize(interpNormal);
    vec3 lighting = uColor * max(dot(normal, uLightDir), max(dot(normal, -uLightDir), 0.0));
    lighting = lighting * texture(heightmap, interpTexCoord).r;
    fragColor = lighting;

    // float height = texture(heightmap, interpTexCoord).r;
}
