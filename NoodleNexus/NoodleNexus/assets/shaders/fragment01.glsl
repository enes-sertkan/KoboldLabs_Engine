#version 410  
// (Pixel) Fragment Shader


#define PI 3.14159265359

// === Input Attributes (from Vertex Shader) ===
in vec4 fColour;              // Base colour (from vertex buffer or particle color)
in vec4 fvertexWorldLocation; // Vertex world position
in vec4 fvertexNormal;        // Vertex normal in world space
in vec2 fUV;                  // Texture coordinates (UV)
in vec3 fTangent;             // Tangent in world space
in vec3 fBitangent;           // Bitangent in world space
in float fLife;               // Particle lifetime (0.0 for meshes)


// === Particle-Specific Uniforms ===
uniform bool isParticleEmitter;  // True if rendering particles



uniform sampler2D depthTexture;  
uniform bool bDepth;  


// === Uniforms for Object & Lighting ===
uniform vec4 objectColour;    // Override colour 
uniform bool bUseObjectColour;
uniform vec4 eyeLocation;     // Camera (eye) location
uniform bool bDoNotLight;     // If true, skip lighting
uniform bool bNightMode;      // Night mode flag
uniform float wholeObjectTransparencyAlpha; // Alpha channel for entire object




// === Output ===
out vec4 finalPixelColour;    // Final fragment colour (RGBA)

// === Light Types ===
const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

// === Light Structure ===
struct sLight {
    vec4 position;         // Light position (for point/spot)
    vec4 diffuse;          // Diffuse colour
    vec4 specular;         // Specular colour (rgb = highlight, w = power)
    vec4 atten;            // Attenuation: x = constant, y = linear, z = quadratic, w = cutoff distance
    vec4 direction;        // Light direction (for spot/directional)
    vec4 param1;           // x = light type, y = inner cone angle, z = outer cone angle, w = TBD
    vec4 param2;           // x = on/off flag, yzw = TBD
};

struct sWave {
    vec4 data;  // x = uv.x, y = uv.y, z = active flag, w = time
};

const int NUMBEROFLIGHTS = 40;
uniform sLight theLights[NUMBEROFLIGHTS];

// === Function Prototype for PBR Lighting ===
vec4 calculateLightContrib(vec3 vertexMaterialColor, vec3 vertexNormal, vec3 vertexWorldPos, 
                           vec4 vertexSpecular, float roughness, float metallic, vec3 F0);

// === Texture Samplers ===
uniform sampler2D texture00;
uniform sampler2D texture01;
uniform sampler2D texture02;
uniform sampler2D texture03;
uniform sampler2D textureAO;         // Ambient Occlusion
uniform bool useAO;
uniform sampler2D textureST;         // Metallic & Smoothness texture
uniform bool useST;
uniform sampler2D textureNM;         // Normal map
uniform bool useNM;




uniform bool bShellTexturing;  
in float fshellLayer;
uniform int shellCount;
uniform float verticalTightening;
uniform float verticalExponent;




struct sSTCollider {
    bool isOn;
    vec3 position;  // Now in world space!
    float radius;
    float blendingRadius;
};

uniform sSTCollider colliders[20];


// === Material Uniforms ===
uniform float metallic;              // Default metallic value
uniform float smoothness;            // Default smoothness value (if no texture)

// === Texture Blend Ratios ===
uniform vec4 texRatio_0_to_3;        // Blend ratios for texture layers
uniform bool bUseTextureAsColour;    // If true, use textures for colour

// === Skybox / Stencil ===
uniform samplerCube skyBoxTextureSampler;
uniform bool bIsSkyBoxObject;
uniform sampler2D stencilTexture;
uniform bool bUseStencilTexture;

// === Time / Animation Uniforms ===
uniform float time;       // Current time in seconds
uniform float speedX;     // UV speed X
uniform float speedY;     // UV speed Y
uniform float zoomPower;  // Zoom effect on texture
uniform float chromaticPower; // Chromatic Abberation effect on textures

