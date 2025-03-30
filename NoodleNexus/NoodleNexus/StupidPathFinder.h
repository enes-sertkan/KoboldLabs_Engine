#pragma once
#include "MazeGenerator.hpp"
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set> // Added for closed set
#include <iostream>

class StupidPathFinder {
public:
    glm::vec2 movementDirections[4] = { glm::vec2(1, 0), glm::vec2(-1, 0), glm::vec2(0, 1), glm::vec2(0, -1) };
    MazeGenerator* maze;
    long pathsChecked = 0; // Counter for tested paths

    struct Node {
        glm::vec2 position;
        float cost;
        float heuristic;
        Node* parent;

        bool operator<(const Node& other) const {
            return (cost + heuristic) > (other.cost + other.heuristic);
        }
    };

    struct vec2Hash {
        size_t operator()(const glm::vec2& v) const {
            return std::hash<float>()(v.x) ^ (std::hash<float>()(v.y) << 1);
        }
    };

    std::vector<glm::vec2> FindPath(glm::vec2 startPos, glm::vec2 targetPos) {
        pathsChecked = 0; // Reset counter
        auto cmp = [](Node* left, Node* right) { return *left < *right; };
        std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> openSet(cmp);
        std::unordered_map<glm::vec2, Node*, vec2Hash> allNodes;
        std::unordered_set<glm::vec2, vec2Hash> closedSet; // Added closed set

        Node* startNode = new Node{ startPos, 0, heuristic(startPos, targetPos), nullptr };
        openSet.push(startNode);
        allNodes[startPos] = startNode;

        while (!openSet.empty()) {
            Node* current = openSet.top();
            openSet.pop();

            // Skip if already processed
            if (closedSet.find(current->position) != closedSet.end()) {
                continue;
            }

            closedSet.insert(current->position); // Mark as processed
            pathsChecked++;

            if (current->position == targetPos) {
                std::vector<glm::vec2> path;
                while (current != nullptr) {
                    path.push_back(current->position);
                    current = current->parent;
                }
                std::reverse(path.begin(), path.end());

                // Cleanup
                for (auto& pair : allNodes) delete pair.second;
                std::cout << "A* tested " << pathsChecked << " nodes to find path ("
                    << path.size() << " steps)" << std::endl;
                return path;
            }

            // Explore neighbors
            for (glm::vec2 dir : movementDirections) {
                glm::vec2 neighborPos = current->position + dir;

                if (maze->IsWall(neighborPos.y, neighborPos.x)) continue;

                float newCost = current->cost + 1;
                Node* neighborNode = nullptr;
                auto it = allNodes.find(neighborPos);

                if (it == allNodes.end()) {
                    neighborNode = new Node{
                        neighborPos,
                        newCost,
                        heuristic(neighborPos, targetPos),
                        current
                    };
                    allNodes[neighborPos] = neighborNode;
                    openSet.push(neighborNode);
                }
                else if (newCost < it->second->cost) {
                    it->second->cost = newCost;
                    it->second->parent = current;
                    openSet.push(it->second); // Re-add to update priority
                }

                // If neighbor is in closedSet but a better path is found, 
                // A* with consistent heuristic doesn't need to re-open it.
            }
        }

        // Cleanup if no path found
        for (auto& pair : allNodes) delete pair.second;
        std::cout << "A* tested " << pathsChecked << " nodes (no path found)" << std::endl;
        return {};
    }

private:
    float heuristic(glm::vec2 a, glm::vec2 b) {
        return abs(a.x - b.x) + abs(a.y - b.y);
    }
};