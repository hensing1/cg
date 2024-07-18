// Geometry Pass Fragment Shader
#version 330 core
#extension GL_ARB_separate_shader_objects : enable
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = vec4(FragPos, 1.0);
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
}  