#version 330
// Vertex shader
in vec3 vCol;
in vec3 vPos;
in vec3 vNormal;	// Normal from the model ("model" space)
in vec2 vUV;		// Texture coordinates

out vec3 fColour;
out vec4 fvertexWorldLocation;
out vec4 fvertexNormal;		// Normal in "world" space
out vec2 fUV;				// Texture coordinates (to the fragment shader)

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
	//fvertexNormal = matRoationOnly * vec4(vNormal, 1.0);
	mat4 matInvTransModel = inverse(transpose(matModel));
	// Just in case
	
	vec3 vNormNormalize = normalize(vNormal.xyz);
	fvertexNormal = matInvTransModel * vec4(vNormNormalize, 1.0);
	// Just in case
	fvertexNormal.xyz = normalize(vNormNormalize);
	
	fColour = vCol;
	fUV = vUV;			// Sent UVs to fragment shader
}
