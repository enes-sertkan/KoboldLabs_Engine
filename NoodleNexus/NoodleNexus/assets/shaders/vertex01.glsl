#version 330
// Vertex shader
in vec3 vCol;
in vec3 vPos;
in vec3 vNormal;	// Normal from the model ("model" space)
in vec2 vUV;		// Texture coordinates
in vec3 vTangent;  // Tangent vector


out vec3 fColour;
out vec4 fvertexWorldLocation;
out vec4 fvertexNormal;		// Normal in "world" space
out vec2 fUV;				// Texture coordinates (to the fragment shader)
out vec3 fTangent;         // Tangent in world space
out vec3 fBitangent;       // Bitangent in world space


//uniform mat4 MVP;
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;
uniform float time;
uniform vec3 cameraLocation;
uniform float suckPower;
uniform float shakePower;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{

  vec2 seed = vPos.xy + vec2(time); // Use position and time for randomness
    float noise = rand(seed);        // Generate random value for the vertex
    float shakeAmount = noise * 10.0 - 5.0; // Scale and offset the noise

    vec3 finalVert = vPos + vec3(shakeAmount*shakePower); // Apply "shake" effect


	

	
	// Calculate location of the vertex in the "world"
	fvertexWorldLocation = matModel * vec4(finalVert, 1.0);

	// Convert fvertexWorldLocation to vec3 for distance and direction calculations
	vec3 fvertexWorldLocation3 = fvertexWorldLocation.xyz;

	float distanceToCamera = distance(fvertexWorldLocation3, cameraLocation);
	if (distanceToCamera < 60.0)
	{
	
		vec3 direction = normalize(cameraLocation - fvertexWorldLocation3);
		float power = suckPower / distanceToCamera;

		if (power>distanceToCamera)
			power = distanceToCamera;

		vec3 change = direction * power;
		finalVert = finalVert + change;
		// You can now use `change` for further computations
	}



	
	// Screen space location of vertex
	mat4 matMVP = matProjection * matView * matModel;
	gl_Position = matMVP * vec4(finalVert, 1.0);

	// Calculatte the vertex normal
	// Don't wank scaling or translation
	// Compute the normal matrix from the model matrix (using a mat3 is enough)
	mat3 normalMatrix = mat3(transpose(inverse(matModel)));


	// Transform the normal and re-normalize it
	vec3 transformedNormal = normalize(normalMatrix * vNormal);

	fvertexNormal = vec4(transformedNormal, 0.0);
	


	// Transform tangent and compute bitangent
    vec3 transformedTangent = normalize(normalMatrix * vTangent);
    vec3 transformedBitangent = normalize(cross(transformedNormal, transformedTangent));

    fTangent = transformedTangent;
    fBitangent = transformedBitangent;


	fColour = vCol;
	fUV = vUV;			// Sent UVs to fragment shader
}
