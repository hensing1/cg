#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{    
    // Store the fragment position vector in the first G-buffer texture
    gPosition = vec4(FragPos, 1.0);
    // Also store the per-fragment normals into the G-buffer
    gNormal = normalize(Normal);
}