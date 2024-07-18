#version 330 core

in vec3 viewDir;
out vec4 fragColor;

uniform float uTime;
// uniform float uEpsilon;

uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uCameraPosition;

uniform vec4 shadowColor = vec4(0, 0, 0, 0.3);
uniform vec3 uAtmosColor;

uniform float uEarthRadius;
uniform float uNear;
uniform float uFar;

uniform float uAlpha;
uniform float uBeta;
uniform float uGamma;

const float maxSteps = 100.0;

const int SKY_ID = 0;
const int CLOUD_ID = 1;
const int EARTH_ID = 2;
const float Inf = 1.0 / 0.0;

const float cloudMovementSpeed = 0.02;
const float animationLength = 16;

const float epsilon = 0.005;

vec4 proceduralSun(vec3 rayDir) {
    return pow(max(0.0, dot(rayDir, uLightDir)), 1000) * vec4(1);
}

vec2 combine(vec2 result, float dist, int ID) {
    return dist < result.x ? vec2(dist, intBitsToFloat(ID)) : result;
}

float sdSphere(vec3 position, vec3 sphereCenter, float radius) {
    return length(position - sphereCenter) - radius;
}

mat3 rotX(float alpha) {
    return mat3(
        1, 0, 0,
        0, cos(alpha), -sin(alpha),
        0, sin(alpha), cos(alpha)
    );
}

mat3 rotY(float alpha) {
    return mat3(
        cos(alpha), 0, sin(alpha),
        0, 1, 0,
        -sin(alpha), 0, cos(alpha)
    );
}

mat3 rotZ(float alpha) {
    return mat3(
        cos(alpha), -sin(alpha), 0,
        sin(alpha), cos(alpha), 0,
        0, 0, 1
    );
}

// alpha: Inklination der Umlaufbahn
// beta: Position auf Umlaufbahn
// gamma: Rotation der Wolke
float sdCloud(vec3 pos, float height, float alpha, float beta, float gamma, float scale) {
    vec3 posCenter = vec3(uEarthRadius + height, 0, 0);
    float offset = scale * 0.09;
    vec3 posLeft = rotY(offset) * posCenter;
    vec3 posRight = rotY(-offset) * posCenter;

    mat3 inclination = rotZ(alpha) * rotY(beta - cloudMovementSpeed * uTime) * rotX(gamma);
    posCenter = inclination * posCenter;
    posLeft = inclination * posLeft;
    posRight = inclination * posRight;

    float scaleL = 0.15 * scale * (1 + 0.2 * sin(0.5 * uTime + 0.1));
    float scaleC = 0.2 *  scale * (1 + 0.2 * sin(0.5 * uTime + 0.4));
    float scaleR = 0.15 * scale * (1 + 0.2 * sin(0.5 * uTime + 0.7));

    return min(sdSphere(pos, posLeft, scaleL), min(sdSphere(pos, posCenter, scaleC), sdSphere(pos, posRight, scaleR)));
}

/* Returns the signed distance from pos to the scene and the ID of the closest object */
vec2 sdScene(vec3 pos) {
    vec2 result = vec2(Inf, intBitsToFloat(SKY_ID));
    result = combine(result, sdCloud(pos, 0.5, 0, 0, 0, 1.2), CLOUD_ID);
    result = combine(result, sdCloud(pos, 0.7, 2, 4, 0, 0.9), CLOUD_ID);
    result = combine(result,
        sdCloud(pos, 0.15, -0.883, animationLength * cloudMovementSpeed + 6.181, 0, 0.5), CLOUD_ID); // diese Wolke ist über Deutschland
    result = combine(result, sdCloud(pos, 0.5, -0.52, 1.01, 0, 1.0), CLOUD_ID);
    result = combine(result, sdCloud(pos, 0.5, 1.2, 1.24, 0, 1.0), CLOUD_ID);
    result = combine(result, sdCloud(pos, 0.3, 1.07, 2.98, 1.5, 0.6), CLOUD_ID);
    result = combine(result, sdCloud(pos, 0.3, 0.1, 2.39, 1.28, 0.6), CLOUD_ID);
    return result; // vec2(distance, ID)
}

