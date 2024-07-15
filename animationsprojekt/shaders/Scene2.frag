#version 330 core

in vec3 interpNormal;
in vec2 fragTexCoords;

out vec4 fragColor;

uniform vec3 uLightDir = normalize(vec3(1.0));
uniform sampler2D uTexture;


void main() {
    vec3 normal = normalize(interpNormal);
    vec3 texColor = texture(uTexture, fragTexCoords).rgb;
    fragColor = vec4(texColor, 1.0);
}
