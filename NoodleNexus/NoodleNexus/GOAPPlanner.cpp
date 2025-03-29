#include "GOAPPlanner.h"
#include "aAgent.h"
#include <algorithm>

// Custom comparator for priority queue
struct NodeCompare {
    bool operator()(const GOAPPlanner::Node* a, const GOAPPlanner::Node* b) const {
        return a->totalCost() > b->totalCost();
    }
};

// Custom hash for node states
struct StateHash {
    size_t operator()(const std::unordered_map<std::string, bool>& state) const {
        size_t hash = 0;
        for (const auto& pair : state) {
            hash ^= std::hash<std::string>{}(pair.first) +
                (pair.second ? 1 : 0);
        }
        return hash;
    }
};


// Custom equality for node states
struct StateEqual {
    bool operator()(const std::unordered_map<std::string, bool>& a,
        const std::unordered_map<std::string, bool>& b) const {
        return a == b;
    }
};


std::queue<GOAPAction*> GOAPPlanner::plan(Agent* agent,
    const std::unordered_map<std::string, bool>& goal) {

    typedef std::unordered_map<
        std::unordered_map<std::string, bool>,
        Node*,
        StateHash,
        StateEqual
    > NodeMap;

    std::priority_queue<Node*, std::vector<Node*>, NodeCompare> openSet;
    NodeMap allNodes;

    // Create initial node
    Node* start = new Node();
    start->state = agent->worldState;
    start->parent = nullptr;
    start->action = nullptr;
    start->runningCost = 0.0f;
    start->heuristic = calculateHeuristic(start, goal);
    openSet.push(start);
    allNodes[start->state] = start;

    Node* solution = nullptr;

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        // Check if current node satisfies the goal
        if (stateMatchesGoal(current, goal)) {
            solution = current;
            break;
        }

        // Get all usable actions for current state
        std::vector<GOAPAction*> usableActions;
        for (GOAPAction* action : agent->availableActions) {
            bool preconditionsMet = true;
            for (const auto& precond : action->preconditions) {
                auto it = current->state.find(precond.first);
                if (it == current->state.end() || it->second != precond.second) {
                    preconditionsMet = false;
                    break;
                }
            }
            if (preconditionsMet) {
                usableActions.push_back(action);
            }
        }

        // Process each usable action
        for (GOAPAction* action : usableActions) {
            if (!action->checkProceduralPrecondition(agent)) continue;

            // Create new state by applying action effects
            std::unordered_map<std::string, bool> newState = current->state;
            for (const auto& effect : action->effects) {
                newState[effect.first] = effect.second;
            }

            // Calculate costs
            float newCost = current->runningCost + action->cost;
            float heuristic = calculateHeuristic(current, goal);

            // Check if we already have this state
            NodeMap::iterator existing = allNodes.find(newState);
            if (existing != allNodes.end()) {
                // If existing path is better, skip this
                if (existing->second->runningCost <= newCost) {
                    continue;
                }
            }

            // Create new node
            Node* newNode = new Node();
            newNode->state = newState;
            newNode->parent = current;
            newNode->action = action;
            newNode->runningCost = newCost;
            newNode->heuristic = heuristic;

            openSet.push(newNode);
            allNodes[newState] = newNode;
        }
    }

    // Build action queue
    std::queue<GOAPAction*> actionQueue;
    if (solution) {
        std::vector<GOAPAction*> reversePlan;
        Node* currentNode = solution;

        while (currentNode->parent != nullptr) {
            if (currentNode->action) {
                reversePlan.push_back(currentNode->action);
            }
            currentNode = currentNode->parent;
        }

        std::reverse(reversePlan.begin(), reversePlan.end());
        for (GOAPAction* action : reversePlan) {
            actionQueue.push(action);
        }
    }

    // Cleanup memory
    for (NodeMap::iterator it = allNodes.begin(); it != allNodes.end(); ++it) {
        delete it->second;
    }

    return actionQueue;
}



// Updated helper functions
bool GOAPPlanner::stateMatchesGoal(const Node* node,
    const std::unordered_map<std::string, bool>& goal) {
    for (const auto& g : goal) {
        auto it = node->state.find(g.first);
        if (it == node->state.end() || it->second != g.second) return false;
    }
    return true;
}

float GOAPPlanner::calculateHeuristic(const Node* node,
    const std::unordered_map<std::string, bool>& goal) {
    if (!node) return 0.0f;

    float count = 0;
    for (const auto& g : goal) {
        auto it = node->state.find(g.first);
        if (it == node->state.end() || it->second != g.second) count++;
    }
    return count;
}