// === Wave Uniforms ===
uniform sWave waves[10];
// Hash function (UV-based)
float hash(vec2 uv) {
    // Scale UV and convert to uint
    uint x = uint(uv.x);
    uint y = uint(uv.y);
    uint seed = x*513U + 101U * y;  // Use a prime number multiplier to mix

    // Simple bit-mixing with smaller constants
    seed = (seed << 5U) ^ seed;
    seed = seed * 1664525U + 1013904223U;  // Constants from a common LCG

    return float(seed) / 4294967295.0;  // Normalize to [0, 1]
}
// Compute shell color with randomness based on the hash value (no need for randomValue function)
vec3 computeShellColor(in vec2 uv, in float shellHeight, in sampler2D baseTex, in sampler2D shellTex, float uvOffsetFactor) {
    uv = uv*500.f;


        for (int i = 0; i < 20; i++) {
        if (!colliders[i].isOn)
            continue;

        float   maxEffect = length(fvertexWorldLocation.xz - colliders[i].position.xz);

         if (maxEffect<colliders[i].radius/2)
            discard;
      }
  


     //   if (maxEffect<0.5)
     //   shellHeight*=0.1;

    vec2 localUV = fract(uv) * 2.0 - 1.0;
    
    // Calculate distance from center
    float localDistance = length(localUV);

    // Generate randomness based on the hash function (UV-based)
    float rnd = hash(uv); // Get random float between 0 and 1 using hash function

    float _Thickness = 1.f;
    float _TaperPower = 3.5f;

    float thicknessThreshold = _Thickness * (rnd - pow(shellHeight, _TaperPower));

      if(localDistance > thicknessThreshold) {
       discard;
    }

    if (rnd < shellHeight) discard;  // You can adjust the threshold based on your effect

    // Sample base color (or use any texture for base color)
    vec3 baseColor = vec3(0.2, 0.8, 0.2) * 0.1f;

    vec3 shellColor = vec3(0.2, 0.8, 0.2) * shellHeight/rnd;  // Placeholder; replace with shell texture if needed
    
    // Blend the base color with the shell color based on shell height
    float blendFactor = shellHeight; // Simple linear blending
    return mix(baseColor, shellColor, 1.f);
}







// === Utility Function: Composite Two Colours ===
vec4 compositeOver(vec4 bottom, vec4 top) {
    if (all(equal(top.rgb, vec3(0.0)))) {
        return bottom;
    }
    vec3 outColor = top.rgb * top.a + bottom.rgb * (1.0 - top.a);
    float outAlpha = top.a + bottom.a * (1.0 - top.a);
    return vec4(outColor, outAlpha);
}



// === Normal Mapping Helper ===


bool IsValidTBN(vec3 tangent, vec3 bitangent, vec3 normal) {
    // Check for non-zero vectors
    float tangentLen = length(tangent);
    float bitangentLen = length(bitangent);
    if (tangentLen < 0.01 || bitangentLen < 0.01) return false;
    
    // Check orthogonality to normal (dot product should be ~0)
    float tangentDot = abs(dot(normalize(tangent), normalize(normal)));
    float bitangentDot = abs(dot(normalize(bitangent), normalize(normal)));
    if (tangentDot > 0.1 || bitangentDot > 0.1) return false;
    
    // Check angle between tangent and bitangent (~90 degrees)
    float tbnAngle = degrees(acos(dot(normalize(tangent), normalize(bitangent))));
    if (abs(tbnAngle - 90.0) > 15.0) return false;
    
    return true;
}


vec3 getTBNNormal(vec3 fNormal, vec3 fTangent, vec3 fBitangent, sampler2D normalMap, vec2 uv) {
if (!IsValidTBN(fTangent, fBitangent, fNormal)) {
        return normalize(fNormal); // Fallback to vertex normal
    }
    // Sample normal map (values in [0,1])
    vec3 normalMapSample = texture(normalMap, uv).rgb;
    // Remap to [-1, 1]
    normalMapSample = normalMapSample * 2.0 - 1.0;
    // Construct TBN matrix
    mat3 TBN = mat3(normalize(fTangent), normalize(fBitangent), normalize(fNormal));
    // Transform to world space and normalize
    return normalize(TBN * normalMapSample);
}

vec3 getSkyboxReflection(vec3 normal, vec3 worldPos, vec3 eyePos, float roughness, vec3 F0) {
    vec3 viewDir = normalize(eyePos - worldPos);
    vec3 reflectionDir = reflect(-viewDir, normalize(normal));
    
    // Sample skybox reflection
    vec3 skyReflection = texture(skyBoxTextureSampler, reflectionDir).rgb;

    // Schlick’s approximation for Fresnel reflectance
    float NoV = max(dot(normal, viewDir), 0.0);
    vec3 fresnelFactor = F0 + (vec3(1.0) - F0) * pow(1.0 - NoV, 5.0);

    // Adjust reflection intensity based on roughness
    float reflectionStrength = mix(1.0, 0.04, roughness); 
    return mix(vec3(0.0), skyReflection, fresnelFactor * reflectionStrength);
}

