#version 330 core

in vec3 viewDir;
out vec3 fragColor;

uniform vec3 uCameraPosition;
uniform vec3 uLightDir;
uniform float uTime;
uniform float uNear;
uniform float uFar;

const vec3 cLightColor = vec3(255,255,112) / 255;
const vec3 cSkyColor = vec3(0.1, 0.3, 0.6);
const vec3 cAmbient = 0.3 * cSkyColor;
const vec3 cDirtColor = vec3(155, 118, 83) / 255;
const float cCloudHeight = 5;
const float cMaxSteps = 100;
const float epsilon = 0.001f;

const float inf = 1.0 / 0.0;

const float cloudPositions[50] = float[](
49	,33	,18	,-10	,-41	,-3 ,
-1	,25	,43	,-38	,39	,-30,
24	,-43	,49	,56	,7	,40 ,
59	,27	,-6	,24	,58	,29 ,
29	,-9	,-12	,-15	,18	,-37,
7	,27	,-49	,40	,32	,-25,
2	,-48	,-25	,36	,44	,43 ,
-3	,23	,-5	,-13	,30	,38 ,
1	,39
);
//
// float min(float a, float b) {
//     return a < b ? a : b;
// }

vec3 proceduralSun(vec3 rayDir) {
    return pow(max(0.0, dot(rayDir, uLightDir)), 1000) * cLightColor;
}

vec3 proceduralSky(vec3 rayDir) {
    return exp(-rayDir.y / cSkyColor);
}

vec3 proceduralDirt(vec3 rayDir) {
    return exp(rayDir.y / cDirtColor);
}

float sdSphere(vec3 position, vec3 sphereCenter, float radius) {
    return length(position - sphereCenter) - radius;
}

float sdCloud(vec3 rayPos, vec3 cloudPos, float radius) {
    vec3 posCenter = cloudPos;
    float offset = radius * 0.9;
    vec3 posLeft = vec3(offset, 0, 0) + posCenter;
    vec3 posRight = vec3(-offset, 0, 0) + posCenter;

    float scaleL = 0.75 * radius * (1 + 0.1 * sin(0.5 * uTime + 0.1));
    float scaleC =        radius * (1 + 0.1 * sin(0.5 * uTime + 0.4));
    float scaleR = 0.75 * radius * (1 + 0.1 * sin(0.5 * uTime + 0.7));

    return min(sdSphere(rayPos, posLeft, scaleL), min(sdSphere(rayPos, posCenter, scaleC), sdSphere(rayPos, posRight, scaleR)));
}

float sdScene(vec3 pos) {
    float result = inf;
    for (int i = 0; i < 25; i++) {
        result = min(result, sdCloud(pos, vec3(cloudPositions[2 * i], cCloudHeight, cloudPositions[2 * i + 1]), 1));
    }
    return result;
}

vec2 raymarchScene(vec3 rayOrigin, vec3 rayDir, float near, float far) {
    float depth = near;
    for (float steps = 0.0; steps < cMaxSteps; steps++) {
        if (depth > far) {
            return vec2(inf, steps);
        }

        vec3 p = rayOrigin + depth * rayDir;
    
        float result = sdScene(p);
        if (result <= epsilon) { // scene hit?
            return vec2(depth, steps);
        }
        depth += result; // Otherwise, advance depth by the distance to the scene
    }
    return vec2(depth, cMaxSteps); // Steps are exhausted, return our approximation
}

vec3 normalScene(vec3 pos) {
    // https://iquilezles.org/articles/normalsSDF/
    float h = 0.0001;
    vec2 k = vec2(1, -1);
    return normalize( k.xyy*sdScene( pos + k.xyy*h ) + 
                      k.yyx*sdScene( pos + k.yyx*h ) + 
                      k.yxy*sdScene( pos + k.yxy*h ) + 
                      k.xxx*sdScene( pos + k.xxx*h ) );
}

void main() {
    // Generate camera ray
    vec3 rayDir = normalize(viewDir); // Renormalize after interpolation
    vec3 rayOrigin = uCameraPosition;

    // Raymarch the scene, result = vec2(depth, steps)
    vec2 result = raymarchScene(rayOrigin, rayDir, 0.01, 100);

    if (isinf(result.x)) {
        fragColor = proceduralSun(rayDir) + (rayDir.y > 0 ? proceduralSky(rayDir) : proceduralDirt(rayDir));
    }
    else {
        // We hit something
        float depth = result.x;
        // Get the position where we hit the scene
        vec3 pos = rayOrigin + rayDir * depth;
        // The normal is the normalized gradient of the signed distance field
        vec3 normal = normalScene(pos);
        // Lambert lighting term
        vec3 lighting = max(dot(normal, uLightDir), 0.0) * cLightColor;
        // Test if something lies between the hit point and the light source
        float shadow = (isinf(raymarchScene(pos, uLightDir, 0.01, 100).x)) ? 1.0 : 0.0;
        // Calculate lighting
        // To simulate a soft ambient light from all directions, we add a little bit of the sky color to the light term
        fragColor = (shadow * lighting + cAmbient) * vec3(1);
    }
    // vec3 glowColor = vec3(0.5);
    // float x = result.y / 50;
    // float glow = 0.5 + 0.5 * tanh(10 * (x - 0.45));
    // 
    // fragColor = fragColor + glow * glowColor;

}
