#version 410

// --- Common Definitions and Functions ---
#define PI 3.14159265359

// Light Types
const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

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

// Cook-Torrance specular function
vec3 cookTorranceSpecular(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness, vec3 F0) {
    vec3 halfwayDir = normalize(viewDir + lightDir);
    float NoL = max(dot(normal, lightDir), 0.0);
    float NoV = max(dot(normal, viewDir), 0.0);
    float NoH = max(dot(normal, halfwayDir), 0.0);
    float D = D_GGX(NoH, roughness);
    float G = G_Smith(NoV, NoL, roughness);
    vec3 F = F0 + (vec3(1.0) - F0) * pow(1.0 - max(dot(halfwayDir, viewDir), 0.0), 5.0);
    return (D * G * F) / (4.0 * NoL * NoV + 0.001);
}

// Lambert diffuse function
vec3 lambertDiffuse(vec3 lightDir, vec3 normal, vec3 albedo) {
    return max(dot(normal, lightDir), 0.0) * albedo / PI;
}

// (Include any additional helper or lighting functions needed...)


// --- Particle Shader Inputs ---
in vec3 fWorldPos;
in vec3 fNormal;
in vec2 fUV;
in vec3 fTangent;
in vec3 fBitangent;
in vec4 fColor;
in float fLife;

out vec4 finalColor;

// Uniforms (example list, adjust to your needs)
uniform vec3 eyeLocation;
uniform float metallic;
uniform float smoothness;
uniform sampler2D textureNM;
uniform bool useNM;
uniform samplerCube skyBoxTextureSampler;
const int NUMBEROFLIGHTS = 40;
uniform struct sLight {
    vec4 position;
    vec4 diffuse;
    vec4 specular;
    vec4 atten;
    vec4 direction;
    vec4 param1;
    vec4 param2;
} theLights[NUMBEROFLIGHTS];

// Prototype for the main lighting function
vec4 calculateLightContrib(vec3 vertexMaterialColor, vec3 vertexNormal, 
                           vec3 vertexWorldPos, vec4 vertexSpecular, 
                           float roughness, float metallic, vec3 F0);

// --- Main Function ---
void main() {
    finalColor.r = 1.f;
    finalColor.a = 1;
    return;
    // Discard dead particles
   // if(fLife <= 0.0) discard;

    float alpha =1;// smoothstep(0.0, 0.2, fLife) * smoothstep(1.0, 0.8, fLife);

    vec3 finalNormal = fNormal;
    if(useNM) {
        vec3 normalMap = texture(textureNM, fUV).xyz * 2.0 - 1.0;
        mat3 TBN = mat3(normalize(fTangent), normalize(fBitangent), normalize(fNormal));
        finalNormal = normalize(TBN * normalMap);
    }

    float roughnessVal = 1.0 - smoothness;
    vec3 F0 = mix(vec3(0.04), fColor.rgb, metallic);
    vec4 litColor = calculateLightContrib(fColor.rgb, finalNormal, fWorldPos, vec4(1.0), roughnessVal, metallic, F0);

    vec3 viewDir = normalize(eyeLocation - fWorldPos);
    vec3 reflection = texture(skyBoxTextureSampler, reflect(-viewDir, finalNormal)).rgb;
    float fresnel = pow(1.0 - max(dot(finalNormal, viewDir), 0.0), 5.0);
    
    finalColor = vec4(mix(litColor.rgb, reflection, fresnel * metallic), alpha);
    finalColor.r = 1.f;
    finalColor.a = 1;
}

// --- PBR Lighting Implementation ---
vec4 calculateLightContrib(vec3 vertexMaterialColor, vec3 vertexNormal, vec3 vertexWorldPos, 
                           vec4 vertexSpecular, float roughness, float metallic, vec3 F0) {
    vec3 norm = normalize(vertexNormal);
    vec3 viewDir = normalize(eyeLocation - vertexWorldPos);
    vec3 finalColorAccum = vec3(0.0);
    
    for (int index = 0; index < NUMBEROFLIGHTS; index++) {
        if (theLights[index].param2.x == 0.0) continue;  // Skip if light is off
        
        int lightType = int(theLights[index].param1.x);
        if (lightType == DIRECTIONAL_LIGHT_TYPE) {
            vec3 lightDir = normalize(-theLights[index].direction.xyz);
            vec3 diffuse = lambertDiffuse(lightDir, norm, vertexMaterialColor);
            vec3 specular = cookTorranceSpecular(norm, viewDir, lightDir, roughness, F0);
            finalColorAccum += (diffuse + specular) * theLights[index].diffuse.rgb;
        } else if (lightType == POINT_LIGHT_TYPE) {
            vec3 lightPos = theLights[index].position.xyz;
            vec3 lightDir = normalize(lightPos - vertexWorldPos);
            float dist = length(lightPos - vertexWorldPos);
            if (dist > theLights[index].atten.w) continue;
            float attenuation = 1.0 / (theLights[index].atten.x +
                                       theLights[index].atten.y * dist +
                                       theLights[index].atten.z * (dist * dist));
            vec3 diffuse = lambertDiffuse(lightDir, norm, vertexMaterialColor);
            vec3 specular = cookTorranceSpecular(norm, viewDir, lightDir, roughness, F0);
            finalColorAccum += (diffuse + specular) * attenuation * theLights[index].diffuse.rgb;
        } else if (lightType == SPOT_LIGHT_TYPE) {
            vec3 lightPos = theLights[index].position.xyz;
            vec3 lightDir = normalize(lightPos - vertexWorldPos);
            float dist = length(lightPos - vertexWorldPos);
            if (dist > theLights[index].atten.w) continue;
            float attenuation = 1.0 / (theLights[index].atten.x +
                                       theLights[index].atten.y * dist +
                                       theLights[index].atten.z * (dist * dist));
            vec3 spotDir = normalize(theLights[index].direction.xyz);
            float currentLightRayAngle = dot(-lightDir, spotDir);
            float outerConeAngleCos = cos(radians(theLights[index].param1.z));
            float innerConeAngleCos = cos(radians(theLights[index].param1.y));
            if (currentLightRayAngle < outerConeAngleCos) continue;
            else if (currentLightRayAngle < innerConeAngleCos) {
                float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) /
                                        (innerConeAngleCos - outerConeAngleCos);
                attenuation *= penumbraRatio;
            }
            vec3 diffuse = lambertDiffuse(lightDir, norm, vertexMaterialColor);
            vec3 specular = cookTorranceSpecular(norm, viewDir, lightDir, roughness, F0);
            finalColorAccum += (diffuse + specular) * attenuation * theLights[index].diffuse.rgb;
        }
    }
    return vec4(finalColorAccum, 1.0);
}
