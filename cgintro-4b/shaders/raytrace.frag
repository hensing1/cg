#version 330 core

in vec3 viewDir;
out vec3 fragColor;

const uint MAX_TRIANGLES = 100u;
uniform vec4 uVertices[MAX_TRIANGLES * 3u * 2u];
uniform uvec3 uIndices[MAX_TRIANGLES];
uniform uint uTriangleCount;

uniform vec3 uCameraPosition;

uniform uint uView = 0u;
uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uLightColor = vec3(1.0);
uniform float uNear = 0.1;
uniform float uFar = 100.0;

const float INF = 1.0 / 0.0;

vec3 proceduralSun(vec3 rayDir) {
    return pow(max(0.0, dot(rayDir, uLightDir)), 1000) * uLightColor;
}

/* Returns vec3(u, v, t) where u, v are the barycentric coordinates and t is the distance along the ray,
   or vec3(INF) if there is no intersection */
vec3 intersectTriangle(vec3 rayOrigin, vec3 rayDir, vec3 v0, vec3 v1, vec3 v2) {
    // TODO implement ray-triangle intersection test from slide 18 of cgintro-04-Raytracing 
    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;
    vec3 n = cross(e1, e2);
    float t = dot(n, v0 - rayOrigin) / dot(n, rayDir);
    if (t < uNear) return vec3(INF);

    vec3 p = rayOrigin + rayDir * t - v0;

    // Solve (a|b|n) x = p using Cramers rule
    // We utilize that the triple product dot(cross(a, b), n) = det(a|b|n) (https://en.wikipedia.org/wiki/Triple_product)
    float invDet = 1.0 / dot(n, n); // = 1.0 / dot(cross(e1, e2), n)
    float u = dot(cross(p, e2), n) * invDet;
    float v = dot(cross(e1, p), n) * invDet;
    if (u < 0.0 || v < 0.0 || u + v > 1.0) return vec3(INF);

    return vec3(u, v, t);
}

void main() {
    // Generate camera ray
    vec3 rayDir = normalize(viewDir); // Renormalize after interpolation
    vec3 rayOrigin = uCameraPosition;

    // Clear with background
    fragColor = proceduralSun(rayDir);
    float depth = uFar;

    // Loop through each triangle uIndices[i].xyz
    for (uint i = 0u; i < uTriangleCount; i++) {

        // Fetch vertex positions
        vec3 v0 = uVertices[uIndices[i].x * 2u].xyz;
        vec3 v1 = uVertices[uIndices[i].y * 2u].xyz;
        vec3 v2 = uVertices[uIndices[i].z * 2u].xyz;

        // Perform intersection test
        vec3 result = intersectTriangle(rayOrigin, rayDir, v0, v1, v2);

        // Overdraw if closer
        if (result.z < depth) {
            // Unpack result
            depth = result.z;
            vec3 barycentrics = vec3(1.0 - result.x - result.y, result.xy);

            // Fetch vertex normals
            vec3 n0 = uVertices[uIndices[i].x * 2u + 1u].yzw;
            vec3 n1 = uVertices[uIndices[i].y * 2u + 1u].yzw;
            vec3 n2 = uVertices[uIndices[i].z * 2u + 1u].yzw;

            // Interpolate normals
            vec3 normal = normalize(mat3(n0, n1, n2) * barycentrics);
            // = normalize(n0 * barycentrics.x + n1 * barycentrics.y + n2 * barycentrics.z);

            // Shade
            fragColor = max(dot(normal, uLightDir), 0.0) * uLightColor + vec3(0.005);

            // Debug views
            if (uView == 1u) fragColor = normal * 0.5 + 0.5;
            if (uView == 2u) fragColor = vec3(1.0 - (depth / uFar));
            if (uView == 3u) fragColor = barycentrics;
        }
    }
}