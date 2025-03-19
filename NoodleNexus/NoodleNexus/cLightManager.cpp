#include "cLightManager.h"
#include <float.h>
#include <sstream>	// String stream
#include "sharedThings.h"
#include <iostream>


cLightManager::cLightManager()
{
	// Clear the lights to a known default state
	for (unsigned int index = 0; index != cLightManager::NUMBEROFLIGHTS; index++)
	{
		this->theLights[index].position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		// White coloured light
		this->theLights[index].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		// White specular highlight colour, NOT shiny (4th value of 1.0f)
		this->theLights[index].specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		// Reasonable attenuation
		this->theLights[index].atten.x = 0.0f;		// x = constant
		this->theLights[index].atten.y = 0.1f;		// y = linear
		this->theLights[index].atten.z = 0.01f;		// z = quadratic
		//FOR SOE REASON THIS IS GETTING REWRITTEN TO THIS EVERY TIME I RELOAD THE SCENE
		//TODO FIX
		this->theLights[index].atten.w = 30.f;	// w = DistanceCutOff (FLT_MAX = infinite)

		this->theLights[index].direction = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		// 0 = pointlight
		// 1 = spot light
		// 2 = directional light
		this->theLights[index].param1.x = 0.0f;		// x = lightType
		this->theLights[index].param1.y = 0.0f;		// y = inner angle
		this->theLights[index].param1.z = 0.0f;		// z = outer angle
		this->theLights[index].param1.w = 1.0f;		// (TDB) "To Be Determined"

		this->theLights[index].param2.x = 0.0f;		// x = 0 for off, 1 for on
		this->theLights[index].param2.y = 0.0f;		// (TDB) 
		this->theLights[index].param2.z = 0.0f;		// (TDB) 
		this->theLights[index].param2.w = 0.0f;		// (TDB) 
	}//for (unsigned int index
}

void cLightManager::loadUniformLocations(GLuint shaderProgram)
{
	// Assuming 'MAX_LIGHTS' is the maximum number of lights
	for (int i = 0; i < NUMBEROFLIGHTS; ++i)
	{
		std::string baseName = "theLights[" + std::to_string(i) + "].";

		this->theLights[i].position_UL = glGetUniformLocation(shaderProgram, (baseName + "position").c_str());
		this->theLights[i].diffuse_UL = glGetUniformLocation(shaderProgram, (baseName + "diffuse").c_str());
		this->theLights[i].specular_UL = glGetUniformLocation(shaderProgram, (baseName + "specular").c_str());
		this->theLights[i].atten_UL = glGetUniformLocation(shaderProgram, (baseName + "atten").c_str());
		this->theLights[i].direction_UL = glGetUniformLocation(shaderProgram, (baseName + "direction").c_str());
		this->theLights[i].param1_UL = glGetUniformLocation(shaderProgram, (baseName + "param1").c_str());
		this->theLights[i].param2_UL = glGetUniformLocation(shaderProgram, (baseName + "param2").c_str());
	}

	return;
}



void cLightManager::updateShaderWithLightInfo(void)
{
	// Called every frame, updating the light information in the shader


	//glUniform4f(light_00_position_UL, this->theLights[0].position.x,
	//	this->theLights[0].position.y,
	//	this->theLights[0].position.z,
	//	1.0f);

	for (unsigned int index = 0; index != cLightManager::NUMBEROFLIGHTS; index++)
	{
		glUniform4f(
			this->theLights[index].position_UL,
			this->theLights[index].position.x,
			this->theLights[index].position.y,
			this->theLights[index].position.z,
			this->theLights[index].position.w);

		glUniform4f(
			this->theLights[index].diffuse_UL,
			this->theLights[index].diffuse.x,
			this->theLights[index].diffuse.y,
			this->theLights[index].diffuse.z,
			this->theLights[index].diffuse.w);

		glUniform4f(
			this->theLights[index].specular_UL,
			this->theLights[index].specular.x,
			this->theLights[index].specular.y,
			this->theLights[index].specular.z,
			this->theLights[index].specular.w);

		glUniform4f(
			this->theLights[index].atten_UL,
			this->theLights[index].atten.x,
			this->theLights[index].atten.y,
			this->theLights[index].atten.z,
			this->theLights[index].atten.w);

		glUniform4f(
			this->theLights[index].direction_UL,
			this->theLights[index].direction.x,
			this->theLights[index].direction.y,
			this->theLights[index].direction.z,
			this->theLights[index].direction.w);

		glUniform4f(
			this->theLights[index].param1_UL,
			this->theLights[index].param1.x,
			this->theLights[index].param1.y,
			this->theLights[index].param1.z,
			this->theLights[index].param1.w);

		glUniform4f(
			this->theLights[index].param2_UL,
			this->theLights[index].param2.x,
			this->theLights[index].param2.y,
			this->theLights[index].param2.z,
			this->theLights[index].param2.w);
	}//for (unsigned int index...

	return;
}



