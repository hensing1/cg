#version 330 core

in vec3 interpNormal;
in vec2 texCoords; // Falls Texturkoordinaten verwendet werden

out vec3 fragColor;

uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uColor = vec3(1.0);
uniform sampler2DArray btfArray; // Die BTF-Textur als 2D-Array

uniform vec3 viewDir; // Blickrichtung
uniform vec3 lightDir; // Beleuchtungsrichtung
uniform int numViewDirections;
uniform int numLightDirections;

const float M_PI = 3.1415926535897932384626433832795;

int getDirectionIndex(vec3 dir, int numDirections) {
    // Beispielhafte Diskretisierung basierend auf einer festen Anzahl von Richtungen
    float phi = atan(dir.z, dir.x);
    float theta = acos(dir.y);

    int phiIndex = int((phi + M_PI) / (2.0 * M_PI) * numDirections);
    int thetaIndex = int(theta / M_PI * numDirections);

    return thetaIndex * numDirections + phiIndex;
}

void main() {
    vec3 normal = normalize(interpNormal);

    // Berechnung der Indizes basierend auf Blick- und Beleuchtungsrichtung
    int viewIndex = getDirectionIndex(viewDir, numViewDirections);
    int lightIndex = getDirectionIndex(lightDir, numLightDirections);

    // Wählen Sie die richtige BTF-Textur basierend auf den Indizes
    vec4 btfColor = texture(btfArray, vec3(texCoords, viewIndex + lightIndex * numViewDirections));

    fragColor = btfColor.rgb * uColor;
}