#pragma once
#include "aBaseMazeCharacter.h"
#include "GOAPAction.h"
#include "GOAPPlanner.h"
#include <unordered_map>
#include <queue>
#include <algorithm>

class Agent : public BazeMazeCharacter {
public:
    virtual void updateWorldState() = 0; // Must be implemented per enemy type
    void replanIfNeeded();              // Checks if current plan is still valid

    // World state (shared + local knowledge)
    std::unordered_map<std::string, bool> worldState;

    // GOAP components
    std::vector<GOAPAction*> availableActions;  // All actions this agent can do
    std::queue<GOAPAction*> currentPlan;        // Current action sequence
    std::unordered_map<std::string, bool> goal; // Current goal (e.g., {"playerInAttackRange", true})

    float attackRange = 0;
   

    // Override from BazeMazeCharacter
    void Update() override {
        updateWorldState();
        if (currentPlan.empty()) {
            currentPlan = GOAPPlanner::plan(this, goal);
        }
        executeCurrentAction();
    }

private:
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