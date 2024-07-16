#version 330 core

in vec3 viewDir;
out vec4 fragColor;

uniform float uTime;
// uniform float uEpsilon;

uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uCameraPosition;

uniform float uEarthRadius;
uniform float uNear;
uniform float uFar;

const float maxSteps = 100.0;

const int SKY_ID = 0;
const int CLOUD_ID = 1;
const int EARTH_ID = 2;
const float Inf = 1.0 / 0.0;

const float epsilon = 0.005;

vec2 combine(vec2 result, float dist, int ID) {
    return dist < result.x ? vec2(dist, intBitsToFloat(ID)) : result;
}

float sdSphere(vec3 position, vec3 sphereCenter, float radius) {
    return length(position - sphereCenter) - radius;
}

// float sdCloud(float height, float lat, float lon) {
//     
// }

/* Returns the signed distance from pos to the scene and the ID of the closest object */
vec2 sdScene(vec3 pos) {
    vec2 result = vec2(Inf, intBitsToFloat(SKY_ID));
    result = combine(result, sdSphere(pos, vec3(0., 0., 2.5), 0.2), CLOUD_ID);
    result = combine(result, sdSphere(pos, vec3(-0.25, 0., 2.5), 0.12), CLOUD_ID);
    result = combine(result, sdSphere(pos, vec3(0.25, 0., 2.5), 0.12), CLOUD_ID);
    return result; // vec2(distance, ID)
}

vec2 sdEarth(vec3 pos) {
    return vec2(sdSphere(pos, vec3(0), uEarthRadius), intBitsToFloat(EARTH_ID));
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

void main() {
    // Generate camera ray
    vec3 rayDir = normalize(viewDir); // Renormalize after interpolation
    vec3 rayOrigin = uCameraPosition;

    // first: raymarch earth
    vec4 color = vec4(0);

    vec3 distToEarth = raymarchScene(rayOrigin, rayDir, uNear, uFar, true);
    
    // Raymarch the scene, result = vec3(depth, steps, ID)
    vec3 result = raymarchScene(rayOrigin, rayDir, uNear, min(uFar, distToEarth.x), false);
    vec4 border = vec4(0);

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

        // vec4 border = vec4(0, 0, 0, result.y / maxSteps);

        // fragColor = color + border;

        // float a = (uFar+uNear)/(uFar-uNear);
        // float b = 2.0*uFar*uNear/(uFar-uNear);
        //
        // vec4 vPos = uViewMatrix * vec4(pos, 1.0);
        // gl_FragDepth = a + b / vPos.z; // not works :(
        // 
        // float zc = ( uWorldToClip * vec4( pos, 1.0 ) ).z;
        // float wc = ( uWorldToClip * vec4( pos, 1.0 ) ).w;
        // gl_FragDepth = zc;
    }
    vec3 borderColor = vec3(0.5);
    float x = result.y / 50;
    border = vec4(borderColor, 0.5 + 0.5 * tanh(10 * (x - 0.5)));

    fragColor = color + border;
}
