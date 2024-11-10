#pragma once


#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>
#include "Gun.hpp"
#include "Scene.hpp"
#include "sharedThings.h"


class aExhaustTrail : public Action
{
public:
    glm::vec4 trailColor = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
    std::vector<glm::vec3> trailPositions;

    void Start() override {}

    void Update() override
    {
        // Store current position as trail point
        trailPositions.push_back(object->mesh->positionXYZ);
        if (trailPositions.size() > 50)  // Limit the trail length
            trailPositions.erase(trailPositions.begin());

        //// Draw each trail particle
        //for (glm::vec3& pos : trailPositions) 
        //    DrawDebugSphere(pos, trailColor, 0.1f, object->program);  // Ensure 'program' is defined
    }
};


