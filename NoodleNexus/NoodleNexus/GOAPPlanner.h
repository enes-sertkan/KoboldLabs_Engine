#pragma once

#include <queue>
#include <unordered_map>
#include <vector>
#include <functional>
#include "GOAPAction.h"
#include <string>
#include <sstream>

class Agent;

class GOAPPlanner {
public:
   static std::queue<GOAPAction*> plan(Agent* agent,
        const std::unordered_map<std::string, bool>& goal);


    struct Node {
        std::unordered_map<std::string, bool> state;
        Node* parent;
        GOAPAction* action;
        float runningCost;
        float heuristic;

        float totalCost() const { return runningCost + heuristic; }
    };

private:
  
    // Helper functions
    static bool stateMatchesGoal(const Node* node,
        const std::unordered_map<std::string, bool>& goal);
    static float calculateHeuristic(const Node* node,
        const std::unordered_map<std::string, bool>& goal);
    static std::vector<GOAPAction*> getUsableActions(Agent* agent, const Node* node);
    static void applyActionEffects(std::unordered_map<std::string, bool>& newState,
        const GOAPAction* action);
};
