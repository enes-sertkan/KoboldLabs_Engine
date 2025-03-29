#include "GOAPPlanner.h"
#include "aAgent.h"
 std::queue<GOAPAction*> GOAPPlanner::plan(Agent* agent, std::unordered_map<std::string, bool>& goal) {
    std::queue<GOAPAction*> plan;

    // Simple planner: Pick the first valid action that meets the goal
    for (GOAPAction* action : agent->availableActions)
    {
        if (actionValidForGoal(action, goal) &&
            action->checkProceduralPrecondition(agent)) {
            plan.push(action);
            break;
        }
    }
    return plan;
}