#version 330 core

in vec3 interpNormal;
in vec2 interpTexCoords;
in vec3 worldPos;

out vec3 fragColor;

uniform sampler2D texNoise;
uniform sampler2D depthTexture;
uniform sampler2D uTexture;
uniform sampler2D ssaoTexture; // Add SSAO texture uniform
uniform vec3 uCameraPos;
uniform vec3 uLightPos;
uniform bool uUseOrenNayar;
uniform float uRoughness;
uniform float uMetallness;
uniform mat4 view;
uniform mat4 projection;

const int kernelSize = 64;
uniform vec3 samples[kernelSize];

uniform bool uUseTexture;
uniform vec3 uColor;

const float PI = 3.14159265359;
const float EPSILON = 1e-5;

vec3 L = normalize(uLightPos - worldPos);

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

float G_geometricAttenuation(float NdotL, float NdotV) {
    float k = uRoughness / 2.0;
    float G1L = NdotL / (NdotL * (1.0 - k) + k);
    float G1V = NdotV / (NdotV * (1.0 - k) + k);
    return G1L * G1V;
}

float orenNayarTerm(float sigma2, float NdotV, float NdotL, vec3 N, vec3 L, vec3 V) {
    float sigma2_sq = sigma2 * sigma2;
    float A = 1.0 - (sigma2_sq / (2.0 * (sigma2_sq + 0.33)));
    float B = 0.45 * sigma2_sq / (sigma2_sq + 0.09);

    vec3 Ln = normalize(L - dot(L, N) * N);
    vec3 Vn = normalize(V - dot(V, N) * N);

    float phiL = atan(Ln.y, Ln.x);
    float phiV = atan(Vn.y, Vn.x);
    float cosPhiDiff = cos(phiL - phiV);

    float alpha = max(acos(NdotL), acos(NdotV));
    float beta = min(acos(NdotL), acos(NdotV));

    float orenNayar = A + B * max(0.0, cosPhiDiff) * sin(alpha) * tan(beta);

    return orenNayar;
}

vec3 principledBRDF(vec3 N, vec3 L, vec3 V, vec3 H, float NdotL, float NdotV, float NdotH, float HdotV, vec3 baseColor, float roughness, float metallness) {
    vec3 R0 = mix(vec3(0.04), baseColor, metallness);
    vec3 F = F_schlickApprox(HdotV, R0);

    float D = D_beckmannDistribution(NdotH, roughness);
    float G = G_geometricAttenuation(NdotL, NdotV);

    vec3 diffuse = baseColor / PI;
    vec3 specular = F * D * G / (4.0 * NdotL * NdotV);

    return diffuse + specular;
}


void main() {
    vec3 N = normalize(interpNormal);
    vec3 V = normalize(uCameraPos - worldPos);
    vec3 H = normalize(L + V);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float HdotV = max(dot(H, V), 0.0);

    vec3 baseColor = uUseTexture ? texture(uTexture, interpTexCoords).rgb : uColor;
    vec3 brdfColor = principledBRDF(N, L, V, H, NdotL, NdotV, NdotH, HdotV, baseColor, uRoughness, uMetallness);
    
    // Fetch the SSAO value from the SSAO texture
    float ao = texture(ssaoTexture, interpTexCoords).r;
    
    // Ambient component
    vec3 ambient = 0.3 * brdfColor * ao;
    
    // Diffuse component
    vec3 diffuse = max(dot(N, L), 0.0) * brdfColor * vec3(1.0);
    
    // Specular component
    vec3 specular = vec3(0.0);
    if (NdotL > 0.0) {
        vec3 viewDir = normalize(-worldPos); // Assuming view position is at (0,0,0)
        vec3 reflectDir = reflect(-L, N);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        specular = vec3(1.0) * spec; // Assuming white specular light
    }
    
    // Final color calculation
    vec3 lighting = ambient + diffuse + specular;
    fragColor = lighting;
}
