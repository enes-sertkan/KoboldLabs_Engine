#version 410  

// Vertex Attributes
in vec3 vCol;
in vec3 vPos;
in vec3 vNormal;
in vec2 vUV;
in vec3 vTangent;


layout (location = 2) in float shellLayer;

// Outputs to Fragment Shader
out vec3 fColour;
out vec4 fvertexWorldLocation;
out vec4 fvertexNormal;
out vec2 fUV;
out vec3 fTangent;
out vec3 fBitangent;
out mat4 fragViewMatrix;
out vec3 fPositionOffset; // Change due to shell extrusion (for fragment processing)
out float fshellLayer;

// Uniforms
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;
uniform float time;
uniform vec3 cameraLocation;
uniform float suckPower;
uniform float shakePower;


uniform bool bShellTexturing;  
// Current shell layer index (0 - max)
uniform int shellCount;
uniform float verticalTightening;
uniform float verticalExponent;
uniform float shellLength;

struct sSTCollider {
    bool isOn;
    vec3 position;  // Now in world space!
    float radius;
    float blendingRadius;
};

uniform sSTCollider colliders[20];




// Function: Generate random noise for shake effect
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}
float hash(vec2 uv) {
    uint x = uint(uv.x * 250.0);  // Scale to avoid precision issues
    uint y = uint(uv.y * 250.0);
    uint seed = x * 513U + 101U * y;

    seed = (seed << 5U) ^ seed;
    seed = seed * 1664525U + 1013904223U;

    return float(seed) / 4294967295.0;
}

    vec3 extrudeShellLayer(vec3 pos, vec3 normal, 
                       float shellIndex, float shellCount, float shellLength, 
                       float time, 
                       float minWindOffset, float maxWindOffset, 
                       float verticalTightening, 
                       float verticalExponent,  
                       float windExponent)       
{
    float vertexRandomness = hash(pos.xz);

    // 1. Compute normalized shell height (0 at bottom, 1 at top)
    float shellHeight = (shellIndex + 1.0) / shellCount;

    // 2. Compute adjusted shell parameters
    float effectiveHeight = mix(shellHeight, pow(shellHeight, verticalExponent), verticalTightening);
    vec3 updatedPos = normalize(normal) * shellLength * effectiveHeight;
    
    // 3. Only process colliders on upper half of shells
    if (effectiveHeight > 0.5) {
        vec3 totalColliderOffset = vec3(0.0);
        float colliderPower = pow((effectiveHeight - 0.5) * 2.0, 8.0);
        
        // 4. Process all colliders
        for (int i = 0; i < 10; i++) {
            if (!colliders[i].isOn) continue;

            // Calculate XZ distance to collider
            vec2 colliderXZ = colliders[i].position.xz;
            vec2 posXZ = pos.xz;
            float d = distance(colliderXZ, posXZ);
            
            // Calculate falloff effect
            float radius = colliders[i].radius;
            float blendRange = colliders[i].blendingRadius;
            if (d>blendRange) continue;

            float effect = clamp(1.0 - d/blendRange, 0.0, 1.0);
            
            if (effect <= 0.0) continue;

            // Calculate direction vector
            vec3 dir = normalize(pos - colliders[i].position);
            dir.y = 0.0; // Flatten to XZ plane
            dir = normalize(dir);
            dir.xz *= vec2(1.3, 1.0); // Apply directional bias
            dir = normalize(dir);

            // Accumulate offset
            totalColliderOffset += dir * effect * colliderPower * 0.03;
        }
        
        // 5. Apply accumulated collider effect
        updatedPos += totalColliderOffset;
    }

    // 6. Wind effect (existing code)

    float windNorm = (sin(time + vertexRandomness * 3.14) + 1.0) / 2.0;
    float windOffset = pow(effectiveHeight, windExponent) * 
                      mix(minWindOffset, maxWindOffset, windNorm);
    updatedPos.x += windOffset * (0.8 + vertexRandomness * 0.4);

    return updatedPos;
}


void main() {
    vec3 finalVert = vPos;



    // Apply Shake Effect
    vec2 seed = finalVert.xy + vec2(time);
    float noise = rand(seed);
    float shakeAmount = noise * 10.0 - 5.0;
    finalVert += vec3(shakeAmount * shakePower);

    // Compute World Position
    fvertexWorldLocation = matModel * vec4(finalVert, 1.0);
    vec3 fvertexWorldLocation3 = fvertexWorldLocation.xyz;

    // Camera Attraction Effect (Suck Power)
   // float distanceToCamera = distance(fvertexWorldLocation3, cameraLocation);
  //  if (distanceToCamera < 0.0) {
   //     vec3 direction = normalize(cameraLocation - fvertexWorldLocation3);
   //     float power = 0.00 / distanceToCamera;
  //      power = min(power, distanceToCamera);
  //      vec3 change = direction * power;
   //     finalVert += change;
   //     fPositionOffset = change;  // Pass this to fragment shader for further effects
   // } else {
   //     fPositionOffset = vec3(0.0);
   // }

    // Compute Final Vertex Position in Screen Space
    mat4 matMVP = matProjection * matView * matModel;


    // Pass View Matrix
    fragViewMatrix = matView;

    // Compute Normal Matrix (Ignore Scale and Translation)
    mat3 normalMatrix = mat3(transpose(inverse(matModel)));
    vec3 transformedNormal = normalize(normalMatrix * vNormal);
    fvertexNormal = vec4(transformedNormal, 0.0);


        // Apply Shell Extrusion if enabled
    if (bShellTexturing) {
        finalVert = finalVert+ extrudeShellLayer(fvertexWorldLocation.xyz, transformedNormal, shellLayer, shellCount, shellLength, time, 0.01, 0.03, verticalTightening, verticalExponent, 4);
           fshellLayer = shellLayer;
    
    }

        gl_Position = matMVP * vec4(finalVert, 1.0);

    // Compute Tangent and Bitangent
    vec3 transformedTangent = normalize(normalMatrix * vTangent);
    vec3 transformedBitangent = normalize(cross(transformedNormal, transformedTangent));
    fTangent = transformedTangent;
    fBitangent = transformedBitangent;

    // Pass Additional Data
    fColour = vCol;
    fUV = vUV;
}