// === Main Fragment Shader Function ===
void main() {

if (bDepth)
{
vec2 screenUV = gl_FragCoord.xy / vec2(textureSize(depthTexture, 0));
    float storedDepth = texture(depthTexture, screenUV).r;
       finalPixelColour = vec4(vec3(storedDepth), 1.0); 
    if (gl_FragCoord.z > storedDepth + 0.001) {
        discard;  // Occluded fragment!
    }
 

    return;

 }
    if (isParticleEmitter) {

        
        // Get particle color (already passed from vertex shader)
        vec4 particleColor = fColour;
        
        // Apply lighting if needed (simplified for particles)
        vec3 litColor = particleColor.rgb;
        if (!bDoNotLight) {
            vec3 normal = normalize(fvertexNormal.xyz);
            vec3 viewDir = normalize(eyeLocation.xyz - fvertexWorldLocation.xyz);
            
            // Simplified PBR for particles
            float roughness = 0.7;
            vec3 F0 = vec3(0.04);
            litColor = calculateLightContrib(
                particleColor.rgb, 
                normal,
                fvertexWorldLocation.xyz,
                vec4(1.0),
                roughness,
                0.0, // metallic
                F0
            ).rgb;
        }
        
        // Final particle color with fade
        finalPixelColour = vec4(litColor, particleColor.a);
        return;
    }


if (bShellTexturing)
{
    // Compute shell color with the shell height and UV offset factor
    float shellHeight = float(fshellLayer)/shellCount;
    float uvOffsetFactor = 0.1;

    // Calculate the shell color
    vec3 shellColor = computeShellColor(fUV, shellHeight, texture00, texture01, uvOffsetFactor);

    // --- Use PBR lighting for shells ---
      vec4 vertexSpecular = vec4(1.0); 
     float roughnessVal = 1.0 - smoothness;  // Use default smoothness
    roughnessVal = max(roughnessVal, 0.15);
     vec3 F0 = mix(vec3(0.04), shellColor, metallic);  // Use default metallic
    finalPixelColour.xyz = shellColor;
    // Calculate lighting
  if  (shellHeight>10) finalPixelColour = calculateLightContrib(
        shellColor, 
      normalize(fvertexNormal.xyz),  // Use existing normal calculation
       fvertexWorldLocation.xyz, 
       vertexSpecular, 
     roughnessVal, 
      metallic,  // Use default metallic uniform
       F0
   );

    // Apply post-processing (same as regular objects)
    finalPixelColour.a = 1.f;
    
    return;  // Early return still needed for shells
}
      vec3 finalNormal = fvertexNormal.xyz;
    // --- Section 1: Wave Effects & UV Animation ---
    vec2 movingUV = fUV + vec2(time * speedX, time * speedY);
    // === New Zoom Functionality ===
// Apply zoom centered at UV(0.5, 0.5)
if (zoomPower > 0.0) {
    float zoomFactor = zoomPower*1.8+1;// Prevent division by zero
    movingUV -= vec2(0.5); // Center at middle
    movingUV /= zoomFactor; // Apply zoom
    movingUV += vec2(0.5); // Restore coordinates
}

// Process multiple waves
for (int i = 0; i < 10; i++) {
    vec2 waveOrigin = waves[i].data.xy;
    bool isActive = (waves[i].data.z > 0.5);
    float waveTime = waves[i].data.w;
    if (isActive) {
        float dist = length(movingUV - waveOrigin);
        float waveRadius = waveTime * 0.3; // Expanding radius
        float thickness = 0.02;
        float fade = smoothstep(thickness, 0.0, abs(dist - waveRadius));
        vec2 displacement = normalize(movingUV - waveOrigin) * fade * 0.02;
        
        // Apply displacement to UVs
        movingUV += displacement;
        
        // Now, modify the normals in the same way:
        vec3 normalDisplacement = normalize(vec3(displacement, 0.0)) * fade * 0.02;

        // Assuming normal is a vec3 representing the normal vector
       // finalNormal += normalDisplacement; // Add displacement to the normal
        
        // Normalize the modified normal to keep it a unit vector
       // finalNormal = normalize(finalNormal);
    }
}
    
   

    // --- Section 2: Stencil & Skybox Handling ---
    if (bUseStencilTexture) {
        float stencilColour = texture(stencilTexture, movingUV.st).r;
        if (stencilColour < 0.5f)
            discard;
    }
    if (bIsSkyBoxObject) {
        finalPixelColour.rgb = texture(skyBoxTextureSampler, fvertexNormal.xyz).rgb;
        finalPixelColour.a = 1.0f;
        return;
    }
    
    // --- Section 3: Base Vertex Colour Determination ---
    vec4 vertexColour = fColour;
    if (bUseObjectColour) {
        vertexColour = objectColour;
    } else if (bUseTextureAsColour) {
        //vec4 tex0 = vec4(texture(texture00, movingUV.st).rgb, texRatio_0_to_3.x);
        //vec4 tex1 = vec4(texture(texture01, movingUV.st).rgb, texRatio_0_to_3.y);
        //vec4 tex2 = vec4(texture(texture02, movingUV.st).rgb, texRatio_0_to_3.z);
        //vec4 tex3 = vec4(texture(texture03, movingUV.st).rgb, texRatio_0_to_3.w);
        //vec4 layeredColor = compositeOver(tex0, tex1);
        //layeredColor = compositeOver(layeredColor, tex2);
        //layeredColor = compositeOver(layeredColor, tex3);
        //vertexColour = layeredColor.rgb;
        // --- Chromatic Aberration code (optional, commented out) ---
        vec2 uv = movingUV.st;
        float shift = chromaticPower;
        vec4 r = texture(texture00, uv + vec2(shift, 0.0));
        vec4 g = texture(texture00, uv);
        vec4 b = texture(texture00, uv - vec2(shift, 0.0));
        vec4 tex0 = vec4(r.r, g.g, b.b, texRatio_0_to_3.x);
        vec4 tex1 = vec4(texture(texture01, uv).rgb, texRatio_0_to_3.y);
        vec4 tex2 = vec4(texture(texture02, uv).rgb, texRatio_0_to_3.z);
        vec4 tex3 = vec4(texture(texture03, uv).rgb, texRatio_0_to_3.w);
        vec4 layeredColor = compositeOver(tex0, tex1);
        layeredColor = compositeOver(layeredColor, tex2);
        layeredColor = compositeOver(layeredColor, tex3);
        vertexColour = layeredColor;
    }
    
    // --- Section 4: Lighting and PBR Calculation ---
    if (bDoNotLight) {
        finalPixelColour.rgb = vertexColour.rgb;
        finalPixelColour.a = 1.0f;
    } else {
        vec4 vertexSpecular = vec4(1.0); // Default specular properties
        
        // Use default metallic/smoothness values; override with texture if enabled.
        float finalMetalic = metallic;
        float finalSmoothness = smoothness;
        
        // --- Normal Mapping ---
  
        if (useNM) {
            finalNormal = getTBNNormal(finalNormal, fTangent, fBitangent, textureNM, movingUV);
        }
        
        // --- Metallic & Smoothness Texture ---
        if (useST) {
            vec4 stTexture = texture(textureST, movingUV.st);
            finalMetalic = stTexture.r;   // Metallic from red channel
            finalSmoothness = stTexture.g; // Smoothness (or roughness) from green channel
        }
        
        // Compute roughness (inverse of smoothness) and clamp it
        float roughnessVal = 1.0 - finalSmoothness;
        roughnessVal = max(roughnessVal, 0.15);
        
        // Compute F0: blend between non-metal F0 (0.04) and the material colour for metals
        vec3 F0 = mix(vec3(0.04), vertexColour.rgb, metallic);
        
        finalNormal = normalize(finalNormal);
        // Calculate lighting contribution using PBR
        finalPixelColour = calculateLightContrib(vertexColour.rgb, finalNormal.xyz, fvertexWorldLocation.xyz, 
                                                  vertexSpecular, roughnessVal, finalMetalic, F0);


    // Skybox reflection
    vec3 reflection = getSkyboxReflection(finalNormal, fvertexWorldLocation.xyz, eyeLocation.xyz, roughnessVal, F0);
   // reflection = reflection*2.0; 
    // Calculate Fresnel-Schlick approximation to get the reflection factor based on the view direction
    vec3 viewDir = normalize(eyeLocation.xyz - fvertexWorldLocation.xyz); // Camera to fragment direction
    float NoV = max(dot(finalNormal, viewDir), 0.0); // Dot product between normal and view direction
    vec3 fresnelFactor = F0 + (vec3(1.0) - F0) * pow(1.0 - NoV, 5.0); // Fresnel-Schlick

    // Mix the reflection based on the Fresnel factor (affects metal vs. non-metal behavior)
    finalPixelColour.rgb = mix(finalPixelColour.rgb, reflection, fresnelFactor.r); // Using red channel for mixing intensity
     
    vec3 reflectionDir = reflect(-viewDir, normalize(finalNormal));
    // Sample skybox reflection
    vec3 skyReflection = texture(skyBoxTextureSampler, reflectionDir).rgb;
   //  finalPixelColour.rgb = skyReflection;
    }
    
    // --- Section 5: Post-Processing ---
    // Set alpha channel
    finalPixelColour.a = wholeObjectTransparencyAlpha;
    

    // Night mode adjustments
    if (bNightMode) {
        finalPixelColour.r *= 0.7;
        finalPixelColour.b *= 0.7;
        finalPixelColour.g *= 2.0;
        finalPixelColour.a = 1.0;
    }
    
    // Ambient Occlusion
    if (useAO) {
        float ao = texture(textureAO, movingUV).r;
        finalPixelColour.rgb *= ao;
    }

    if (finalPixelColour.a==1)
    finalPixelColour.a = objectColour.a;
    return;
}
///PBR


