#version 330
// (Pixel) Fragment fragment
in vec3 fColour;			// Actual 3D model colour (from vertex buffer)
in vec4 fvertexWorldLocation;
in vec4 fvertexNormal;

// we are not using it now, we are just adding to test
in vec2 fUV;   

uniform vec4 objectColour;			// Override colour 
uniform bool bUseObjectColour;
uniform vec4 eyeLocation;			// Where the camera is
uniform bool bDoNotLight;			// if true, skips lighting

// we are not using it now, we are just adding to test

uniform sampler2D textureSampler; // Texture sampler

out vec4 finalPixelColour;

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

const int NUMBEROFLIGHTS = 20;
uniform sLight theLights[NUMBEROFLIGHTS]; 
// uniform vec4 thelights[0].position;
// uniform vec4 thelights[1].position;

// Inspired by Mike Bailey's Graphic Shader, chapter 6
// (you should read it. Yes, you)
vec4 calculateLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular );

void main()
{
	vec3 vertexColour = fColour;
	if ( bUseObjectColour )
	{
		vertexColour = objectColour.rgb;
	}

	// we are not using it now, we are just adding to test

	// Sample the texture color using UV coordinates
    // vec3 textureColor = texture(textureSampler, fUV).rgb;

	vec3 textureColor = vec3(0.5f, 0.5f, 0.5f);

    // Blend texture color with vertex color
    vertexColour *= textureColor;
	
	// Use lighting?
	if ( bDoNotLight )
	{
		finalPixelColour.rgb = objectColour.rgb;
		finalPixelColour.a = 1.0f;
		return;
	}
	
	
//	finalPixelColour = vec4(finalColour, 1.0);

//	vec3 fvertexNormal = vec3(0.0f, 1.0f, 0.0f);
	vec4 vertexSpecular = vec4(1.0f, 1.0f, 1.0f, 1.0f);	


	vec4 pixelColour = calculateLightContrib( vertexColour.rgb, 
	                                          fvertexNormal.xyz, 
	                                          fvertexWorldLocation.xyz, 
											  vertexSpecular );

											
	finalPixelColour = pixelColour;
	finalPixelColour.a = 1.0f;		

}


// Inspired by Mike Bailey's Graphic Shader, chapter 6
// (you should read it. Yes, you)
vec4 calculateLightContrib(vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular)
{
    vec3 norm = normalize(vertexNormal);
    
    vec4 finalObjectColour = vec4(0.23f, 0.5f, 0.4f, 1.0f);
    //vec4 finalObjectColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    int activeLightCount = 0; // Initialize count of active lights

    for (int index = 0; index < NUMBEROFLIGHTS; index++)
    {   
        // Check if the light is "on"
        if (theLights[index].param2.x == 0.0f)
        {   
            continue; // It's off, skip to the next light
        }
        
        activeLightCount++; // Increment the count of active lights

        // Cast to an int (note with c'tor)
        int intLightType = int(theLights[index].param1.x);
        
        // Handling for directional light
        if (intLightType == DIRECTIONAL_LIGHT_TYPE) // = 2
        {
            vec3 lightContrib = theLights[index].diffuse.rgb;
            float dotProduct = dot(-theLights[index].direction.xyz, normalize(norm.xyz)); // -1 to 1
            dotProduct = max(0.0f, dotProduct); // 0 to 1

            lightContrib *= dotProduct;        
            finalObjectColour.rgb += (vertexMaterialColour.rgb * theLights[index].diffuse.rgb * lightContrib); 
            continue; // Continue to next light
        }
        
        // Assume it's a point light 
        vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;
        float distanceToLight = length(vLightToVertex);    
        vec3 lightVector = normalize(vLightToVertex);
        float dotProduct = dot(lightVector, vertexNormal.xyz);     

        // Cut off the light after the distance cut off 
        if (distanceToLight > theLights[index].atten.w)
        {
            continue; // Skip this light if out of range
        }
        
        dotProduct = max(0.0f, dotProduct);    
        vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;

        vec3 lightSpecularContrib = vec3(0.0f);
        vec3 reflectVector = reflect(-lightVector, normalize(norm.xyz));
        vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);
        float objectSpecularPower = vertexSpecular.w; 
        
        lightSpecularContrib = pow(max(0.0f, dot(eyeVector, reflectVector)), objectSpecularPower)
                               * theLights[index].specular.rgb;

        // Attenuation
        float attenuation = 1.0f / 
                (theLights[index].atten.x +                                         
                 theLights[index].atten.y * distanceToLight +                        
                 theLights[index].atten.z * distanceToLight * distanceToLight);    

        // Total light contribution is Diffuse + Specular
        lightDiffuseContrib *= attenuation;
        lightSpecularContrib *= attenuation;

        // Spot light handling
        if (intLightType == SPOT_LIGHT_TYPE) // = 1
        {   
            vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;
            vertexToLight = normalize(vertexToLight);
            float currentLightRayAngle = dot(vertexToLight.xyz, theLights[index].direction.xyz);
            currentLightRayAngle = max(0.0f, currentLightRayAngle);

            float outerConeAngleCos = cos(radians(theLights[index].param1.z));
            float innerConeAngleCos = cos(radians(theLights[index].param1.y));
                            
            if (currentLightRayAngle < outerConeAngleCos)
            {
                lightDiffuseContrib = vec3(0.0f);
                lightSpecularContrib = vec3(0.0f);
            }
            else if (currentLightRayAngle < innerConeAngleCos)
            {
                float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
                                      (innerConeAngleCos - outerConeAngleCos);
                lightDiffuseContrib *= penumbraRatio;
                lightSpecularContrib *= penumbraRatio;
            }
        }

        finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
                                  + (vertexSpecular.rgb  * lightSpecularContrib.rgb );
    } // for(int index = 0...

    // Debug output: Set the red channel based on active light count
    finalObjectColour.r = float(activeLightCount) / float(NUMBEROFLIGHTS); // Normalize the count

    finalObjectColour.a = 1.0f;

    return finalObjectColour;
}
