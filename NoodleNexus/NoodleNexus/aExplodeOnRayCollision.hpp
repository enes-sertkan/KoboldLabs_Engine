#pragma once

#include "UsableItem.hpp"
#include <vector>
#include <glm/glm.hpp>
#include "PhysicsManager.h"
#include "aExplosion.hpp"


class aExplodeOnRayCollision : public Action
{
public:
    glm::vec3 rayDirection;
    float rayLenght = 2.0f;

    void Start() override
    {
        rayDirection = glm::normalize(rayDirection);

    }

    void Update() override
    {
        glm::vec3 startPoint = object->mesh->positionXYZ;
        glm::vec3 endPoint = object->mesh->positionXYZ + rayDirection * rayLenght;
        std::vector<sCollision_RayTriangleInMesh> collisions;


        if (object->scene->physicsManager->RayCast(startPoint, endPoint, collisions, false))
        {
            OnImpact();
        }


    }

protected:
    virtual void OnImpact()
    {
        /*object->scene->DrawRay(startPoint, endPoint, program, glm::vec4(100.f, 1.f, 1.f, 1.f), false, 0.1f);*/
        Object* explosion = object->scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_xyz_n_uv.ply", object->mesh->positionXYZ, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), false, glm::vec4(0.1, 0.1, 0.1, 1), true, object->scene->sceneObjects);
        explosion->isTemporary = true;

        aExplosion* explosioning = new aExplosion();
        explosioning->expansionRate = 10.f;
        explosioning->explosionColor = glm::vec4(1.0f, 0.6f, 0.0f, 1.0f);

        object->scene->AddActionToObj(explosioning, explosion);

        // Add explosion or impact marker actions here
        object->scene->RemoveObject(object);


    }
};