// Microfacet distribution function (GGX)
float D_GGX(float NoH, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float denom = (NoH * NoH) * (a2 - 1.0) + 1.0;
    return a2 / (PI * denom * denom);
}

// Geometry function (Smith)
float G_Smith(float NoV, float NoL, float roughness) {
    float k = roughness / 2.0;
    return (NoV / (NoV * (1.0 - k) + k)) * (NoL / (NoL * (1.0 - k) + k));
}

// Cook-Torrance specular calculation
// Now takes normal, viewDir, and lightDir as explicit parameters.
vec3 cookTorranceSpecular(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness, vec3 F0) {
    vec3 halfwayDir = normalize(viewDir + lightDir);
    float NoL = max(dot(normal, lightDir), 0.0);
    float NoV = max(dot(normal, viewDir), 0.0);
    float NoH = max(dot(normal, halfwayDir), 0.0);

    // Microfacet distribution
    float D = D_GGX(NoH, roughness);
    // Geometry term
    float G = G_Smith(NoV, NoL, roughness);
    // Fresnel term using Schlick's approximation
    vec3 F = F0 + (vec3(1.0) - F0) * pow(1.0 - max(dot(halfwayDir, viewDir), 0.0), 5.0);

    // Prevent division by zero with a small offset
    return (D * G * F) / (4.0 * NoL * NoV + 0.001);
}

