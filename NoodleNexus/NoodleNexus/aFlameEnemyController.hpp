#pragma once

#include "Action.h"
#include "Scene.hpp"
#include "aRayCastPhysics2D.hpp"
#include "aModelsFramesAnimator.hpp"
#include "aRayCastPhysics2D.hpp"

class aFlameEnemyController : public Action
{
public:


    float timeToChangeDirection=20.f;
    float timer=0;

    aModelsFramesAnimator* animator = new aModelsFramesAnimator();
    aRayCastPhysics2D* physics = new aRayCastPhysics2D();

    void Update() override
    {
        timer += object->scene->deltaTime;

        if (timer > timeToChangeDirection)
        {
            if (animator && physics)
            {
                animator->lookingRight = -animator->lookingRight;
                physics->speed = -physics->speed;
                timer = 0;
            }
        }
    }
};
