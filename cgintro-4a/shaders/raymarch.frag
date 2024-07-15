#version 330 core

in vec3 viewDir;
out vec3 fragColor;

uniform uint uView = 0u;
uniform vec3 uCameraPosition;
uniform float uFocalLength;
uniform float uTime;
uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uLightColor = vec3(1.0);
uniform float uNear = 0.01;
uniform float uFar = 100.0;
uniform float uSteps = 100;
uniform float uEpsilon = 0.0001;
uniform float uNormalEps = 0.0001;

const vec3 cSkyColor = vec3(0.1, 0.3, 0.6);
const vec3 cAmbient = 0.3 * cSkyColor;

/* Since OpenGL 4.1 this is IEEE Inf */
const float Inf = 1.0 / 0.0;

///////////////////////////////////////// procedural textures /////////////////////////////////////////

vec3 proceduralSky(vec3 rayDir) {
    return exp(-abs(rayDir.y) / cSkyColor);
}

vec3 proceduralSun(vec3 rayDir) {
    return pow(max(0.0, dot(rayDir, uLightDir)), 1000) * uLightColor;
}

float checkerPattern(vec3 pos) {
    vec3 p = floor(pos);
    return mod(p.x + p.y + p.z, 2.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//// signed distance functions by Inigo Quilez (https://iquilezles.org/articles/distfunctions/) ////

float sdBox(vec3 pos, vec3 size) {
  vec3 q = abs(pos) - size;
  return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sdEllipsoid(vec3 pos, vec3 radius) {
    float k0 = length(pos / radius);
    float k1 = length(pos / radius / radius);
    return k0 * (k0 - 1.0) / k1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

/* Returns the signed distance from pos to a bouncy ball */
float sdBouncyBall(vec3 pos, float t) {
    float tri = abs(fract(t) - 0.5) * 2.0; // Make a triangle wave from t
    float parabola = 1.0 - pow(tri, 2.5); // Make a parabola wave from the triangle wave
                                          // 2.0 is a realistic exponent, higher values appear cartoony
    float h = 2.0 * parabola; // Animate height h, 2.0 ist the maximum height
    float squish = 0.9 + parabola * 0.2; // Animate squish
    vec3 radius = vec3(0.25 / squish, 0.25 * squish, 0.25 / squish); // Scale the sphere
    vec3 center = vec3(0.0, 0.2 + radius.y + h, 0.0); // Position the sphere h units above the floor
    return sdEllipsoid(pos - center, radius);
}

const int SKY_ID = 0;
const int BOX_ID = 1;
const int BALL_ID = 2;
// TODO:: add another material

vec2 combine(vec2 result, float dist, int ID) {
    return dist < result.x ? vec2(dist, intBitsToFloat(ID)) : result;
}

/* Returns the signed distance from pos to the scene and the ID of the closest object */
vec2 sdScene(vec3 pos) {
    vec2 result = vec2(Inf, intBitsToFloat(SKY_ID));
    result = combine(result, sdBox(pos, vec3(2.0, 0.2, 2.0)), BOX_ID);
    result = combine(result, sdBouncyBall(pos, uTime), BALL_ID);
    // TODO:: add another object to the scene
    return result; // vec2(distance, ID)
}

/* Returns the color of the object with given ID at pos */
vec3 colorScene(vec3 pos, float ID) {
    switch (floatBitsToInt(ID)) {
        case BOX_ID:
            return vec3(0.6, 0.4, 0.2) * (checkerPattern(pos * 5.001) * 0.5 + 0.5);
        case BALL_ID:
            return vec3(1.0, 0.2, 0.2);
        // TODO:: add another material
        default:
            return vec3(1.0, 0.0, 1.0);
    }
}

/* Returns the normalized gradient of the signed distance field of our scene */
vec3 normalScene(vec3 pos) {
    // https://iquilezles.org/articles/normalsSDF/

    // vec2 h = vec2(uEpsilon, 0);
    // vec3 normal =  normalize(vec3(
    //     sdScene(pos + h.xyy).x - sdScene(pos - h.xyy).x,
    //     sdScene(pos + h.yxy).x - sdScene(pos - h.yxy).x,
    //     sdScene(pos + h.yyx).x - sdScene(pos - h.yyx).x
    // ));
    // return normal;

    float h = 0.0001;
    vec2 k = vec2(1, -1);
    return normalize( k.xyy*sdScene( pos + k.xyy*h ).x + 
                      k.yyx*sdScene( pos + k.yyx*h ).x + 
                      k.yxy*sdScene( pos + k.yxy*h ).x + 
                      k.xxx*sdScene( pos + k.xxx*h ).x );
}

/* Raymarch the scene, returns a vec3(depth, steps, ID) */
vec3 raymarchScene(vec3 rayOrigin, vec3 rayDir, float near, float far) {
    // Start at the near plane
    float depth = near;
    vec2 result; // vec2(distance, ID)
    // Advance depth until far plane is reached, steps are exhausted or we hit the scene
    for (float steps = 0.0; steps < uSteps; steps++) {
        // far plane exceeded?
        if (depth > far) {
            return vec3(Inf, steps, SKY_ID);
        }

        vec3 p = rayOrigin + depth * rayDir;

        result = sdScene(p);
        if (result.x <= uEpsilon) {
            return vec3(depth, steps, result.y);
        }
        // TODO: Return vec3(depth, steps, result.y), if we hit the scene
        depth += result.x; // Otherwise, advance depth by the distance to the scene
    }
    return vec3(depth, uSteps, result.y); // Steps are exhausted, return our approximation
}

void main() {
    // Generate camera ray
    vec3 rayDir = normalize(viewDir); // Renormalize after interpolation
    vec3 rayOrigin = uCameraPosition;

    // Raymarch the scene, result = vec3(depth, steps, ID)
    vec3 result = raymarchScene(rayOrigin, rayDir, uNear, uFar);

    if (isinf(result.x)) {
        // No hit, render a procedural sky background
        fragColor = proceduralSky(rayDir) + proceduralSun(rayDir);
    } else {
        // We hit something
        float depth = result.x;
        // Get the position where we hit the scene
        vec3 pos = rayOrigin + rayDir * depth;
        // The normal is the normalized gradient of the signed distance field
        vec3 normal = normalScene(pos);
        // Lambert lighting term
        vec3 lighting = max(dot(normal, uLightDir), 0.0) * uLightColor;
        // Test if something lies between the hit point and the light source
        float shadow = (isinf(raymarchScene(pos, uLightDir, uNear, uFar).x)) ? 1.0 : 0.0;
        // Get the color of the hit point
        vec3 albedo = colorScene(pos, result.z);
        // Calculate lighting
        // To simulate a soft ambient light from all directions, we add a little bit of the sky color to the light term
        fragColor = (shadow * lighting + cAmbient) * albedo;
        // Debug views
        if (uView == 1u) fragColor = normal * 0.5 + 0.5;
        if (uView == 2u) fragColor = vec3((depth - uNear) / (uFar - uNear));
    }
    if (uView == 3u) fragColor = vec3(result.y / uSteps);
}
