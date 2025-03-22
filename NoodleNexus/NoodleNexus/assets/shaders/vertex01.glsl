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
 

// Function: Generate random noise for shake effect
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

// Function: Extrude vertex along normal for shell effect
vec3 extrudeShellLayer(in vec3 pos, in vec3 normal, float shellIndex, float shellCount, float shellLength) {
float shellHeight = (shellLayer + 1.0) / shellCount; // Ensure extrusion starts from layer 1
return pos + normalize(normal) * shellLength * shellHeight;
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
        finalVert = extrudeShellLayer(finalVert, transformedNormal, shellLayer, 64, 0.012);
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
