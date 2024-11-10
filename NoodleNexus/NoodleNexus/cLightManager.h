#pragma once

#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <string>  



//DONE: ? WE should move setLight from main to here;
class cLightManager
{
public:
  
    int lastLightIndex=-1; //-1 bc we start with 0. So, we do +1 before creating new Light, so we start with 0


    cLightManager();

    void loadUniformLocations(GLuint shaderProgram);
    void updateShaderWithLightInfo(void);

    // Mirror of what's inside the shader
    struct sLight
    {
        glm::vec4 position;
        glm::vec4 diffuse;
        glm::vec4 specular;	// rgb = highlight colour, w = power
        glm::vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
        glm::vec4 direction;	// Spot, directional lights
        glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
        // 0 = pointlight
        // 1 = spot light
        // 2 = directional light
        glm::vec4 param2;	// x = 0 for off, 1 for on
        // yzw are TBD

        void setLinearAtten(float newLinear)
        {
            this->atten.y = newLinear;
        }
        void setQuadraticAtten(float newQuad)
        {
            this->atten.z = newQuad;
        }
        void TurnOn(void)
        {
            this->param2.x = 1.0f;
        }
        void TurnOff(void)
        {
            this->param2.x = 0.0f;
        }

        // The uniform locations of >this< light
        GLuint position_UL = -1;
        GLuint diffuse_UL = -1;
        GLuint specular_UL = -1;
        GLuint atten_UL = -1;
        GLuint direction_UL = -1;
        GLuint param1_UL = -1;
        GLuint param2_UL = -1;

        std::string getState(void);
        bool loadState(std::string stateString);
    };

    static const int NUMBEROFLIGHTS = 30;
    sLight theLights[NUMBEROFLIGHTS];

    sLight SetLight(int index,
        const glm::vec4& position,
        const glm::vec4& diffuse,
        const glm::vec3& attenuation,
        const glm::vec4& direction,
        const glm::vec3& param1,
        float param2x); 
    
    sLight CreateNewLight(
        const glm::vec4& position,
        const glm::vec4& diffuse,
        const glm::vec3& attenuation,
        const glm::vec4& direction,
        const glm::vec3& param1,
        float param2x);



    sLight CreateNewLight(sLight light);

        
    void RemoveLight(int index); //DONE: Add remove light


    //std::vector<sLight> vecLights;
};



// This is inside our shader
  //struct sLight
  //{
  //    vec4 position;
  //    vec4 diffuse;
  //    vec4 specular;	// rgb = highlight colour, w = power
  //    vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
  //    vec4 direction;	// Spot, directional lights
  //    vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
  //    // 0 = pointlight
  //    // 1 = spot light
  //    // 2 = directional light
  //    vec4 param2;	// x = 0 for off, 1 for on
  //    // yzw are TBD
  //};
  //const int NUMBEROFLIGHTS = 10;
  //uniform sLight theLights[NUMBEROFLIGHTS];