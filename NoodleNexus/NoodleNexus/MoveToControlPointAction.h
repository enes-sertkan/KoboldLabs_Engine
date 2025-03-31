#pragma once
#include "GOAPAction.h"
#include "StupidPathFinder.h"

class MoveToControlPointAction : public GOAPAction {
public:
    MoveToControlPointAction() {
        // No preconditions needed
        effects["hasReachedControlPoint"] = true;
        cost = 1.0f; // Lower than other actions to prioritize
    }

    bool checkProceduralPrecondition(Agent* agent) override;
    bool perform(Agent* agent, float deltaTime) override;
 
    void reset() override;

private:
    std::vector<glm::vec2> m_CurrentPath;
    int m_CurrentPathPoint = 0;
    bool m_ReachedFinalPoint = false;
    const float REACH_DISTANCE = 0.5f; // Grid cell proximity
};