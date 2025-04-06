#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "sMesh.h"
#include "Transform.h"
#include <vector>
#include "Action.h"



class Scene;


//TODO: Change File Name, bc now it's a class.
class Object
{

public:
    std::string name;
    sMesh* mesh;
    Scene* scene;
    Transform* startTranform = new Transform;
    bool isTemporary = false;
    bool isCollisionStatic = false;
    std::vector<std::string> tags;
    bool isActive = true;


    std::vector<Object*> m_children;  // The proud parent's little ones
    Object* m_parent = nullptr;       // Who raised you, object?

    //GLuint program;

    void Destroy();

    std::vector<Action*> actions;
    std::vector<Object*> sceneObjects;


    // ====== PARENT-CHILD FUNCTIONS ======
    void AddChild(Object* child) {
        if (!child) return;

        // Capture the child's current world transforms
        glm::vec3 childWorldPos = child->GetWorldPosition();
        glm::vec3 childWorldRot = child->GetWorldRotation();
        float childWorldScale = child->GetWorldScale();

        // Capture the parent's current world transforms
        glm::vec3 parentWorldPos = this->GetWorldPosition();
        glm::vec3 parentWorldRot = this->GetWorldRotation();
        float parentWorldScale = this->GetWorldScale();

        // Remove from previous parent
        if (child->m_parent) {
            child->m_parent->RemoveChild(child);
        }

        // Adjust the child's local transforms to maintain world position
        child->mesh->positionXYZ = childWorldPos - parentWorldPos;
        child->mesh->rotationEulerXYZ = childWorldRot - parentWorldRot;
        child->mesh->uniformScale = childWorldScale / parentWorldScale;

        //LOCAL TRANFORM FIX
        childWorldPos = child->startTranform->position;
        childWorldRot = child->startTranform->rotation;
        childWorldScale = child->startTranform->scale.x;

     
        child->startTranform->position = childWorldPos - parentWorldPos;
        child->startTranform->rotation = childWorldRot - parentWorldRot;
        child->startTranform->scale.x = childWorldScale / parentWorldScale;


        // Add to this parent
        m_children.push_back(child);
        child->m_parent = this;
    }

    void AddChildRaw(Object* child) {
        if (!child) return;

       

        // Remove from previous parent
        if (child->m_parent) {
            child->m_parent->RemoveChild(child);
        }

        // Add to this parent
        m_children.push_back(child);
        child->m_parent = this;
    }

    void RemoveChild(Object* child) {
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end()) {
            // Capture world transforms BEFORE removing parent relationship
            glm::vec3 worldPos = child->GetWorldPosition();
            glm::vec3 worldRot = child->GetWorldRotation();
            float worldScale = child->GetWorldScale();

            // Remove parent-child relationship
            m_children.erase(it);
            child->m_parent = nullptr;

            // Update child's local transforms to maintain world position
            child->mesh->positionXYZ = worldPos;
            child->mesh->rotationEulerXYZ = worldRot;
            child->mesh->uniformScale = worldScale;


            //LOCAL TRANFORM FIX
            worldPos = child->startTranform->position;
            worldRot = child->startTranform->rotation;
            worldScale = child->startTranform->scale.x;

            child->startTranform->position = worldPos + this->GetWorldPosition();
            child->startTranform->rotation = worldRot + this->GetWorldRotation();
            child->startTranform->scale.x = worldScale * this->GetWorldScale();
        }
    }

    //CRYING!! NO TIME TO DO OPTIMISED VERSION:((
    // Get world position (slow, recalculates every call)
    glm::vec3 GetWorldPosition() {
        if (!mesh) return glm::vec3(0);
        if (m_parent && m_parent->mesh) {
            return m_parent->GetWorldPosition() + mesh->positionXYZ;
        }
        return mesh->positionXYZ;
    }

    // Get world rotation (slow, recalculates every call)
    glm::vec3 GetWorldRotation() {
        if (m_parent) {
            return m_parent->GetWorldRotation() + mesh->rotationEulerXYZ;
        }
        return mesh->rotationEulerXYZ;
    }

    glm::quat GetWorldRotationQuat()  {
        // Convert degrees to radians
        glm::vec3 radians = glm::radians(GetWorldRotation());

        // Create rotation matrix using XYZ rotation order
        glm::mat4 rotationMatrix = glm::eulerAngleXYZ(
            radians.x,  // X-axis rotation (pitch)
            radians.y,  // Y-axis rotation (yaw)
            radians.z   // Z-axis rotation (roll)
        );

        // Convert matrix to quaternion
        return glm::quat_cast(rotationMatrix);
    }


    // Get world scale (slow, recalculates every call)
    float GetWorldScale() {
        if (m_parent) {
            return m_parent->GetWorldScale() * mesh->uniformScale;
        }
        return mesh->uniformScale;
    }


    void RemoveParent() {
        if (m_parent) {
            // Let the parent handle the detachment and transform updates
            m_parent->RemoveChild(this);
        }
    }


};
