#pragma once

#include "UsableItem.hpp"
#include <vector>
#include <glm/glm.hpp>
#include "PhysicsManager.h"
#include "aExplosion.hpp"
#include "aRayCastPhysics.h"


class aExplodeOnRayCollision : public Action
{
public:
    glm::vec3 rayDirection;
    RayCastPhysics* phys = nullptr;

    float rayLenght = 2.0f;

    void Start() override
    {
        rayDirection = glm::normalize(rayDirection);

    }

    void Update() override
    {
        if (phys != nullptr)
            rayDirection = glm::normalize(phys->speed);

        glm::vec3 startPoint = object->mesh->positionXYZ;
        glm::vec3 endPoint = object->mesh->positionXYZ + rayDirection * rayLenght;
        std::vector<sCollision_RayTriangleInMesh> collisions;


        if (object->scene->physicsManager->RayCast(startPoint, endPoint, collisions, false))
        {
            OnImpact(collisions[0]);
        }


    }

protected:
    virtual void OnImpact(sCollision_RayTriangleInMesh collision)
    {
        /*object->scene->DrawRay(startPoint, endPoint, program, glm::vec4(100.f, 1.f, 1.f, 1.f), false, 0.1f);*/
        Object* explosion = object->scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_xyz_n_uv.ply", collision.vecTriangles[0].intersectionPoint, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), true, glm::vec4(1.0f, 0.6f, 0.0f, 1.0f), false, object->scene->sceneObjects);
        explosion->isTemporary = true;

        aExplosion* explosioning = new aExplosion();
        explosioning->expansionRate = 10.f;
       

        object->scene->AddActionToObj(explosioning, explosion);

        // Add explosion or impact marker actions here
        object->scene->RemoveObject(object);

        Object* explosionHitMark = object->scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_xyz_n_uv.ply", collision.vecTriangles[0].intersectionPoint, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f),true, glm::vec4(0.3, 0.3, 0.3, 1), false, object->scene->sceneObjects);
        explosionHitMark->isTemporary = true;
    }
};