#pragma once

#include "Action.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include "PhysicsManager.h"
#include "Scene.hpp"
#include <cstdlib> // For random number generation
#include <ctime>   // To seed the random number generator
#include "aRayCastPhysics2D.hpp"
#include "aModelsFramesAnimator.hpp"

class aRollDown : public Action {
public:
    float descentSpeed = 10.0f;  // Speed for descending a ladder
    glm::vec3 velocity = glm::vec3(0.0f); // Tracks current speed
    bool isDescending = false;  // Whether the barrel is currently descending
    bool canUseLadder = false;  // Whether this barrel can detect ladders
    bool usedFirstLadder = false; // Tracks if the first ladder was used
    bool skippedFirst = false;
    float raySpacing = -10.0f; 
    float rayHeight = -8.f;
    bool skippedLadder = false;
    int rollDownChance = 50;
    int rayAmmont = 2;
    bool isOnGround = false;  // Whether the barrel has landed on the ground
    bool isMovingDown = true;  // Direction flag for movement after hitting the ground
    bool onLadder = false;
    bool isOn = true;
    aRayCastPhysics2D* phys = nullptr;
    glm::vec3 savedBarrelSpeed;
    glm::vec3 savedBarrelGrav;
    glm::vec4     originalColor = glm::vec4(0.f, 1.f, 0.f, 1.f);
    aModelsFramesAnimator* animator = nullptr;
    float rayLength = 10.0f;

    aRollDown() {
        std::srand(static_cast<unsigned>(std::time(0))); // Seed the RNG

        // Assign 10% chance for this barrel to use ladders
        isOn = (std::rand() % 100) < 70;
    }

    // Ladder detection using horizontal rays
    bool DetectLadder(Scene* scene, glm::vec3 position) {
       

        std::vector<sCollision_RayTriangleInMesh> collisions;
        glm::vec4 gridRayColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue for horizontal rays
        float rayScale = 0.1f;
        
        

        for (int i = 0; i < rayAmmont; i++)
        {
            // Horizontal grid ray detection
            glm::vec3 right = glm::vec3(0, 0, 1); // Direction along Z-axis
            glm::vec3 rayStart = position + glm::vec3(1.8f, rayHeight, 0.0f) - right * rayLength / 2.0f;
            glm::vec3 rayEnd = rayStart + right * rayLength;


            // Draw the ray for debugging
          //  scene->DrawRay(rayStart, rayEnd, scene->programs[0], gridRayColor, true, rayScale);

            // Perform raycast
            if (scene->physicsManager->RayCast(rayStart, rayEnd, collisions, false)) {
                return true;
            }

            position.y += raySpacing;
        }

      

        return false; // No ladder detected
    }





    void Update() override {




        if (!isOn) {
            return;
        }



        glm::vec3 position = object->mesh->positionXYZ;
     
        if (skippedLadder)
        {
            if (!DetectLadder(object->scene, position))
            {
                skippedLadder = false;
            }
            return;
        }

        if (!onLadder)
        {

            // Ladder detection and descent logic
            if (DetectLadder(object->scene, position)) {
              
                if ((std::rand() % 100) < rollDownChance) {
                    StartRollingOnLadder();
                    onLadder = true;
                }
                else
                {
                    skippedLadder = true;
                }


                //if (!usedFirstLadder) {
                //    StartRollingOnLadder();
                //    onLadder = true;
                //    usedFirstLadder = true;
                //}
                //else {
                //    // 50% chance to descend additional ladders
                //    if ((std::rand() % 100) < 50) {
                //        StartRollingOnLadder();
                //        onLadder = true;
                //    }
                //    else
                //    {
                //        isOn = false;
                //    }

                
            }
        }

        // Disable physics (no gravity) while on the ladder
        if (onLadder) {

            

            if (!DetectLadder(object->scene, position))
            {
                onLadder = false;

                StopRollingOnLadder();
            }

            RollOnLadder();
        }

        else

        {
       
        }
            
        

       
    }

    void StartRollingOnLadder()
    {
        //Set Position on Barrel to ladder pos

        if (phys)
        {
            phys->isOn = false;
            savedBarrelSpeed = phys->speed;
            phys->speed = glm::vec3(0, 0, 0);
            savedBarrelGrav = phys->gravityAcceleration;
            phys->gravityAcceleration = glm::vec3(0.f, 0.000000001f, 0.f);

           if (savedBarrelSpeed.z>0)
           {
               object->mesh->positionXYZ.z += rayLength ;
           }
           else
           {
               object->mesh->positionXYZ.z -= rayLength;
           }

        }


        if (animator)
        {
            animator->ChangeAnimation(1); // 1 = Climb Down Anim
        }

        object->mesh->rotationEulerXYZ = glm::vec3(0.f, 0.f, 0.f);
    }   

    void RollOnLadder()
    {
        object->mesh->positionXYZ.y -= descentSpeed;

    }

    void StopRollingOnLadder()
    {
        //Set Position on Barrel to ladder pos

        if (phys)
        {
            phys->isOn = true;;
            savedBarrelSpeed.z = -savedBarrelSpeed.z;
            phys->speed = savedBarrelSpeed;
            phys->gravityAcceleration = savedBarrelGrav;
       
        }

        if (animator)
        {
            animator->ChangeAnimation(0);// 0 = Roll Anim
        }

    }

    void DrawRays(Scene* scene, glm::vec3 position, glm::vec3 forward, float climbHeight, GLuint program) {
        glm::vec4 rayColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        float rayScale = 0.1f;

        // Forward ray for ladder detection
        glm::vec3 rayStart = position + 0.5f;
        glm::vec3 rayEnd = rayStart + forward * 6.0f;
        //scene->DrawRay(rayStart, rayEnd, program, rayColor, true, rayScale);

        // Ground ray for collision detection
        rayStart = position;
        rayEnd = position + glm::vec3(0.0f, -3.3f, 0.0f) * raySpacing;
        //scene->DrawRay(rayStart, rayEnd, program, rayColor, true, rayScale);

        // Side rays for wall detection
        glm::vec3 sideRayEndLeft = position + glm::vec3(0.0f, 0.0f, -3.0f) * raySpacing;
        glm::vec3 sideRayEndRight = position + glm::vec3(0.0f, 0.0f, 3.0f) * raySpacing;
        scene->DrawRay(position, sideRayEndLeft, program, rayColor, true, rayScale);
        scene->DrawRay(position, sideRayEndRight, program, rayColor, true, rayScale);
    }
};
