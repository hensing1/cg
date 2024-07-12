#version 330 core

in vec3 interpNormal;
in vec2 interpTexCoord;

out vec3 fragColor;

uniform sampler2D heightmap;

uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uColor = vec3(1.0);
uniform vec3 oceanColor;
uniform vec3 landColor;
uniform vec3 mountainColor;
uniform float landThreshold;

const float PI = 3.14159265359;

vec2 recalc_tex_coords(vec3 normal) {
    float latitude = asin(normal.y / length(normal));

    vec2 pointOnLatitudeCircle = vec2(normal.x, normal.z);
    float longitude = length(pointOnLatitudeCircle) == 0 ? 0
                      : normal.z >= 0
                          ? acos(normal.x / length(pointOnLatitudeCircle))
                          : -acos(normal.x / length(pointOnLatitudeCircle));

    return vec2(longitude / (2 * PI) + 0.5f, latitude / PI + 0.5f);
}

void main() {
    float height = texture(heightmap, recalc_tex_coords(interpNormal)).r;

    vec3 color = height <= landThreshold ?
        mix(oceanColor, landColor, height / landThreshold) : 
        mix(landColor, mountainColor, (height - landThreshold) / (1 - landThreshold));

    vec3 normal = normalize(interpNormal);
    vec3 lighting = color * max(dot(normal, uLightDir), 0.05);
    fragColor = lighting;
}
