#include "cSoftBodyCollisions.h"
#include "aSoftBodyAction.hpp"
glm::vec3 SoftBodyCollision::ProcessCollisionToOtherSoftBodies(glm::vec3 particlePos)
{
    glm::vec3 posChange = glm::vec3(0);

    for (SoftBody* softBody : otherSoftBodies)
    {

        for (cSoftBodyVerlet::sParticle* particle : softBody->softBody->vec_pParticles)
        {
            particle->accelerationMultiplier = 1.f;
            glm::vec3 worldPos = softBody->object->mesh->positionXYZ + particle->position* softBody->object->mesh->uniformScale;
            float distance = glm::distance(worldPos, particlePos);

            if (distance> particleAffectionRange) continue;
            particle->accelerationMultiplier = 0.f;
            glm::vec3 direction = particlePos - worldPos;
            direction = glm::normalize(direction);
            posChange += collisionMult* 0.005f * direction /distance;
            if (distance > particleAffectionRange / 2) continue;
            particle->position = particle->old_position;

        }
    }

    return posChange;
}