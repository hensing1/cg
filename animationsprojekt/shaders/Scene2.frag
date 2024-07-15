#version 330 core

in vec3 interpNormal;
in vec2 fragTexCoords;

out vec4 fragColor;

uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uColor = vec3(1.0);
uniform sampler2D texture1;

void main() {
    vec3 normal = normalize(interpNormal);
    vec4 texColor = texture(texture1, fragTexCoords);
    fragColor = texColor * vec4(uColor, 1.0);
}
