#include "MoveToControlPointAction.h"
#include "aAgent.h"

bool MoveToControlPointAction::checkProceduralPrecondition(Agent* agent) {
    // Only valid if we have control points to visit
    return !agent->controlPoints.empty();
}



bool MoveToControlPointAction::perform(Agent* agent, float deltaTime) {
    if (m_ReachedFinalPoint) return true;


    // If no path, generate new one to first control point
    if (m_CurrentPath.empty() && !agent->controlPoints.empty()) {
        glm::vec2 target = agent->controlPoints.front();

        agent->pathFinder->maze = agent->maze;
        glm::vec3 mazePos = agent->GetMazePosition();


        m_CurrentPath = agent->pathFinder->FindPath(mazePos, target);

        //if (m_CurrentPath.empty()) {
        //    // Invalid path, remove control point
        //    agent->controlPoints.pop();
        //    return false;
        //}

        m_CurrentPathPoint = 0;
    }

    if (agent->controlPoints.empty())
    {
        m_ReachedFinalPoint = true;
        return true;
    }

    // Move through path points
    if (m_CurrentPathPoint < m_CurrentPath.size()) {
        glm::vec3 targetPoint;
        targetPoint.x = m_CurrentPath[m_CurrentPathPoint].x;
        targetPoint.z = m_CurrentPath[m_CurrentPathPoint].y;
        targetPoint.y = agent->GetMazePosition().y;

      
        agent->targetWorldPosition = agent->maze->GridToWorld(targetPoint.x, targetPoint.z);
        agent->targetWorldPosition.y = agent->object->mesh->positionXYZ.y;

        glm::vec3 mazePosition = agent->GetMazePosition();
        float distance = glm::distance(agent->object->mesh->positionXYZ, agent->targetWorldPosition);

        // Check if reached current path point
        if (distance < REACH_DISTANCE) {
            m_CurrentPathPoint++;
        }
        else {

            agent->targetWorldPosition = agent->maze->GridToWorld(targetPoint.x, targetPoint.z);
            agent->targetWorldPosition.y = agent->object->mesh->positionXYZ.y;

            // Move towards current path point
          /*  if (mazePosition.x < targetPoint.x) agent->Move(CRIGHT);
            else if (mazePosition.x > targetPoint.x) agent->Move(CLEFT);

            if (mazePosition.z < targetPoint.z) agent->Move(CUP);
            else if (mazePosition.z > targetPoint.z) agent->Move(CDOWN);
        }*/
        }
    }
    else {
        // Reached final control point
        agent->controlPoints.pop();
        m_CurrentPath.clear();
    }

    agent->MoveToTargetPositionUpdate();


    return false;
}

void MoveToControlPointAction::reset() {
    m_CurrentPath.clear();
    m_CurrentPathPoint = 0;
    m_ReachedFinalPoint = false;
}