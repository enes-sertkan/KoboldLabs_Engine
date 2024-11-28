#pragma once

#include "Action.h"
#include "Scene.hpp"
#include "aRayCastPhysics2D.hpp"
#include "aRemoveAfterTime.hpp"


class aScoreManager : public Action
{
public:


    aRayCastPhysics2D* marioPhys;
    float downRayLength = 15.f;;
    glm::vec3 down = glm::vec3(0, 1, 0);
    bool wasOnRight = false;
    bool wasOnLeft = false;
    Object* currentBarrel = nullptr;
    void Update() override
    {
        if (!marioPhys) return;

        glm::vec3 startPos = object->mesh->positionXYZ;
        glm::vec3 endPos = startPos+down*downRayLength;
        std::vector<sCollision_RayTriangleInMesh> collisions;

        Object* barrel = object->scene->GetNearbyObject("Barrel", object->mesh->positionXYZ, 25.f);


        if (!barrel) return;

        std::cout << "WE HAVE A BARREL" << std::endl;

        if (barrel != currentBarrel)
        {
            currentBarrel = barrel;

            wasOnRight = false;
            wasOnLeft = false;

            if (barrel->mesh->positionXYZ.z > object->mesh->positionXYZ.z)
            {
                wasOnRight = true;
                std::cout << "On the right" << std::endl;
            }
            else
            {
                wasOnLeft = true;
            }

            return;
        } 


        if (wasOnRight)
        {
            if (barrel->mesh->positionXYZ.z < object->mesh->positionXYZ.z)
            {
                DrawScore(object->mesh->positionXYZ);
                currentBarrel = nullptr;
                wasOnRight = false;
                wasOnLeft = false;
            }
        }

        else if (wasOnLeft)
        {
            if (barrel->mesh->positionXYZ.z > object->mesh->positionXYZ.z)
            {
                DrawScore(object->mesh->positionXYZ);
                currentBarrel = nullptr;
                wasOnRight = false;
                wasOnLeft = false;
            }
     
        }
     

       
    }

    void DrawScore(glm::vec3 position)
    {
        std::cout << "SCORE +100" << std::endl;
        Object*  score = object->scene->GenerateMeshObjectsFromObject("assets/models/dk_3d_all_obj/DonkeyKong_Level_0_100.ply",position,0.1f,glm::vec3(0,-90,0),true,glm::vec4(1,1,1,1),false,object->scene->sceneObjects);
       aRemoveAfterTime* removeAfterTime = new aRemoveAfterTime();
        removeAfterTime->timeToRemove = 1.f;
        object->scene->AddActionToObj(removeAfterTime, score);
    }
};
