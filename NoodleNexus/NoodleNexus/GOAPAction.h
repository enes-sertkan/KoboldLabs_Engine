#pragma once
#include "glm/glm.hpp"
#include <unordered_map>
#include <string>

class Agent;

class GOAPAction {
public:
    virtual ~GOAPAction() = default;

    // Must implement these in each action
    virtual bool checkProceduralPrecondition(Agent* agent) = 0;
    virtual bool perform(Agent* agent, float deltaTime) = 0;

    virtual void reset() = 0;

    // Preconditions & effects (used by planner)
    std::unordered_map<std::string, bool> preconditions;
    std::unordered_map<std::string, bool> effects;
    float cost = 1.0f; // Lower cost = better action
};