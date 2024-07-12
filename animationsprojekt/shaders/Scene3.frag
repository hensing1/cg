#version 330 core

in vec3 interpNormal;
in vec2 interpTexCoords;
in vec3 worldPos;

out vec3 fragColor;

uniform sampler2D holztexture;
uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uColor = vec3(1.0);
uniform vec3 uCameraPos;
uniform int uDebugView = 0;
uniform int uDistribution = 0;
uniform bool uUseOrenNayar = true;
uniform float uRoughness;
uniform float uMetallness;
uniform vec3 uAlbedo;
const float PI = 3.14159265359;

vec3 F_schlickApprox(float HdotV, vec3 R0) {
    return R0 + (1.0 - R0) * pow(1.0 - HdotV, 5.0);
}

float D_beckmannDistribution(float NdotH, float sigma2) {
float alpha = sigma2;
    float alpha2 = alpha * alpha;
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * alpha2 + (1.0 - NdotH2));
    denom = PI * denom * denom;

    float exponent = (NdotH2 - 1.0) / (NdotH2 * alpha2);

    return exp(exponent) / denom;
}

float G_geometricAttenuation(float NdotL, float NdotV, float NdotH, float HdotV) {
    float k = uRoughness / 2.0;
    float G1L = NdotL / (NdotL * (1.0 - k) + k);
    float G1V = NdotV / (NdotV * (1.0 - k) + k);
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

vec3 principledBRDF(float roughness, float metallness, vec3 albedo,  float NdotL, float NdotV, float NdotH, float HdotV, vec3 N, vec3 L, vec3 V) {
    float sigma = roughness * roughness;
    float sigma2 = sigma * sigma;

    // Most dielectrics have an IoR near 1.5 => R0 = ((1 - 1.5) / (1 + 1.5))^2 = 0.04, for conductors we use the albedo as R0
    vec3 R0 = mix(vec3(0.04), albedo, metallness);

    vec3 F = F_schlickApprox(HdotV, R0);
    float D, G;
        /* Beckmann distribution */
        D = D_beckmannDistribution(NdotH, sigma2);
        G = G_geometricAttenuation(NdotL, NdotV, NdotH, HdotV);

    vec3 specular = F * G * D / (4.0 * NdotV * NdotL);

    vec3 diffuse = albedo / PI; // Lambertian diffuse
    float orenNayar = orenNayarTerm(sigma2, NdotV, NdotL, N, L, V);
    if (uUseOrenNayar) diffuse *= orenNayar;
    diffuse *= (1.0 - F) * (1.0 - metallness); // diffuse is only applied to dielectrics


    return specular + diffuse;
}


void main() {
    vec3 Color = texture(holztexture,interpTexCoords).rgb;
    vec3 normal = normalize(interpNormal);
    vec3 lighting = Color * max(dot(normal, uLightDir), max(dot(normal, -uLightDir), 0.0));
    fragColor = lighting;
}
