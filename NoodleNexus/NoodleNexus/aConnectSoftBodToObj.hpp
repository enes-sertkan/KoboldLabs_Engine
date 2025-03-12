#pragma once
#include "Action.h"
#include "aSoftBodyAction.hpp"

class ConnSoftToObj : public Action
{
private:
    glm::vec3 m_initialOffset;         // Original position difference
    bool m_bInitialized = false;

public:
    SoftBody* softbody = nullptr;
    bool applyRotation = true;         // Control rotation influence
    bool maintainRelativePosition = true;

    virtual void Start()
    {
        
    }
    // make position of the object to position of the vertices


    virtual void Update()
    {
        object->mesh->positionXYZ = softbody->GetParticlePosition(30);
    }
};