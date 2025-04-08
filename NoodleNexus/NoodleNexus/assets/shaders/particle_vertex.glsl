#version 430

// Vertex Attributes
in vec3 vPos;
in vec3 vNormal;
in vec2 vUV;
in vec3 vTangent;

#define MAX_PARTICLES 1024

// Define Particle struct outside the UBO
struct Particle {
    vec3 position;
    float padding1;
    vec3 velocity;
    float padding2;
    vec4 color;
    float size;
    float lifetime;
    vec2 padding3;
};

// UBO declaration
layout(std140, binding = 0) uniform ParticleData {
    Particle particles[MAX_PARTICLES];
};

// Outputs
out vec3 fWorldPos;
out vec3 fNormal;
out vec2 fUV;
out vec3 fTangent;
out vec3 fBitangent;
out vec4 fColor;
out float fLife;

// Uniforms
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;
uniform float time;

void main() {
    int particleID = gl_InstanceID;
    vec3 position = particles[particleID].position;
    float size = particles[particleID].size;
    fColor = particles[particleID].color;
    fLife = particles[particleID].lifetime;

    // Transform position (corrected formula)
    vec4 worldPos = matModel * vec4(vPos * size, 1.0);
    fWorldPos = worldPos.xyz;
    
    // Transform normals
    mat3 normalMatrix = mat3(transpose(inverse(matModel)));
    fNormal = normalize(normalMatrix * vNormal);
    fTangent = normalize(normalMatrix * vTangent);
    fBitangent = normalize(cross(fNormal, fTangent));
    
    // Final position (fixed matrix multiplication order)
    gl_Position = matProjection * matView * worldPos;
    fUV = vUV;
}