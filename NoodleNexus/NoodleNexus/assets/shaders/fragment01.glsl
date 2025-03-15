#version 330
// (Pixel) Fragment fragment

#define PI 3.14159265359


in vec3 fColour;			// Actual 3D model colour (from vertex buffer)
in vec4 fvertexWorldLocation;
in vec4 fvertexNormal;
in vec2 fUV;				// Texture (UV) coordinates

uniform vec4 objectColour;			// Override colour 
uniform bool bUseObjectColour;
uniform vec4 eyeLocation;			// Where the camera is
uniform bool bDoNotLight;			// if true, skips lighting
uniform bool bNightMode;
// 0.0 to 1.0 (invisible to solid)
// Controls the alpha channel
uniform float wholeObjectTransparencyAlpha;

// Written to the framebuffer (backbuffer)
out vec4 finalPixelColour;	// RGB_A

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;


struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
	                // yzw are TBD
};

struct sWave {
    vec4 data; // x = uv.x, y = uv.y, z = active flag, w = time
};

const int NUMBEROFLIGHTS = 40;
uniform sLight theLights[NUMBEROFLIGHTS]; 
// uniform vec4 thelights[0].position;
// uniform vec4 thelights[1].position;

// Inspired by Mike Bailey's Graphic Shader, chapter 6
// (you should read it. Yes, you)
vec4 calculateLightContrib(vec3 vertexMaterialColor, vec3 vertexNormal, vec3 vertexWorldPos, 
                            vec4 vertexSpecular, float roughness, float metallic, vec3 F0);

// Allows us to lookup the RGB colour from a 2D texture
// Give it the UV and it returns the colour at that UV location
uniform sampler2D texture00;
uniform sampler2D texture01;
uniform sampler2D texture02;
uniform sampler2D texture03;

uniform sampler2D textureAO;
uniform bool useAO;


uniform	sampler2D textureST;
uniform bool useST;


uniform	float metallic;
uniform	float smoothness;


uniform vec4 texRatio_0_to_3;	// x index 0, y index 1, etc/
//uniform float texRatio[4];
uniform bool bUseTextureAsColour;	// If true, then sample the texture

// Skybox (or reflection, refraction, etc.)
uniform samplerCube skyBoxTextureSampler;
uniform bool bIsSkyBoxObject;
//

// For discard stencil example
uniform sampler2D stencilTexture;
uniform bool bUseStencilTexture;

uniform float time;        // Current time in seconds
uniform float speedX;      // Speed in the X direction
uniform float speedY;      // Speed in the Y direction


uniform float zoomPower;      // Power of zoom on texture


uniform sWave waves[10];


