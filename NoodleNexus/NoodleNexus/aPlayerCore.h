#pragma once
#define NOMINMAX  // Disable Windows min/max macros
#include <windows.h>  // If you need Windows headers
#include "Action.h"
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>
#include "MazeGenerator.hpp"
#include <algorithm> 
#include "aPlayerMovement.h"
#include "AudioManager.hpp"


class aPlayerCore : public Action {
private:
 
    glm::vec3 respawnPosition;  // Where player teleports when they die
    bool isInvulnerable = false;
    float invulnerabilityTimer = 0.0f;
    const float INVULNERABILITY_DURATION = 2.0f; // 2 seconds of invulnerability after respawn
    const float DAMAGE_INVULNERABILITY = 0.5f;    // After taking damage

public:
    aPlayerMovement* playerMovement = nullptr;

    glm::vec3 colliderCenter = glm::vec3(0);
    float colliderRadius = 1.f;
    float health = 20.f;
    float maxHealth = 20.f;
    bool isDead = false;

    void Start() override {
        // Initialize respawn position to the object's starting position
        respawnPosition = object->GetWorldPosition();
    }

    void Update() override {
        // Handle invulnerability cooldown
        if (isInvulnerable) {
            invulnerabilityTimer -= object->scene->deltaTime;
            if (invulnerabilityTimer <= 0.0f) {
                isInvulnerable = false;
            }
        }

        // Check for death
        if (health <= 0.0f && !isDead) {
            Die();
        }
    }

    void Damage(float damage) {
        if (isDead || isInvulnerable) return;
        health -= damage;
        health = (std::max)(health, 0.0f);  // Extra parentheses prevent macro expansion
        audio::AudioManager::Instance().Load2DSound("Hurt", "assets/sounds/roblox.flac");
        audio::AudioManager::Instance().Play2DSound("Hurt");
        playerMovement->DamageJump();


    }

    void Heal(float amount) {
        health = (std::min)(health + amount, maxHealth);
    }

    void Die() {
        isDead = true;
        health = 0.0f;

        // Teleport to respawn position
        object->mesh->positionXYZ = respawnPosition;

        // Reset health after respawn
        health = maxHealth;
        isDead = false;

        // Set temporary invulnerability
        isInvulnerable = true;
        invulnerabilityTimer = INVULNERABILITY_DURATION;


        // You could also trigger particle effects here
        // object->scene->SpawnParticles("death_explosion", object->GetWorldPosition());
    }

    void SetRespawnPosition(const glm::vec3& newPosition) {
        respawnPosition = newPosition;
    }

    void FullHeal() {
        health = maxHealth;
    }

    // Percentage of health remaining (0.0 to 1.0)
    float GetHealthPercentage() const {
        return health / maxHealth;
    }
};