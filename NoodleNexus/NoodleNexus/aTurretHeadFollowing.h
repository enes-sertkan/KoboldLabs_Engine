#pragma once
#include "aTurretHead.hpp"
#include "LabAttackFactory.h"

class aTurretHeadFollowing : public aTurretHead {
public:
    float m_timeSinceLastShot = 0.0f;
    size_t m_currentBarrel = 0;

    // Recoil parameters
    glm::vec3 m_originalLocalPosition;
    float m_currentRecoil = 0.0f;

    // Cached rotation quaternion for frame consistency
    glm::quat m_currentWorldRotation;

    void Shoot() override {

        if (!object || !factory) return;

        // Get world rotation once per frame
        m_currentWorldRotation = object->GetWorldRotationQuat();

        std::vector<glm::vec3> m_barrelPositions = turret->head->barrelsPos;
        if (m_barrelPositions.empty()) return;

        // Apply recoil using cached rotation
        m_currentRecoil = m_recoilDistance;

        // Get current barrel position using frame-consistent rotation
        glm::vec3 localBarrelPos = m_barrelPositions[m_currentBarrel];
        glm::vec3 worldBarrelPos = object->GetWorldPosition() +
            m_currentWorldRotation * (localBarrelPos - glm::vec3(0.0f, 0.0f, m_currentRecoil * object->GetWorldScale()));

        // Get shoot direction using cached rotation
        glm::vec3 forward = m_currentWorldRotation * glm::vec3(0.0f, 0.0f, 1.0f);

        // Changed to spawn following bullets
        factory->SpawnPlayerFollowingBullet(worldBarrelPos, forward * 7.5f, true);

        m_currentBarrel = (m_currentBarrel + 1) % m_barrelPositions.size();
    }

    aTurretHeadFollowing* Clone() const override {
        aTurretHeadFollowing* clone = new aTurretHeadFollowing(*this);
        clone->factory = factory;
        clone->m_timeSinceLastShot = 0.0f;
        clone->m_currentBarrel = 0;
        return clone;
    }
};