vec4 compositeOver(vec4 bottom, vec4 top) {

   if (all(equal(top.rgb, vec3(0.0)))) {
        return bottom;
    }

    vec3 outColor = top.rgb * top.a + bottom.rgb * (1.0 - top.a);
    float outAlpha = top.a + bottom.a * (1.0 - top.a);
    return vec4(outColor, outAlpha);
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



void main()
{

 vec2 movingUV = fUV + vec2(time * speedX, time * speedY);

//vec2 center = vec2(0.5);
//float scale = 1.0 + zoomPower;
//movingUV = (movingUV - center) / scale + center;

// movingUV.x += sin(movingUV.y * 10.0 + zoomPower) * 0.05;
//movingUV = vortexEffect(movingUV, 0.2, 2.0);


//WAVES


for (int i = 0; i < 10; i++) {
    // Unpack the wave data from the uniform array
    vec2 waveOrigin = waves[i].data.xy;
    bool isActive = (waves[i].data.z > 0.5);
    float waveTime = waves[i].data.w;

    // Only process active waves
    if (isActive)
    {
        // Compute the distance from this wave's origin
        float dist = length(movingUV - waveOrigin);

        // Define wave properties.
        // Here we're using zoomPower to compute the radius, but you could also use waveTime.
        float waveRadius = waveTime * 0.3;  // For example, an expanding radius based on zoomPower
        float thickness = 0.02;              // Thickness of the wave
        float fade = smoothstep(thickness, 0.0, abs(dist - waveRadius));

        // Compute the displacement for this wave.
        vec2 displacement = normalize(movingUV - waveOrigin) * fade * 0.02;
        
        // Accumulate the displacement.
        movingUV += displacement;
    }
}


//SINGLE WAVE
    // Origin of the shockwave (can be set dynamically)
    vec2 waveOrigin = vec2(0.5, 0.5);

    // Distance from the wave origin
    float dist = length(movingUV - waveOrigin);

    // Define wave properties
    float waveRadius = zoomPower * 0.3;  // Expanding radius over time
    float thickness = 0.02;          // Thickness of the wave
    float fade = smoothstep(thickness, 0.0, abs(dist - waveRadius));

    // Apply radial distortion outward only where the wave exists
    vec2 displacement = normalize(movingUV - waveOrigin) * fade * 0.02;
    movingUV += displacement;

	//ZOOOM
 
   // float dist = length(movingUV - 0.5);
  // movingUV.x += sin(dist * 20.0 - zoomPower * 2.0) * 0.01;
   // movingUV.y += cos(dist * 20.0 - zoomPower * 2.0) * 0.01;






	// discard transparency
	// uniform sampler2D stencilTexture;
	// uniform bool bUseStencilTexture;
	if ( bUseStencilTexture )
	{
		float stencilColour = texture( stencilTexture, movingUV.st ).r;
		//
		if ( stencilColour < 0.5f )	// Is it "black enough"
		{
			discard;	// don't draw this pixel
		}
	}


	// For the skybox object
	if ( bIsSkyBoxObject )
	{
		//finalPixelColour.rgb = fvertexNormal.xyz;
		//uniform samplerCube skyBoxTextureSampler;
		// Note: We are passing the NORMALS (a ray to hit the inside
		// 	of the cube) and NOT the texture coordinates
		finalPixelColour.rgb = texture( skyBoxTextureSampler, fvertexNormal.xyz ).rgb;
		finalPixelColour.a = 1.0f;
		return;
	}
	
	



	vec3 vertexColour = fColour;
	if ( bUseObjectColour )
	{
		vertexColour = objectColour.rgb;
	}
	else
	{
	
	if ( bUseTextureAsColour )
	{


	//I'm gonna make it so textures gonna be on top if each other.
	//I don't think we'll need blending in the future.
	//but if we'l need it, we'll wigure it out.



		vec4 tex0 = vec4(texture(texture00, movingUV.st).rgb, texRatio_0_to_3.x);
		vec4 tex1 = vec4(texture(texture01, movingUV.st).rgb, texRatio_0_to_3.y);
		vec4 tex2 = vec4(texture(texture02, movingUV.st).rgb, texRatio_0_to_3.z);
		vec4 tex3 = vec4(texture(texture03, movingUV.st).rgb, texRatio_0_to_3.w);

		
		// Composite textures sequentially: tex0 is the base
		vec4 layeredColor = compositeOver(tex0, tex1);
		layeredColor = compositeOver(layeredColor, tex2);
		layeredColor = compositeOver(layeredColor, tex3);
		vertexColour = layeredColor.rgb;

// ------------------------------------------------------------------------
//      Chromatic Abberation
// ------------------------------------------------------------------------
//		vec2 uv = movingUV.st;
//		float shift = 0.01 * sin(time); // Chromatic shift factor

//		vec4 r = texture(texture00, uv + vec2(shift, 0.0));
//		vec4 g = texture(texture00, uv);
//		vec4 b = texture(texture00, uv - vec2(shift, 0.0));

//		vec4 tex0 = vec4(r.r, g.g, b.b, texRatio_0_to_3.x); // Apply RGB shift

//		vec4 tex1 = vec4(texture(texture01, uv).rgb, texRatio_0_to_3.y);
//		vec4 tex2 = vec4(texture(texture02, uv).rgb, texRatio_0_to_3.z);
//		vec4 tex3 = vec4(texture(texture03, uv).rgb, texRatio_0_to_3.w);

		// Composite textures
//		vec4 layeredColor = compositeOver(tex0, tex1);
//		layeredColor = compositeOver(layeredColor, tex2);
//		layeredColor = compositeOver(layeredColor, tex3);
    
//		vertexColour = layeredColor.rgb;

						   
					   
	} 

	}


	
	// Use lighting?
	if ( bDoNotLight )
	{
		finalPixelColour.rgb = vertexColour.rgb;
		finalPixelColour.a = 1.0f;
		
	}
	else
	{
	
	vec4 vertexSpecular = vec4(1.0f, 1.0f, 1.0f, 1.0f);	

float finalMetalic = metallic;
float finalSmoothness = smoothness;


    if (useST)
    {
    // Sample the texture for metallic and roughness information
vec4 stTexture = texture(textureST, movingUV.st);


// Extract values for smoothness and metallic (assuming they are in the R and G channels respectively)
finalMetalic = stTexture.r;   // Assuming metallic is stored in the red channel
finalSmoothness = stTexture.g; // Assuming roughness is stored in the green channel
}

float roughness =1-finalSmoothness; 
roughness = max(roughness, 0.1);

// Compute F0: for non-metals, F0 is typically 0.04, and for metals, it's the albedo.
// mix() blends these based on the 'metallic' value.

		vec3 F0 = mix(vec3(0.04), vertexColour, metallic);

// Now compute the lighting contribution using the new PBR function:
finalPixelColour = calculateLightContrib(vertexColour, fvertexNormal.xyz, fvertexWorldLocation.xyz, 
                                          vertexSpecular, roughness, finalMetalic, F0);
}
	
        

	

											
	
	// Set the alpha channel
	finalPixelColour.a = wholeObjectTransparencyAlpha;	
	
//	// Reflection:
//	vec3 eyeToVertexRay = normalize(eyeLocation.xyz - fvertexWorldLocation.xyz);
//	
//	vec3 reflectRay = reflect(eyeToVertexRay, fvertexNormal.xyz);	
//	vec3 refractRay = refract(eyeToVertexRay, fvertexNormal.xyz, 1.2f);
//	
//	vec3 reflectColour = texture( skyBoxTextureSampler, reflectRay.xyz ).rgb;
//	vec3 refractColour = texture( skyBoxTextureSampler, refractRay.xyz ).rgb;
//	
//	finalPixelColour.rgb += reflectColour.rgb * 0.3f
//	                       + refractColour.rgb * 0.3f;
			
			
	if ( bNightMode )
	{
		
		finalPixelColour.r=finalPixelColour.r* 0.7;
		finalPixelColour.b=finalPixelColour.b* 0.7;
		finalPixelColour.g=finalPixelColour.g* 2;
		finalPixelColour.a = 1.0f;
	
	}
	
	if (useAO)
	{
	float ao = texture(textureAO, movingUV).r;
	finalPixelColour.rgb *= ao;
	}
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