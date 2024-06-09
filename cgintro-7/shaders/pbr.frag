#version 330 core

in vec3 interpNormal;
in vec3 worldPos;

out vec3 fragColor;

uniform vec3 uLightDir = normalize(vec3(1.0, 1.0, 1.0));
uniform vec3 uCameraPos;
uniform int uDebugView = 0;
uniform int uDistribution = 0;
uniform bool uUseOrenNayar = true;

uniform float uRoughness;
uniform float uMetallness;
uniform vec3 uAlbedo;

const float PI = 3.14159265359;

/**
 * Schlick's approximation for the Fresnel term (see https://en.wikipedia.org/wiki/Schlick%27s_approximation).
 * The Fresnel term describes how light is reflected at the surface.
 * For conductors the reflection coefficient R0 is chromatic, for dielectrics it is achromatic.
 * R0 = ((n1 - n2) / (n1 + n2))^2 with n1, n2 being the refractive indices of the two materials.
 * We can set n1 = 1.0 (air) and n2 = IoR of the material.
 * Most dielectrics have an IoR near 1.5 => R0 = ((1 - 1.5) / (1 + 1.5))^2 = 0.04.
 */
vec3 F_schlickApprox(float HdotV, vec3 R0) {
    // TODO
    return vec3(1.0);
}

/**
 * Normal distribution function from the Cook-Torrance model (beckmann distribution)
 * See: The Scattering of Electromagnetic Waves from Rough Surfaces by Petr Beckmann and André Spizzichino
 * See: A Reflectance Model for Computer Graphics by Robert L. Cook and Kenneth E. Torrance
 */
float D_beckmannDistribution(float NdotH, float sigma2) {
    // TODO
    return 1.0;
}

/**
 * Geometric attenuation term from the Cook-Torrance model
 * See: A Reflectance Model for Computer Graphics by Robert L. Cook and Kenneth E. Torrance
 */
float G_geometricAttenuation(float NdotL, float NdotV, float NdotH, float HdotV) {
    // TODO
    return 1.0;
}

/**
 * Oren-Nayar reflectance model without the lambertian term (A + B * max(0, cos(phi_L - phi_V)) * sin(alpha) * tan(beta)).
 */
float orenNayarTerm(float sigma2, float NdotV, float NdotL, vec3 N, vec3 L, vec3 V) {
    // TODO
    return 1.0;
}

/**
 * Trowbridge-Reitz normal distribution function
 * See: PBR Book by Matt Pharr and Greg Humphreys
 * See: Physically Based Shading at Disney by Brent Burley
 * See: Microfacet Models for Refraction through Rough Surfaces by Bruce Walter et al.
 */
float D_TrowbridgeReitz(float NdotH, float sigma2) {
    // BONUS
    return 1.0;
}

/**
 * Lambda for the Trowbridge-Reitz NDF
 * Measures invisible masked microfacet area per visible microfacet area.
 */
float Lambda_TrowbridgeReitz(float NdotV, float sigma2) {
    // BONUS
    return 1.0;
}

/**
 * Smith's shadowing-masking function for the Trowbridge-Reitz NDF.
 */
float G_TrowbridgeReitz(float NdotL, float NdotV, float sigma2) {
    // BONUS
    return 1.0;
}

/* Debug colors: pink = nan, red = inf, blue = negative, green = greater than 1 */
vec3 debugColor(vec3 x) {
    if (any(isnan(x))) return vec3(1.0, 0.0, 1.0);
    if (any(isinf(x))) return vec3(1.0, 0.0, 0.0);
    if (any(lessThan(x, vec3(0.0)))) return vec3(0.0, 0.0, 1.0);
    if (any(greaterThan(x, vec3(1.0)))) return vec3(0.0, 1.0, 0.0);
    return x;
}

vec3 principledBRDF(float roughness, float metallness, vec3 albedo,  float NdotL, float NdotV, float NdotH, float HdotV, vec3 N, vec3 L, vec3 V) {
    float sigma = roughness * roughness;
    float sigma2 = sigma * sigma;

    // Most dielectrics have an IoR near 1.5 => R0 = ((1 - 1.5) / (1 + 1.5))^2 = 0.04, for conductors we use the albedo as R0
    vec3 R0 = mix(vec3(0.04), albedo, metallness);

    vec3 F = F_schlickApprox(HdotV, R0);
    float D, G;
    if (uDistribution == 0) {
        /* Beckmann distribution */
        D = D_beckmannDistribution(NdotH, sigma2);
        G = G_geometricAttenuation(NdotL, NdotV, NdotH, HdotV);
    } else {
        /* Trowbridge-Reitz (GGX) */
        D = D_TrowbridgeReitz(NdotH, sigma2);
        G = G_TrowbridgeReitz(NdotL, NdotV, sigma2);
    }

    vec3 specular = F * G * D / (4.0 * NdotV * NdotL);

    vec3 diffuse = albedo / PI; // Lambertian diffuse
    float orenNayar = orenNayarTerm(sigma2, NdotV, NdotL, N, L, V);
    if (uUseOrenNayar) diffuse *= orenNayar;
    diffuse *= (1.0 - F) * (1.0 - metallness); // diffuse is only applied to dielectrics

    /* Debug views */
    switch (uDebugView) {
        case 1: fragColor = debugColor(vec3(D)); break;
        case 2: fragColor = debugColor(F); break;
        case 3: fragColor = debugColor(vec3(G)); break;
        case 4: fragColor = debugColor(specular); break;
        case 5: fragColor = debugColor(diffuse); break;
        case 6: fragColor = debugColor(vec3(orenNayar / PI)); break;
    }

    return specular + diffuse;
}

void main() {
    /* Surface normal in world space */
    vec3 N = normalize(interpNormal);
    /* Normalized light direction in world space */
    vec3 L = uLightDir;
    /* Normalized view direction in world space */
    vec3 V = normalize(uCameraPos - worldPos);
    /* Normalized half vector in world space */
    /* The half vector is half way between the light and view direction and is thus the direction of perfect reflection */
    vec3 H = normalize(V + L);

    float NdotL = dot(N, L);
    float NdotV = dot(N, V);
    float NdotH = dot(N, H);
    float HdotV = dot(H, V);

    vec3 lighting = principledBRDF(uRoughness, uMetallness, uAlbedo, NdotL, NdotV, NdotH, HdotV, N, L, V) * max(NdotL, 0.0);

    /* Debug views */
    switch (uDebugView) {
        case 0: fragColor = lighting; break;
        case 7: fragColor = 0.5 * N + 0.5; break;
        case 8: fragColor = 0.5 * V + 0.5; break;
        case 9: fragColor = 0.5 * H + 0.5; break;
        case 10: fragColor = debugColor(vec3(NdotL)); break;
        case 11: fragColor = debugColor(vec3(NdotV)); break;
        case 12: fragColor = debugColor(vec3(NdotH)); break;
        case 13: fragColor = debugColor(vec3(HdotV)); break;
        case 14: fragColor = worldPos; break;
    }
}