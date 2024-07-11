#version 330 core

in vec3 interpNormal;
in vec3 interpPosition;

out vec3 fragColor;

uniform sampler2D hullin;

uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uColor = vec3(1.0);

void main() {
    vec4 firstData;
    if (abs(interpPosition.y - 5) < 20.5)
        firstData = texture(hullin, interpPosition.xy);
    else
        firstData = vec4(0.0f);


    vec3 normal = normalize(interpNormal);
    vec3 lighting = uColor * max(dot(normal, uLightDir), max(dot(normal, -uLightDir), 0.0));
    fragColor = (firstData.w != 0.0f) ? lighting + firstData.xyz : lighting;
}