// Lambert diffuse calculation now accepts the material albedo.
vec3 lambertDiffuse(vec3 lightDir, vec3 normal, vec3 albedo) {
    return max(dot(normal, lightDir), 0.0) * albedo / PI;
}


// --- Lighting Type Functions ---

// Directional Light: No attenuation.
vec3 calculateDirectionalLightPBR(int index, vec3 norm, vec3 vertexMaterialColor, 
                                  vec3 viewDir, float roughness, vec3 F0) {
    vec3 lightDir = normalize(-theLights[index].direction.xyz);
    vec3 diffuse = lambertDiffuse(lightDir, norm, vertexMaterialColor);
    vec3 specular = cookTorranceSpecular(norm, viewDir, lightDir, roughness, F0);
    return (diffuse + specular) * theLights[index].diffuse.rgb;
}

// Point Light: With attenuation.
vec3 calculatePointLightPBR(int index, vec3 norm, vec3 vertexMaterialColor, 
                            vec3 vertexWorldPos, vec3 viewDir, float roughness, vec3 F0) {
    vec3 lightPos = theLights[index].position.xyz;
    vec3 lightDir = normalize(lightPos - vertexWorldPos);
    float dist = length(lightPos - vertexWorldPos);
    // If beyond the cutoff distance, skip this light.
    if (dist > theLights[index].atten.w) return vec3(0.0);


    // Attenuation: 1/(constant + linear * d + quadratic * d²)
    float attenuation = 1.0 / (theLights[index].atten.x +
                                 theLights[index].atten.y * dist +
                                 theLights[index].atten.z * (dist * dist));

    vec3 diffuse = lambertDiffuse(lightDir, norm, vertexMaterialColor);

    vec3 specular = cookTorranceSpecular(norm, viewDir, lightDir, roughness, F0);

    return (diffuse + specular) * attenuation * theLights[index].diffuse.rgb;
}