std::string cLightManager::sLight::getState(void)
{
	std::stringstream ssLightState;

	// "position 922.3  8882.9   22.3"
	ssLightState << "position "
		<< this->position.x << " "
		<< this->position.y << " "
		<< this->position.z << " "
		<< this->atten.x << " "
		<< this->atten.y << " "
		<< this->atten.z << " "
		<< this->direction.x << " "
		<< this->direction.y << " "
		<< this->direction.x << " "
		<< this->param1.a << " "
		<< this->param1.b << " "
		<< this->param1.g << " "
		<< this->param2.x << std::endl;
	// And so on...

	// return as a string
	return ssLightState.str();
}

cLightManager::sLight cLightManager::SetLight(int index,
	const glm::vec4& position,
	const glm::vec4& diffuse,
	const glm::vec3& attenuation,
	const glm::vec4& direction,
	const glm::vec3& param1,
	float param2x)
{

	// Set the properties of the light
	theLights[index].position = position;
	theLights[index].diffuse = diffuse;
	theLights[index].atten.y = attenuation.y;
	theLights[index].atten.z = attenuation.z;

	// If it's a spotlight, set the direction and angles
	theLights[index].direction = direction;
	theLights[index].param1 = glm::vec4(param1, 0.0f);
	theLights[index].param2.x = param2x;  // Turn on/off

	return theLights[index];
}

cLightManager::sLight cLightManager::CreateNewLight(const glm::vec4& position,
	const glm::vec4& diffuse,
	const glm::vec3& attenuation,
	const glm::vec4& direction,
	const glm::vec3& param1,
	float param2x)
{
	if (lastLightIndex >= NUMBEROFLIGHTS)
	{
		std::cout << "You reached the max ammount of lights. Please increase the ammount in code or change your code completly, you stupid." << std::endl;
		return sLight();

	}


	lastLightIndex++;

	SetLight(lastLightIndex,
		position,
		diffuse,
		attenuation,
		direction,
		param1,
		param2x);
}


cLightManager::sLight cLightManager::CreateNewLight(sLight light)
{
	return CreateNewLight(light.position, light.diffuse, light.atten, light.direction, light.param1, light.param2.x);
}

void cLightManager::RemoveLight(int index)
{
	if (index < 0 || index > lastLightIndex) {
		//TODO: Add error 
		// Invalid index; do nothing
		return;
	}

	// Disable the light by turning it off
	theLights[index].TurnOff();

	// Shift remaining lights down if needed
	for (int i = index; i < lastLightIndex; ++i) {
		theLights[i] = theLights[i + 1];
	}

	// Clear the last light (now unused) to reset it
	theLights[lastLightIndex] = sLight();
	lastLightIndex--;
}

bool cLightManager::sLight::loadState(std::string stateString)
{
	std::stringstream ssLightState;
	ssLightState << stateString;

	std::string discard;
	ssLightState >> discard;		// "position"
	ssLightState >> this->position.x;
	ssLightState >> this->position.y;
	ssLightState >> this->position.z;
	ssLightState >> this->atten.x;
	ssLightState >> this->atten.y;
	ssLightState >> this->atten.z;
	ssLightState >> this->direction.x;
	ssLightState >> this->direction.y;
	ssLightState >> this->direction.x;
	ssLightState >> this->param1.a;
	ssLightState >> this->param1.b;
	ssLightState >> this->param1.g;
	ssLightState >> this->param2.x;


	// And so on...

	// If it worked
	return true;
}