vec2 sdEarth(vec3 pos) {
    return vec2(sdSphere(pos, vec3(0), uEarthRadius + 0.03), intBitsToFloat(EARTH_ID));
}

vec3 raymarchScene(vec3 rayOrigin, vec3 rayDir, float near, float far, bool earth) {
    // Start at the near plane
    float depth = near;
    vec2 result; // vec2(distance, ID)
    // Advance depth until far plane is reached, steps are exhausted or we hit the scene
    for (float steps = 0.0; steps < maxSteps; steps++) {
        // far plane exceeded?
        if (depth > far) {
            return vec3(Inf, steps, intBitsToFloat(SKY_ID));
        }

        vec3 p = rayOrigin + depth * rayDir;

        result = earth ? sdEarth(p) : sdScene(p);
        if (result.x <= epsilon) { // scene hit?
            return vec3(depth, steps, result.y);
        }
        depth += result.x; // Otherwise, advance depth by the distance to the scene
    }
    return vec3(depth, maxSteps, result.y); // Steps are exhausted, return our approximation
}

/* Returns the normalized gradient of the signed distance field of our scene */
vec3 normalScene(vec3 pos) {
    // https://iquilezles.org/articles/normalsSDF/
    float h = 0.0001;
    vec2 k = vec2(1, -1);
    return normalize( k.xyy*sdScene( pos + k.xyy*h ).x + 
                      k.yyx*sdScene( pos + k.yyx*h ).x + 
                      k.yxy*sdScene( pos + k.yxy*h ).x + 
                      k.xxx*sdScene( pos + k.xxx*h ).x );
}

vec3 normalEarth(vec3 pos) {
    float h = 0.0001;
    vec2 k = vec2(1, -1);
    return normalize( k.xyy*sdEarth( pos + k.xyy*h ).x + 
                      k.yyx*sdEarth( pos + k.yyx*h ).x + 
                      k.yxy*sdEarth( pos + k.yxy*h ).x + 
                      k.xxx*sdEarth( pos + k.xxx*h ).x );
}

vec4 blend(vec4 source, vec4 destination) {
    vec4 s = vec4(source.a);
    vec4 d = vec4(1 - source.a);
    return s * source + d * destination;
}

void main() {
    // Generate camera ray
    vec3 rayDir = normalize(viewDir); // Renormalize after interpolation
    vec3 rayOrigin = uCameraPosition;

    vec4 color = vec4(0);
    vec4 border = vec4(0);
    bool isEarthShadow = false;

    // first: raymarch earth
    vec3 distToEarth = raymarchScene(rayOrigin, rayDir, uNear, uFar, true);

    // draw shadow
    if (!isinf(distToEarth.x)) {
        vec3 pos = rayOrigin + rayDir * distToEarth.x;
        if (!isinf(raymarchScene(pos, uLightDir, uNear, uFar, false).x) &&
            dot(normalEarth(pos), uLightDir) > 0) {
            color = shadowColor;
            isEarthShadow = true;
        }
    }

    // draw atmosphere
    color = blend(vec4(uAtmosColor, distToEarth.y / 40), color);
    
    // then: Raymarch the scene, result = vec3(depth, steps, ID)
    vec3 result = raymarchScene(rayOrigin, rayDir, uNear, min(uFar, distToEarth.x), false);

    if (!isinf(result.x)) {
        // We hit something
        float depth = result.x;
        // Get the position where we hit the scene
        vec3 pos = rayOrigin + rayDir * depth;
        // The normal is the normalized gradient of the signed distance field
        vec3 normal = normalScene(pos);
        // Lambert lighting term
        vec3 lighting = max(dot(normal, uLightDir), 0.2) * vec3(1);

        color = vec4(lighting, 0.5f);

    }

    if (isinf(result.x) && isinf(distToEarth.x)) {
        color = blend(proceduralSun(rayDir), color);
    }

    vec3 glowColor = vec3(0.5);
    float x = result.y / 50;
    vec4 glow = vec4(glowColor, 0.5 + 0.5 * tanh(10 * (x - 0.45)));
    
    // if (isEarthShadow && isinf(result.x)) {
        fragColor = blend(glow, color); 
    // }
    // else {
        // fragColor = glow + color;
    // }
}
