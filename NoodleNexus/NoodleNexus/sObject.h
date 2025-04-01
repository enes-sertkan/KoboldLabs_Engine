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

        // Add to this parent
        m_children.push_back(child);
        child->m_parent = this;
    }

    void RemoveChild(Object* child) {
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end()) {
            m_children.erase(it);
            child->m_parent = nullptr;
        }
    }

    // Get world position (slow, recalculates every call)
    glm::vec3 GetWorldPosition() {
        if (m_parent) {
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

    // Get world scale (slow, recalculates every call)
    float GetWorldScale() {
        if (m_parent) {
            return m_parent->GetWorldScale() * mesh->uniformScale;
        }
        return mesh->uniformScale;
    }


    void RemoveParent() {
        if (m_parent) {
            // Store current parent reference
            Object* oldParent = m_parent;

            // Capture world transforms BEFORE parent removal
            glm::vec3 worldPosition = GetWorldPosition();
            glm::vec3 worldRotation = GetWorldRotation();
            float worldScale = GetWorldScale();

            // Remove from parent's children (this sets m_parent to nullptr)
            oldParent->RemoveChild(this);

            // Convert to independent object by setting local transforms to world values
            mesh->positionXYZ = worldPosition;
            mesh->rotationEulerXYZ = worldRotation;
            mesh->uniformScale = worldScale;
        }
    }

};
