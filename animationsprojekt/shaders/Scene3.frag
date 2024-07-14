#version 330 core

in vec3 interpNormal;
in vec2 interpTexCoords;
in vec3 worldPos;

out vec3 fragColor;

uniform sampler2D holztexture;
vec3 L = normalize(vec3(1.0, 0.0, 0.0));
uniform vec3 uColor = vec3(1.0);
uniform vec3 uCameraPos;
uniform vec3 uLightDir;
uniform int uDebugView = 0;
uniform int uDistribution = 0;
uniform bool uUseOrenNayar = true;
uniform float uRoughness;
uniform float uMetallness;
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

vec3 principledBRDF(vec3 N, vec3 L, vec3 V, vec3 H, float NdotL, float NdotV, float NdotH, float HdotV, vec3 baseColor, float roughness, float metallness) {
    vec3 R0 = mix(vec3(0.04), baseColor, metallness);

    vec3 F = F_schlickApprox(HdotV, R0);
    float D = D_beckmannDistribution(NdotH, roughness);
    float G = G_geometricAttenuation(NdotL, NdotV, NdotH, HdotV);

    vec3 diffuse = vec3(0.0);
    if (uUseOrenNayar) {
        diffuse = orenNayarTerm(roughness, NdotV, NdotL, N, L, V) * baseColor / PI;
    } else {
        diffuse = baseColor / PI;
    }

    vec3 specular = F * G * D / (4.0 * NdotL * NdotV + 0.001);

    return diffuse + specular;
}


void main() {
    vec3 N = normalize(interpNormal);
    vec3 V = normalize(uCameraPos - worldPos);
    vec3 H = normalize(L + V);
    vec3 F0 = vec3(0.04);
     if (dot(N, V) < 0.0) {
        N = -N;
    }
    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float HdotV = max(dot(H, V), 0.0);
    vec3 texture = texture(holztexture,interpTexCoords).rgb;
    vec3 normal = normalize(interpNormal);
    vec3 baseColor = texture * max(dot(normal, uLightDir), max(dot(normal, -uLightDir), 0.0));

    vec3 brdfColor = principledBRDF(N, L, V, H, NdotL, NdotV, NdotH, HdotV,baseColor,uRoughness,uMetallness);


    fragColor = brdfColor; // Combine BRDF result with ambient light
}
