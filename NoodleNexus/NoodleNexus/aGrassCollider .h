#pragma once
#include "sObject.h"
#include <glm/glm.hpp>
#include "Scene.hpp"
#include <GLFW/glfw3.h>
#include "aSoftBodyAction.hpp"

class Object;

class aGrassCollider : public Action {
private:
    Object* grass = nullptr;       // Grass object to manage
    int colliderID = -1;            // ID of created collider
    bool isColliderActive = false;  // Current collider state

public:
    float activationDistance = 15.0f;   // Distance to activate collider
    float deactivationDistance = 18.0f; // Distance to deactivate
    float colliderRadius = 0.8f;       // Collider dimensions
    float colliderBlendRadius = 1.2f;
    
    SoftBody* softBody = nullptr;

    virtual void Start() override {
        colliderID = -1;
        isColliderActive = false;
    }

    virtual void Update() override {
        if (!grass || !grass->mesh || !object || !object->mesh) return;

        // Get positions
        glm::vec3 selfPos = object->mesh->positionXYZ; // Position of object this is attached to
        glm::vec3 grassPos = grass->mesh->positionXYZ;

        // Calculate distance between this object and grass
        float distance = glm::distance(selfPos, grassPos);

        // Collider activation logic
        if (!isColliderActive && distance <= activationDistance) {
            // Create new collider with configured size
            colliderID = grass->mesh->CreateCollider(
                grassPos,
                colliderRadius,
                colliderBlendRadius
            );
            isColliderActive = true;
            std::cout << "Grass collider ID:"<< colliderID<<" activated at distance " << distance << std::endl;
        }
        else if (isColliderActive && distance > deactivationDistance) {
            // Remove existing collider
            grass->mesh->RemoveCollider(colliderID);
            colliderID = -1;
            isColliderActive = false;
            std::cout << "Grass collider deactivated at distance " << distance << std::endl;
        }

        // Update collider position if active
        if (isColliderActive) {
           if( softBody)
            grass->mesh->UpdateColliderPosition(colliderID, object->mesh->positionXYZ+softBody->softBody->getGeometricCentrePoint()*softBody->object->GetWorldScale());
           else
           grass->mesh->UpdateColliderPosition(colliderID, object->mesh->positionXYZ);
        }
    }

    virtual void OnDestroy() override {
        if (grass && grass->mesh && isColliderActive) {
            grass->mesh->RemoveCollider(colliderID);
        }
    }

    // Set which grass object to manage
    void SetGrass(Object* grassObj) {
        grass = grassObj;
        // Reset state if changing grass reference
        if (isColliderActive) {
            grass->mesh->RemoveCollider(colliderID);
            colliderID = -1;
            isColliderActive = false;
        }
    }
};