// Spot Light: Similar to point light, but with additional cone falloff.
vec3 calculateSpotLightPBR(int index, vec3 norm, vec3 vertexMaterialColor, 
                           vec3 vertexWorldPos, vec3 viewDir, float roughness, vec3 F0) {
    vec3 lightPos = theLights[index].position.xyz;
    vec3 lightDir = normalize(lightPos - vertexWorldPos);
    float dist = length(lightPos - vertexWorldPos);
    if (dist > theLights[index].atten.w) return vec3(0.0);
    float attenuation = 1.0 / (theLights[index].atten.x +
                                 theLights[index].atten.y * dist +
                                 theLights[index].atten.z * (dist * dist));
    
    // Compute spotlight cone factor.
    vec3 spotDir = normalize(theLights[index].direction.xyz);
    // Note: 'lightDir' is from light to fragment, so -lightDir aligns with the light's forward direction.
    float currentLightRayAngle = dot(-lightDir, spotDir);
    // Outer cone and inner cone (in degrees) stored in param1.z and param1.y respectively.
    float outerConeAngleCos = cos(radians(theLights[index].param1.z));
    float innerConeAngleCos = cos(radians(theLights[index].param1.y));
    
    // If outside outer cone, no contribution.
    if (currentLightRayAngle < outerConeAngleCos)
        return vec3(0.0);
    // Between outer and inner cone: apply penumbra smoothing.
    else if (currentLightRayAngle < innerConeAngleCos) {
        float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) /
                              (innerConeAngleCos - outerConeAngleCos);
        attenuation *= penumbraRatio;
    }
    
    vec3 diffuse = lambertDiffuse(lightDir, norm, vertexMaterialColor);
    vec3 specular = cookTorranceSpecular(norm, viewDir, lightDir, roughness, F0);
    return (diffuse + specular) * attenuation * theLights[index].diffuse.rgb;
}

// --- Main PBR Lighting Function ---
vec4 calculateLightContrib(vec3 vertexMaterialColor, vec3 vertexNormal, vec3 vertexWorldPos, 
                            vec4 vertexSpecular, float roughness, float metallic, vec3 F0) {
    // Normalize the input normal.
    vec3 norm = normalize(vertexNormal);
    // Compute view direction (from fragment to camera).
    vec3 viewDir = normalize(eyeLocation.xyz - vertexWorldPos);
    vec3 finalColor = vec3(0.0);
    
    // Loop over each light.
    for (int index = 0; index < NUMBEROFLIGHTS; index++) {
        // Skip light if turned off.
        if (theLights[index].param2.x == 0.0) continue;
        
        int lightType = int(theLights[index].param1.x);
        if (lightType == DIRECTIONAL_LIGHT_TYPE) {
            finalColor += calculateDirectionalLightPBR(index, norm, vertexMaterialColor, viewDir, roughness, F0);
        } else if (lightType == POINT_LIGHT_TYPE) {
            finalColor += calculatePointLightPBR(index, norm, vertexMaterialColor, vertexWorldPos, viewDir, roughness, F0);
        } else if (lightType == SPOT_LIGHT_TYPE) {
            finalColor += calculateSpotLightPBR(index, norm, vertexMaterialColor, vertexWorldPos, viewDir, roughness, F0);
        }
    }
    
    return vec4(finalColor, 1.0);
}


//vec2 vortexEffect(vec2 uv, float strength, float speed)
//{
//    vec2 centeredUV = uv - 0.5;  // Center the UV coordinates
//   float angle = atan(centeredUV.y, centeredUV.x);  // Get the angle
//    float radius = length(centeredUV);  // Get the distance from center

//   float twist = strength * sin(time * speed + radius * 10.0);  // Create a time-based twist
//    angle += twist;  // Apply the twist

//    vec2 newUV;
//    newUV.x = cos(angle) * radius;
//    newUV.y = sin(angle) * radius;

//    return newUV + 0.5;  // Shift back to UV space
//}
