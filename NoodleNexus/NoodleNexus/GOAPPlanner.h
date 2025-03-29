#pragma once
#include <queue>
#include "GOAPAction.h"
class Agent;
class GOAPPlanner {
public:
    static std::queue<GOAPAction*> plan(Agent* agent, std::unordered_map<std::string, bool>& goal);
private:
    static bool actionValidForGoal(GOAPAction* action, const std::unordered_map<std::string, bool>& goal) {
        for (auto& g : goal) {
            auto it = action->effects.find(g.first);
            if (it == action->effects.end() || it->second != g.second) return false;
        }
        return true;
    }
};