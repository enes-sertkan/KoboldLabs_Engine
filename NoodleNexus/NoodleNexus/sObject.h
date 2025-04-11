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
    bool fakeParent = false;
    std::string name;
    sMesh* mesh;
    Scene* scene;
    Transform* startTranform = new Transform;
    bool isTemporary = false;
    bool isCollisionStatic = false;
    std::vector<std::string> tags;
    bool isActive = true;
    bool useRotationMatrix = false;

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

    glm::mat4 GetLocalTransform() {
        // Translation matrix from local position
        glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), mesh->positionXYZ);
        // Rotation matrix from Euler angles (converted to radians)
        glm::mat4 rotationMat = glm::eulerAngleXYZ(
            glm::radians(mesh->rotationEulerXYZ.x),
            glm::radians(mesh->rotationEulerXYZ.y),
            glm::radians(mesh->rotationEulerXYZ.z)
        );
        // Scale matrix (uniform scaling)
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(mesh->uniformScale));

        // Combine: Translation * Rotation * Scale
        return translationMat * rotationMat * scaleMat;
    }

    // Build the world transformation matrix by composing parent's transformation if available
    glm::mat4 GetWorldTransform() {
        // Start with the local transform
        glm::mat4 localTransform = GetLocalTransform();

        // If there is a valid parent, the world transform is parent's world transform multiplied by local
        if (m_parent) {
            if (m_parent->fakeParent)  return  localTransform;



            return m_parent->GetWorldTransform() * localTransform;
        }
        return localTransform;
    }

    // Get the world position by extracting the translation part from the world matrix
    glm::vec3 GetWorldPosition() {
        // If no mesh is attached, return zero vector
        if (!mesh) return glm::vec3(0.0f);

        // Compute the full world transform
        glm::mat4 worldTransform = GetWorldTransform();
        // The translation is in the 4th column of the 4x4 matrix (ignoring w component)
        return glm::vec3(worldTransform[3]);
    }

    // Get the world rotation quaternion directly from the world transformation matrix
    glm::quat GetWorldRotationQuat() {
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

    // Get the world rotation as Euler angles (degrees),
    // by converting the quaternion from the world transform to Euler angles.
    glm::vec3 GetWorldRotation() {
        // Get world transformation matrix
        glm::mat4 worldTransform = GetWorldTransform();

        // Extract rotation matrix (ignore scale and translation)
        glm::mat3 rotationMatrix(worldTransform);

        // Extract Euler angles in XYZ order (same as original)
        glm::vec3 radians;
        glm::extractEulerAngleXYZ(glm::mat4(rotationMatrix), radians.x, radians.y, radians.z);

        return glm::degrees(radians);
    }

    float GetWorldScale() {
        if (!mesh) return 1.0f;

        float localScale = mesh->uniformScale;

        // Recursively multiply parent's scale if it exists
        if (m_parent) {
            return m_parent->GetWorldScale() * localScale;
        }

        return localScale;
    }



    void RemoveParent() {
        if (m_parent) {
            // Let the parent handle the detachment and transform updates
            m_parent->RemoveChild(this);
        }
    }

    Object* Clone();
  


    void RemoveAction(Action* action)
    {
        std::vector<Action*>::iterator it = std::find(actions.begin(), actions.end(), action);
        if (it != actions.end())
        {
            delete* it; // Free the memory if dynamically allocated
            actions.erase(it); // Remove from vector
        }
    }

    
};
