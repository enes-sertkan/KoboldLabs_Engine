//#pragma once
//
//#include "Action.h"
//#include "Barrel.h"
//#include "Platform.h"
//#include "Character.h"
//
//class BarrelCoreAction : public Action
//{
//public:
//    Barrel* barrel = nullptr;
//    Platform* platform = nullptr;
//    Character* character = nullptr;
//
//    void Start(Barrel* barrelObj, Platform* platformObj, Character* characterObj)
//    {
//        barrel = barrelObj;
//        platform = platformObj;
//        character = characterObj;
//    }
//
//    void Update() override
//    {
//        // Handle initial movement: rolling or dropping based on starting position
//        if (barrel->isOnPlatform())
//        {
//            barrel->startRolling();
//        }
//        else if (barrel->isInAir())
//        {
//            barrel->startDropping();
//        }
//
//        // Update movement based on current state
//        if (barrel->isRolling())
//        {
//            barrel->moveForward(barrel->rollSpeed);
//        }
//        else if (barrel->isDropping())
//        {
//            barrel->applyGravity();
//        }
//
//        // Collision with platform
//        if (barrel->detectPlatformCollision(platform))
//        {
//            barrel->startRolling();
//        }
//
//        // Collision with character
//        if (barrel->detectCharacterCollision(character))
//        {
//            barrel->applyKnockbackToCharacter(character);
//        }
//
//        // Check if barrel goes off-screen
//        if (barrel->isOffScreen())
//        {
//            barrel->resetPosition();
//        }
//    }
//};
