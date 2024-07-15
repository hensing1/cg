#version 330 core

in vec3 interpNormal;
in vec2 interpTexCoords;
in vec3 worldPos;

out vec3 fragColor;

uniform sampler2D holztexture;
uniform vec3 uCameraPos;
uniform vec3 uLightDir;
vec3 L = normalize(uLightDir);
uniform bool uUseOrenNayar;
uniform float uRoughness;
uniform float uMetallness;
uniform bool useBlaetter;
const float PI = 3.14159265359;
const float EPSILON = 1e-5;

vec3 F_schlickApprox(float HdotV, vec3 R0) {
    return R0 + (1.0 - R0) * pow(1.0 - HdotV, 5.0);
}

float D_beckmannDistribution(float NdotH, float sigma2) {
    // Beckmann distribution function
    float alpha = sigma2;
    float alpha2 = alpha * alpha;
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * alpha2 + (1.0 - NdotH2));
    denom = PI * denom * denom + EPSILON; // Add epsilon to avoid division by zero

    float exponent = (NdotH2 - 1.0) / (NdotH2 * alpha2);

    return exp(exponent) / denom;
}

float G_geometricAttenuation(float NdotL, float NdotV) {
    // Geometric attenuation function
    float k = uRoughness / 2.0;
    float G1L = NdotL / (NdotL * (1.0 - k) + k); // Add epsilon to avoid division by zero
    float G1V = NdotV / (NdotV * (1.0 - k) + k); // Add epsilon to avoid division by zero
    return G1L * G1V;
}

float orenNayarTerm(float sigma2, float NdotV, float NdotL, vec3 N, vec3 L, vec3 V) {
   // Berechne A und B
    float sigma2_sq = sigma2 * sigma2;
    float A = 1.0 - (sigma2_sq / (2.0 * (sigma2_sq + 0.33)));
    float B = 0.45 * sigma2_sq / (sigma2_sq + 0.09);

    // Berechne die Richtungsvektoren relativ zur Normalen
    vec3 Ln = normalize(L - dot(L, N) * N);
    vec3 Vn = normalize(V - dot(V, N) * N);

    // Berechne die Winkel phiL und phiV
    float phiL = atan(Ln.y, Ln.x);
    float phiV = atan(Vn.y, Vn.x);
    float cosPhiDiff = cos(phiL - phiV);

    // Berechne die Winkel alpha und beta
    float alpha = max(acos(NdotL), acos(NdotV));
    float beta = min(acos(NdotL), acos(NdotV));

    // Berechne den Oren-Nayar Term
    float orenNayar = A + B * max(0.0, cosPhiDiff) * sin(alpha) * tan(beta);

    return orenNayar;
}

vec3 principledBRDF(vec3 N, vec3 L, vec3 V, vec3 H, float NdotL, float NdotV, float NdotH, float HdotV, vec3 baseColor, float roughness, float metallness) {

    // Calculate the Fresnel term using Schlick's approximation
    vec3 R0 = mix(vec3(0.04), baseColor, metallness);
    vec3 F = F_schlickApprox(HdotV, R0);

    // Calculate the Beckmann distribution term
    float D = D_beckmannDistribution(NdotH, roughness);

    // Calculate the geometric attenuation term
    float G = G_geometricAttenuation(NdotL, NdotV);

    // Calculate diffuse and specular components
    vec3 diffuse = baseColor / PI;
    vec3 specular = F * D * G / (4.0 * NdotL * NdotV + EPSILON); // Add epsilon to avoid division by zero


    return diffuse + specular;
}


void main() {
    vec3 N = normalize(interpNormal);
    vec3 V = normalize(uCameraPos - worldPos);
    vec3 L = normalize(uLightDir);
    vec3 H = normalize(L + V);
    vec3 baseColor;

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), EPSILON);
    float HdotV = max(dot(H, V), 0.0);


    baseColor = texture(holztexture, interpTexCoords).rgb;
    vec3 brdfColor = principledBRDF(N, L, V, H, NdotL, NdotV, NdotH, HdotV, baseColor, uRoughness, uMetallness);
    fragColor = brdfColor;
}

