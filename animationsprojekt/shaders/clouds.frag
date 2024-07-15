#version 330 core

in vec3 viewDir;
out vec4 fragColor;

uniform vec3 uCameraPosition;
uniform float uEpsilon;

const float maxSteps = 100.0;

const int SKY_ID = 0;
const int CLOUD_ID = 1;
const float Inf = 1.0 / 0.0;

float sdSphere(vec3 position, vec3 sphereCenter, float radius) {
    return length(position - sphereCenter) - radius;
}

vec2 combine(vec2 result, float dist, int ID) {
    return dist < result.x ? vec2(dist, intBitsToFloat(ID)) : result;
}

/* Returns the signed distance from pos to the scene and the ID of the closest object */
vec2 sdScene(vec3 pos) {
    vec2 result = vec2(Inf, intBitsToFloat(SKY_ID));
    result = combine(result, sdSphere(pos, vec3(0.f, 0.f, -3.f), 0.5f), CLOUD_ID);
    return result; // vec2(distance, ID)
}


vec3 raymarchScene(vec3 rayOrigin, vec3 rayDir, float near, float far) {
    // Start at the near plane
    float depth = near;
    vec2 result; // vec2(distance, ID)
    // Advance depth until far plane is reached, steps are exhausted or we hit the scene
    for (float steps = 0.0; steps < maxSteps; steps++) {
        // far plane exceeded?
        if (depth > far) {
            return vec3(Inf, steps, SKY_ID);
        }

        vec3 p = rayOrigin + depth * rayDir;

        result = sdScene(p);
        if (result.x <= uEpsilon) { // scene hit?
            return vec3(depth, steps, result.y);
        }
        depth += result.x; // Otherwise, advance depth by the distance to the scene
    }
    return vec3(depth, maxSteps, result.y); // Steps are exhausted, return our approximation
}

void main() {
    // Generate camera ray
    vec3 rayDir = normalize(viewDir); // Renormalize after interpolation
    vec3 rayOrigin = uCameraPosition;

    // Raymarch the scene, result = vec3(depth, steps, ID)
    vec3 result = raymarchScene(rayOrigin, rayDir, 0.01, 100);

    if (!isinf(result.x)) {
        // We hit something
        float depth = result.x;
        // Get the position where we hit the scene
        vec3 pos = rayOrigin + rayDir * depth;

        fragColor = vec4(1.f,1.f,1.f, 0.9f);
    }
    fragColor = vec4(0, 0, 0, 0);
}
