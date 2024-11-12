//#pragma once
//
//#include "Action.h"
//#include "Character.h"
//#include "Ladder.h"
//
//class LadderUseAction : public Action
//{
//public:
//    Character* character = nullptr;
//    Ladder* ladder = nullptr;
//    bool isClimbing = false;
//
//    void Start(Character* characterObj, Ladder* ladderObj)
//    {
//        character = characterObj;
//        ladder = ladderObj;
//    }
//
//    void Update() override
//    {
//        // Check if character is near the ladder and facing it
//        if (character->isNear(ladder) && character->isFacing(ladder))
//        {
//            // Start climbing if character is on the ground and pressing the "up" key
//            if (character->isOnGround() && glfwGetKey(character->scene->window, GLFW_KEY_UP) == GLFW_PRESS)
//            {
//                character->startClimbing(ladder);
//                isClimbing = true;
//            }
//
//            // If already climbing, move character up or down depending on input
//            if (isClimbing)
//            {
//                if (glfwGetKey(character->scene->window, GLFW_KEY_UP) == GLFW_PRESS)
//                {
//                    character->moveUp(ladder->climbSpeed);
//                }
//                else if (glfwGetKey(character->scene->window, GLFW_KEY_DOWN) == GLFW_PRESS)
//                {
//                    character->moveDown(ladder->climbSpeed);
//                }
//            }
//        }
//        else
//        {
//            // Stop climbing if character is not near or not facing ladder
//            character->stopClimbing();
//            isClimbing = false;
//        }
//    }
//};
