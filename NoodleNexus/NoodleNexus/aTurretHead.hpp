// aTurretHead.hpp
#pragma once
#include "Action.h"
#include "LabAttackFactory.h"
#include <glm/gtc/quaternion.hpp>

class aTurretHead : public Action
{
private:
    float m_shootCooldown = 1.0f; // Seconds between shots
    float m_timeSinceLastShot = 0.0f;

public:
    LabAttackFactory* factory = nullptr; // Should be set by turret creation

    void Update() override
    {
        if (!object || !factory) return;

        m_timeSinceLastShot += object->scene->deltaTime;

        if (m_timeSinceLastShot >= m_shootCooldown)
        {
            Shoot();
            m_timeSinceLastShot = 0.0f;
        }
    }

    void Shoot()
    {
        // Get head's forward direction
        glm::vec3 rotationDegrees = object->GetWorldRotation();
        glm::vec3 rotationRadians = glm::radians(rotationDegrees);
        glm::quat rotation = glm::quat(rotationRadians);
        glm::vec3 forward = rotation * glm::vec3(0.0f, 0.0f, 1.0f);

        // Get shoot position (center of head)
        glm::vec3 shootPosition = object->GetWorldPosition();

        // Create bullet with direction
        float bulletSpeed = 10.0f;
        factory->SpawnPlayerBullet(shootPosition, forward * bulletSpeed);
    }

    Action* Clone() const override
    {
        aTurretHead* clone = new aTurretHead(*this);
        clone->factory = factory; 
        clone->m_timeSinceLastShot = 0.0f;
        return clone;
    }
};