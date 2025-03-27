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
    uint x = uint(uv.x * 1000.0);  // Scale to avoid precision issues
    uint y = uint(uv.y * 1000.0);
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
    // 1. Compute normalized shell height (0 at bottom, 1 at top)
    float shellHeight = (shellIndex + 1.0) / shellCount;

    // 2. Compute an adjusted (effective) shell height.
    float effectiveHeight = mix(shellHeight, pow(shellHeight, verticalExponent), verticalTightening);

    // 3. Extrude the vertex along its normal using the effective height.
    vec3 updatedPos = normalize(normal) * shellLength * effectiveHeight;

    // 4. Compute an easing factor for the wind effect.
    float easeFactor = pow(effectiveHeight, windExponent);
    
    // 5. Introduce per-vertex randomness using the hash function
    float vertexRandomness = hash(pos.xz);
    
    // 6. Wind effect with per-vertex phase shift
    float windNorm = (sin(time + vertexRandomness * 3.14) + 1.0) / 2.0;
    float windOffset = easeFactor * mix(minWindOffset, maxWindOffset, windNorm);
    
    // 7. Process colliders in world space.
    float maxEffect = 0.0;
    vec3 bestDir = vec3(0.0);
    
    for (int i = 0; i < 20; i++) {
        if (!colliders[i].isOn)
            continue;

        float d = length(pos - colliders[i].position);
        float effect = 0.0;

        if (d < colliders[i].radius) {
            effect = 1.0;
        } else if (d < colliders[i].blendingRadius) {
            effect = 1.0 - (d - colliders[i].radius) / (colliders[i].blendingRadius - colliders[i].radius);
        }

        if (effect > maxEffect) {
            maxEffect = effect;
            bestDir = normalize(pos - colliders[i].position);
        }
    }
 
    maxEffect = length(pos.xz - colliders[0].position.xz);
   maxEffect=maxEffect;
  // if (maxEffect<0.1)
  // maxEffect = -1;

    bestDir = normalize(colliders[0].position-pos);
         bestDir.y*=0;
        bestDir.x*=1.3;

        // if ((length(bestDir) < 0.001))
    //     bestDir.x+=0.15;
       
       
       bestDir = normalize(  bestDir);

    


     if (effectiveHeight>0.5)
   {
     float colliderPower =  pow((effectiveHeight-0.5)*2, 8);
   
    vec3 colliderOffset = bestDir * 0.03* colliderPower/maxEffect;
    colliderOffset.y = 0;

    // Apply collider effect with random variation
    if (maxEffect < 3.0)
     { updatedPos -= colliderOffset;  // Add randomness

      if (maxEffect < 0.5)
        updatedPos -= normalize(normal) * shellLength * shellHeight*0.2*(0.5-maxEffect);
      //    updatedPos.y -= 0.007*effectiveHeight/maxEffect;
       }
  // else if (maxEffect < 15.0) 
   //   updatedPos = bestDir * 0.2 * effectiveHeight/maxEffect;

    // Apply wind effect with per-vertex variation

 
}


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
    float distanceToCamera = distance(fvertexWorldLocation3, cameraLocation);
    if (distanceToCamera < 0.0) {
        vec3 direction = normalize(cameraLocation - fvertexWorldLocation3);
        float power = 0.00 / distanceToCamera;
        power = min(power, distanceToCamera);
        vec3 change = direction * power;
        finalVert += change;
        fPositionOffset = change;  // Pass this to fragment shader for further effects
    } else {
        fPositionOffset = vec3(0.0);
    }

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
        finalVert = finalVert+ extrudeShellLayer(fvertexWorldLocation.xyz, transformedNormal, shellLayer, shellCount, shellLength, time, 0.01, 0.015, verticalTightening, verticalExponent, 4);
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
