// aTurretHead.hpp
#pragma once
#include "Action.h"
#include "LabAttackFactory.h"
#include <glm/gtc/quaternion.hpp>
#include "aTurretPart.h"
#include "sObject.h"
#include "AudioManager.hpp"

class aTurretHead : public aTurretPart
{
private:

    float m_timeSinceLastShot = 0.0f;
    size_t m_currentBarrel = 0;

    // Recoil parameters
    glm::vec3 m_originalLocalPosition;
    float m_currentRecoil = 0.0f;
   

    // Cached rotation quaternion for frame consistency
    glm::quat m_currentWorldRotation;

public:
    float m_shootCooldown = 0.6f;
    float m_recoilDistance = 0.4/2.5f;
    float m_recoilRecoverySpeed = 1.5f;

    void Start() override {
        aTurretPart::Start();
        m_originalLocalPosition = object->mesh->positionXYZ;
    }

    void Update() override
    {
        if (!object || !factory) return;

        if (!turret->target && !turret->constantShooting) return;

        // Get world rotation once per frame
        m_currentWorldRotation = object->GetWorldRotationQuat();

        // Handle recoil recovery
        if (m_currentRecoil > 0.0f) {
            m_currentRecoil -= m_recoilRecoverySpeed * object->scene->deltaTime;
            m_currentRecoil = glm::max(m_currentRecoil, 0.0f);
            glm::vec3 recoilOffset =  glm::vec3(0.0f, 0.0f, -m_currentRecoil);

            // Apply recoil in local space backward direction
            object->mesh->positionXYZ = m_originalLocalPosition + recoilOffset;
        }

        m_timeSinceLastShot += object->scene->deltaTime;

        if (m_timeSinceLastShot >= m_shootCooldown)
        {
            Shoot();
            //audio::AudioManager::Instance().PlayOneShot3D("assets/sounds/RocketFlyBy.wav", object->GetWorldPosition(), {0, 0, 0}, 1.0f);

            audio::AudioManager::Instance().Load2DSound("FlyingMissile", "assets/sounds/RocketFlyBy.wav");
            audio::AudioManager::Instance().Play2DSound("FlyingMissile");
            m_timeSinceLastShot = 0.0f;
        }

    }

    virtual void Shoot()
    {
        std::vector<glm::vec3> m_barrelPositions = turret->head->barrelsPos;
        if (m_barrelPositions.empty()) return;

        // Apply recoil using cached rotation
        m_currentRecoil = m_recoilDistance;

        // Get current barrel position using frame-consistent rotation
        glm::vec3 localBarrelPos = m_barrelPositions[m_currentBarrel];
     
        glm::vec3 worldBarrelPos = object->GetWorldPosition() +
            m_currentWorldRotation * (localBarrelPos - glm::vec3(0.0f, 0.0f, m_currentRecoil* object->GetWorldScale()));

        // Get shoot direction using cached rotation
        glm::vec3 forward = m_currentWorldRotation * glm::vec3(0.0f, 0.0f, 1.0f);

        factory->SpawnPlayerBullet(worldBarrelPos, forward * 15.0f, true);

        m_currentBarrel = (m_currentBarrel + 1) % m_barrelPositions.size();
    }

    aTurretHead* Clone() const override
    {
        aTurretHead* clone = new aTurretHead(*this);
        clone->factory = factory;
        clone->m_timeSinceLastShot = 0.0f;
        clone->m_currentBarrel = 0;
        return clone;
    }
};