#version 330

// Vertex Attributes
in vec3 vCol;
in vec3 vPos;
in vec3 vNormal;
in vec2 vUV;
in vec3 vTangent;

// Outputs to Fragment Shader
out vec3 fColour;
out vec4 fvertexWorldLocation;
out vec4 fvertexNormal;
out vec2 fUV;
out vec3 fTangent;
out vec3 fBitangent;
out mat4 fragViewMatrix;
out vec3 fPositionOffset; // Change due to shell extrusion (for fragment processing)

// Uniforms
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;
uniform float time;
uniform vec3 cameraLocation;
uniform float suckPower;
uniform float shakePower;


uniform bool bShellTexturing;  
uniform int shellLayer;  // Current shell layer index (0 - max)
uniform int shellCount;
uniform float verticalTightening;
uniform float verticalExponent;
uniform float shellLength;

// Function: Generate random noise for shake effect
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}
vec3 extrudeShellLayer(in vec3 pos, in vec3 normal, 
                       float shellIndex, float shellCount, float shellLength, 
                       float time, 
                       float minWindOffset, float maxWindOffset, 
                       float verticalTightening, // 0 = linear, 1 = fully compressed per verticalExponent
                       float verticalExponent,   // >1 compresses the top (e.g., 2.0 or 3.0)
                       float windExponent)       // Exponent for wind easing (e.g., 2.0)
{
    // 1. Compute normalized shell height (0 at bottom, 1 at top)
    float shellHeight = (shellIndex + 1.0) / shellCount;
    
    // 2. Compute an adjusted (effective) shell height.
    //    We blend the linear shellHeight with a compressed version (using pow) to keep the top layers closer.
    float effectiveHeight = mix(shellHeight, pow(shellHeight, verticalExponent), verticalTightening);

    // 3. Extrude the vertex along its normal using the effective height.
    vec3 updatedPos = pos + normalize(normal) * shellLength * effectiveHeight;
    
    // 4. Compute an easing factor for the wind effect.
    float easeFactor = pow(effectiveHeight, windExponent);
    
    // 5. Animate the wind: map sin(time) from [-1, 1] to [0, 1]
    float windNorm = (sin(time) + 1.0) / 2.0;
    // Interpolate between min and max wind offsets based on windNorm.
    float windOffset = easeFactor * mix(minWindOffset, maxWindOffset, windNorm);
    
    // 6. Apply the wind offset on the x-axis.
    updatedPos.x += windOffset;
    
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
    if (distanceToCamera < 60.0) {
        vec3 direction = normalize(cameraLocation - fvertexWorldLocation3);
        float power = suckPower / distanceToCamera;
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
        finalVert = extrudeShellLayer(finalVert, transformedNormal, shellLayer, shellCount, 0.02, time, 0.01, 0.02, verticalTightening, verticalExponent, 4);
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
