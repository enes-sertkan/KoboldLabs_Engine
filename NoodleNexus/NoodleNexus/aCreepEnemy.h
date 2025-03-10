#pragma once
#include "aLACharacter.h"
#include <functional>  // Make sure to include this for std::function

class aCreepEnemy : public aLACharacter {
public:
    void Start() override
    {
        aLACharacter::Start();
        // Initial state set to Idle
        stateMachine->ChangeState(std::bind(&aCreepEnemy::MoveAwayFromPlayerState, this, std::placeholders::_1)); //bleeehhhh
    }


    void MoveAwayFromPlayerState(StateMachine* stateMachine)
    {
        if (!IsPlayerFacingUs()) {
            stateMachine->ChangeState(std::bind(&aCreepEnemy::MoveToPlayerState, this, std::placeholders::_1));
            return;
        }

            moveSpeed = 10.f;
            MoveAwayFrom(player->mesh->positionXYZ);

    }

    // State functions with the correct signature
    void MoveToPlayerState(StateMachine* stateMachine)
    {
        if (IsPlayerFacingUs()) {
            stateMachine->ChangeState(std::bind(&aCreepEnemy::MoveAwayFromPlayerState, this, std::placeholders::_1));
            return;
        }

            moveSpeed = 20.f;
            MoveTowards(player->mesh->positionXYZ);
    }


 

};