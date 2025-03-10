#pragma once;
#include "Action.h"
#include "LabAttackFactory.h"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>


class StateMachine;  // Forward declaration

using StateFunction = std::function<void(StateMachine*)>;

class StateMachine {
public:
    void ChangeState(StateFunction newStateFunction) {
        currentStateFunction = newStateFunction;
    }

    void Update() {
        if (currentStateFunction) {
            currentStateFunction(this);  // Call the current state's function, passing `this` as the parameter
        }
    }

private:
    StateFunction currentStateFunction;
};


class aLACharacter: public Action
{
protected:

    float moveSpeed = 10.f;
    float rotationSpeed=100.f;
    float shootingCooldown;
    float currentCooldown;
    int health;

 
  

public:
    
    StateMachine* stateMachine = new StateMachine();

    Object* player = nullptr;
    LabAttackFactory* factory = nullptr;


    // Initialize common enemy properties
    virtual void Start() override {


        moveSpeed = 3.0f;
        rotationSpeed = 180.0f;
        shootingCooldown = 2.0f;
        currentCooldown = 0.0f;
        health = 3;
    }

    virtual void Update() override {
        if (!player) return;

        // Base update handles cooldowns
        if (currentCooldown > 0)
            currentCooldown -= object->scene->deltaTime;

        stateMachine->Update();

  //      MoveTowards(player->mesh->positionXYZ);
     
    }

    // Common movement methods
    void MoveTowards( glm::vec3 target) {

        target.y = object->mesh->positionXYZ.y;

        glm::vec3 direction = glm::normalize(target - object->mesh->positionXYZ);
        if (glm::length(direction) < 0.001f) return;
          
        object->mesh->positionXYZ += direction * moveSpeed * 0.1f;//object->scene->deltaTime;

       RotateTowards(target);
    }

    void MoveAwayFrom(glm::vec3 target) {
        target.y = object->mesh->positionXYZ.y;
        glm::vec3 direction = glm::normalize(object->mesh->positionXYZ - target);
        object->mesh->positionXYZ += direction * moveSpeed * 0.1f;//object->scene->deltaTime;

        RotateTowards(object->mesh->positionXYZ + direction);
    }

    void RotateTowards(glm::vec3 target) {
        glm::vec3 toTarget = target - object->mesh->positionXYZ;
        toTarget.y = 0; // Keep rotation on Y axis only

        float targetAngle = glm::degrees(atan2(toTarget.x, toTarget.z));
        float currentAngle = object->mesh->rotationEulerXYZ.y;

        float rotation = glm::mod(targetAngle - currentAngle + 180.0f, 360.0f) - 180.0f;
        rotation = glm::clamp(rotation, -rotationSpeed * object->scene->deltaTime,
            rotationSpeed * 0.1f);

        object->mesh->rotationEulerXYZ.y += rotation;
    }

    // Basic shooting implementation
    void ShootAtPlayer(float bulletSpeed) {
        if (currentCooldown > 0) return;

        glm::vec3 toPlayer = player->mesh->positionXYZ - object->mesh->positionXYZ;
        toPlayer = glm::normalize(toPlayer);

        if (factory) {
            Object* bullet = factory->SpawnEnemyBullet(
                object->mesh->positionXYZ + toPlayer * 1.5f,
                toPlayer * bulletSpeed
            );
        }

        currentCooldown = shootingCooldown;
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
        glm::vec3 toEnemy = glm::normalize(object->mesh->positionXYZ - player->mesh->positionXYZ);

        // Check if dot product is within ~45-degree forward cone
        return glm::dot(playerForward, toEnemy) > 0.7f;
    }

    // Helper to find closest bullet
    Object* FindClosestPlayerBullet() {
        // Implementation would search scene for nearest active player bullet
        // Return nullptr if none in detection radius
        return nullptr;
    }



    //BASE STATES


};


