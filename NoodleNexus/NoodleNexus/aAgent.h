#pragma once
#include "aBaseMazeCharacter.h"
#include "GOAPAction.h"
#include "GOAPPlanner.h"
#include <unordered_map>
#include <queue>
#include <algorithm>
#include "glm/glm.hpp"
#include "LabAttackFactory.h"
#include "StupidPathFinder.h"



class DamageListener {
public:
    virtual void OnDamage(int damage) = 0;
};

class Agent : public BazeMazeCharacter {
public:

    glm::vec3 colliderCenter = { 0,1.5f,0 };
    LabAttackFactory* factory = nullptr;
    virtual void updateWorldState() = 0; // Must be implemented per enemy type
    void replanIfNeeded();              // Checks if current plan is still valid
    std::queue<glm::vec2> controlPoints;
    StupidPathFinder* pathFinder = new StupidPathFinder();
    // World state (shared + local knowledge)
    std::unordered_map<std::string, bool> worldState;

    // GOAP components
    std::vector<GOAPAction*> availableActions;  // All actions this agent can do
    std::queue<GOAPAction*> currentPlan;        // Current action sequence
    std::unordered_map<std::string, bool> goal; // Current goal (e.g., {"playerInAttackRange", true})

    float attackRange = 0;
    float playerDetectionRange = 5.f;

    float health;
    float maxHealth=10;
    
    virtual void ResetPath()
    {

    }

    // Override from BazeMazeCharacter
    void Start() override {
        health = maxHealth;
        for (glm::vec2 CP : maze->controlPoints)
        {
            controlPoints.push(CP);
     }
    }


    // Override from BazeMazeCharacter
    void Update() override {
        updateWorldState();


        // Dynamic goal switching
        if (IsPlayerInRange() && goal["hasReachedControlPoint"]) {
            goal = { {"playerDamaged", true} }; // Attack mode
            ResetPath();
            currentPlan = std::queue<GOAPAction*>();
        }
        else if (!IsPlayerInAttackRange()&& goal["playerDamaged"])
        {
            if (IsPlayerInRange())
                goal = { {"playerDamaged", true} }; // Attack mode
            else goal = { { "hasReachedControlPoint", true} };

            currentPlan = std::queue<GOAPAction*>();
        }
        else if (!controlPoints.empty()&&!IsPlayerInRange()) {
            goal = { {"hasReachedControlPoint", true} }; // Patrol 
            currentPlan = std::queue<GOAPAction*>();

        }

        if (currentPlan.empty()) {
            currentPlan = GOAPPlanner::plan(this, goal);
        }
        executeCurrentAction();


       
           

    }

    void AddDamageListener(DamageListener* listener) {
        damageListeners.push_back(listener);
    }
    
    void RemoveDamageListener(DamageListener* listener) {
        damageListeners.erase(std::remove(damageListeners.begin(), 
                                        damageListeners.end(), listener), 
                            damageListeners.end());
    }

    void Damage(int damage)  {
        health -= damage;
        for (auto listener : damageListeners) {
            listener->OnDamage(damage);
        }

        if (health <= 0)
            Death();
    }

    virtual void OnDamage(int damage)
    {

    }

    virtual void Death()
    {
        object->Destroy();
    }

    bool IsPlayerInRange()  {
        float dist = glm::distance(object->mesh->positionXYZ,
            maze->player->mesh->positionXYZ);
        return dist <= playerDetectionRange;
    }


    bool IsPlayerInAttackRange() {
        float dist = glm::distance(object->mesh->positionXYZ,
            maze->player->mesh->positionXYZ);
        return dist <= attackRange;
    }



    // Common movement methods
    void MoveTowards(glm::vec3 target) {

        target.y = object->mesh->positionXYZ.y;

        glm::vec3 direction = glm::normalize(target - object->mesh->positionXYZ);
        if (glm::length(direction) < 0.001f) return;

        object->mesh->positionXYZ += direction * speed * 0.1f;//object->scene->deltaTime;

       // RotateTowards(target);
    }

    void MoveAwayFrom(glm::vec3 target) {
        target.y = object->mesh->positionXYZ.y;
        glm::vec3 direction = glm::normalize(object->mesh->positionXYZ - target);
        object->mesh->positionXYZ += direction * speed * 0.1f;//object->scene->deltaTime;

       // RotateTowards(object->mesh->positionXYZ + direction);
    }

    //void RotateTowards(glm::vec3 target) {
    //    glm::vec3 toTarget = target - object->mesh->positionXYZ;
    //    toTarget.y = 0; // Keep rotation on Y axis only

    //    float targetAngle = glm::degrees(atan2(toTarget.x, toTarget.z));
    //    float currentAngle = object->mesh->rotationEulerXYZ.y;

    //    float rotation = glm::mod(targetAngle - currentAngle + 180.0f, 360.0f) - 180.0f;
    //    rotation = glm::clamp(rotation, -rotationSpeed * object->scene->deltaTime,
    //        rotationSpeed * 0.1f);

    //    object->mesh->rotationEulerXYZ.y += rotation;
    //}

    // Basic shooting implementation
    void ShootAtPlayer(float bulletSpeed) {
       // if (currentCooldown > 0) return;

        glm::vec3 toPlayer = maze->player->mesh->positionXYZ - object->mesh->positionXYZ;
        toPlayer = glm::normalize(toPlayer);

        if (factory) {
            Object* bullet = factory->SpawnEnemyBullet(
                object->mesh->positionXYZ + toPlayer * 1.5f,
                toPlayer * bulletSpeed
            );
        }

     //   currentCooldown = shootingCooldown;
    }

    // Damage handling
    virtual void OnHit() {
        health--;
        if (health <= 0) {
            //factory->ReturnEnemy(this); // Assume factory has recycling
            //object->SetActive(false);
        }
    }

    // Helper to check if player is facing this enemy
    bool IsPlayerFacingUs() {
        // Get camera rotation (pitch, yaw) in degrees
        glm::vec2 eyeRotation = object->scene->fCamera->getEyeRotation();
        float yaw = glm::radians(eyeRotation.y);

        // Adjust yaw by subtracting 90 degrees to match movement logic
        float adjustedYaw = yaw - glm::radians(90.0f);

        // Compute the player's forward direction based on adjusted yaw
        glm::vec3 playerForward(sin(adjustedYaw), 0.0f, cos(adjustedYaw));
        playerForward = glm::normalize(playerForward);


        // Vector from player (camera) to enemy
        glm::vec3 toEnemy = glm::normalize(object->mesh->positionXYZ - maze->player->mesh->positionXYZ);

        // Check if dot product is within ~45-degree forward cone
        return glm::dot(playerForward, toEnemy) > 0.7f;
    }

    // Helper to find closest bullet
    Object* FindClosestPlayerBullet() {
        // Implementation would search scene for nearest active player bullet
        // Return nullptr if none in detection radius
        return nullptr;
    }




private:
    std::vector<DamageListener*> damageListeners;
    void executeCurrentAction() {
        if (!currentPlan.empty()) {
            GOAPAction* action = currentPlan.front();
            float deltaTime = (object->scene->deltaTime < 0.5f) ? object->scene->deltaTime : 0.5f;
            if (action->perform(this, deltaTime)) {
                currentPlan.pop();
            }
        }
    }
};