#version 430
#extension GL_NV_uniform_buffer_std430_layout : enable
// ------ Input Attributes -----------
in vec3 vPos;
in vec3 vNormal;
in vec2 vUV;
in vec3 vTangent;
in vec3 vCol;            // Only used in mesh rendering branch.
layout (location = 2) in float shellLayer; // Only used in mesh branch.

// ------ Particle UBO ----------------
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
layout(std430, binding = 1) uniform ParticleData {
    Particle particles[MAX_PARTICLES];
};

// ------ Uniforms (Common) -----------
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;
uniform float time;
uniform bool isParticleEmitter;    // Selects between particle or mesh branch

// ------ Additional Uniforms for Mesh Branch ------
uniform vec3 cameraLocation;
uniform float suckPower;
uniform float shakePower;
uniform bool bShellTexturing;  
uniform int shellCount;
uniform float verticalTightening;
uniform float verticalExponent;
uniform float shellLength;

struct sSTCollider {
    bool isOn;
    vec3 position;
    float radius;
    float blendingRadius;
};
uniform sSTCollider colliders[20];

// ------ Outputs ----------------------
out vec4 fvertexWorldLocation; // Used in both branches (world position)
out vec4 fvertexNormal;        // Passed transformed normal (as vec4)
out vec2 fUV;
out vec3 fTangent;
out vec3 fBitangent;
out mat4 fragViewMatrix;       // For use in the fragment shader
out vec3 fPositionOffset;      // For mesh branch (e.g. camera attraction)
out float fshellLayer;         // Shell layer index (mesh branch)
out vec4  fColour;              // Color output (particle uses Particle.color, mesh uses vCol)
out float fLife;               // Particle lifetime (set to 0.0 for mesh)

// ------ Utility functions --------------
// Simple random/ hash functions for noise calculations.
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}
float hash(vec2 uv) {
    uint x = uint(uv.x * 250.0);
    uint y = uint(uv.y * 250.0);
    uint seed = x * 513U + 101U * y;
    seed = (seed << 5U) ^ seed;
    seed = seed * 1664525U + 1013904223U;
    return float(seed) / 4294967295.0;
}

// Extrude function for shell texturing, from your mesh shader.
vec3 extrudeShellLayer(vec3 pos, vec3 normal, 
                       float shellIndex, float shellCount, float shellLength, 
                       float time, 
                       float minWindOffset, float maxWindOffset, 
                       float verticalTightening, 
                       float verticalExponent,  
                       float windExponent)       
{
    float vertexRandomness = hash(pos.xz);
    float shellHeight = (shellIndex + 1.0) / shellCount;
    float effectiveHeight = mix(shellHeight, pow(shellHeight, verticalExponent), verticalTightening);
    vec3 updatedPos = normalize(normal) * shellLength * effectiveHeight;
    
    if (effectiveHeight > 0.5) {
        vec3 totalColliderOffset = vec3(0.0);
        float colliderPower = pow((effectiveHeight - 0.5) * 2.0, 8.0);
        for (int i = 0; i < 10; i++) {
            if (!colliders[i].isOn) continue;
            vec2 colliderXZ = colliders[i].position.xz;
            vec2 posXZ = pos.xz;
            float d = distance(colliderXZ, posXZ);
            float blendRange = colliders[i].blendingRadius;
            if (d > blendRange) continue;
            float effect = clamp(1.0 - d/blendRange, 0.0, 1.0);
            if (effect <= 0.0) continue;
            vec3 dir = normalize(pos - colliders[i].position);
            dir.y = 0.0;
            dir = normalize(dir);
            dir.xz *= vec2(1.3, 1.0);
            dir = normalize(dir);
            totalColliderOffset += dir * effect * colliderPower * 0.03;
        }
        updatedPos += totalColliderOffset;
    }
    float windNorm = (sin(time + vertexRandomness * 3.14) + 1.0) / 2.0;
    float windOffset = pow(effectiveHeight, windExponent) * mix(minWindOffset, maxWindOffset, windNorm);
    updatedPos.x += windOffset * (0.8 + vertexRandomness * 0.4);
    return updatedPos;
}

// ------ Main Shader Function --------------
void main() {
    if (isParticleEmitter) {
        // ----- Particle Branch -----
        int particleID = gl_InstanceID;
        // Retrieve particle-specific data.
        vec3 particlePos = particles[particleID].position;
        float particleSize = particles[particleID].size;
        fColour = particles[particleID].color;
        fLife = particles[particleID].lifetime;
        
        // Scale the input vertex positions by the particle's size.
        vec4 worldPos = matModel * vec4(vPos * particleSize+particlePos, 1.0);
        fvertexWorldLocation = worldPos;
        
        // Transform normals and tangents.
        mat3 normalMatrix = mat3(transpose(inverse(matModel)));
        vec3 transformedNormal = normalize(normalMatrix * vNormal);
        fvertexNormal = vec4(transformedNormal, 0.0);
        fTangent = normalize(normalMatrix * vTangent);
        fBitangent = normalize(cross(transformedNormal, fTangent));
        fUV = vUV;
        fragViewMatrix = matView;
        
        // Compute clip-space position.
        gl_Position = matProjection * matView  * vec4(vPos * particleSize+particlePos, 1.0);
        
        // Not used in particle branch.
        fshellLayer = 0.0;
        fPositionOffset = vec3(0.0);
    } else {
        // ----- Mesh (Non-Particle) Branch -----
        vec3 finalVert = vPos;
        
        // Optional: Apply shake effect.
        vec2 seed = finalVert.xy + vec2(time);
        float noise = rand(seed);
        float shakeAmount = noise * 10.0 - 5.0;
        finalVert += vec3(shakeAmount * shakePower);
        
        // Compute world position.
        fvertexWorldLocation = matModel * vec4(finalVert, 1.0);
        vec3 worldPos3 = fvertexWorldLocation.xyz;
        
        // (Optional) You can add other effects such as camera attraction here.
        fPositionOffset = vec3(0.0);
        fragViewMatrix = matView;
        
        // Normal and tangent transformations.
        mat3 normalMatrix = mat3(transpose(inverse(matModel)));
        vec3 transformedNormal = normalize(normalMatrix * vNormal);
        fvertexNormal = vec4(transformedNormal, 0.0);
        vec3 transformedTangent = normalize(normalMatrix * vTangent);
        vec3 transformedBitangent = normalize(cross(transformedNormal, transformedTangent));
        fTangent = transformedTangent;
        fBitangent = transformedBitangent;
        
        // Shell extrusion if enabled.
        if (bShellTexturing) {
            finalVert = finalVert + extrudeShellLayer(fvertexWorldLocation.xyz, transformedNormal, shellLayer, float(shellCount), shellLength, time, 0.01, 0.03, verticalTightening, verticalExponent, 4.0);
            fshellLayer = shellLayer;
        } else {
            fshellLayer = 0.0;
        }
        
        // Final transformation.
        mat4 matMVP = matProjection * matView * matModel;
        gl_Position = matMVP * vec4(finalVert, 1.0);
        
        // Pass through vertex color.
        if (!isParticleEmitter) {
        fColour.rgb = vCol;
        fColour.a = 1;
        }
        fUV = vUV;
        fLife = 0.0;  // Not used for mesh.
    }
}
