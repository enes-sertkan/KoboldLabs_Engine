#pragma once
#include "cSoftBodyVerlet.hpp"

#include "MazeGenerator.hpp"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <thread>
#include <mutex>
class SoftBody;
struct Cylinder
{
    glm::vec3 cylinderPos;
    float cylinderRadius;
    float cylinderHeight;

    //glm::vec3 start;
    //glm::vec3 end;
    //float capRadius;

    //Capsule(glm::vec3 s, glm::vec3 e, float r) : start(s), end(e), capRadius(r) {}

};

class SoftBodyCollision
{
public:
    std::mutex m_CollisionMutex;
    Cylinder* cylinder=nullptr;
    float particleAffectionRange = 1.0f;
    float collisionMult = 1.f;

	MazeGenerator* mazeGenerator = nullptr;
    std::vector<SoftBody*> otherSoftBodies;

    glm::vec3 ProcessCollisionToOtherSoftBodies(glm::vec3 particlePos);

	glm::vec3 ProcessMazeCollision(glm::vec3 particlePos)
	{

        const float TILE_SIZE = mazeGenerator->TILE_SIZE;
        particlePos.x += TILE_SIZE / 2.7f;
        particlePos.z += TILE_SIZE / 1.6f;

        glm::vec3 correction(0.0f);

        // Horizontal collision check with maze walls.
        if (mazeGenerator)
        {
            // Convert the world position to grid coordinates.
            glm::vec2 gridPos = mazeGenerator->WorldToGrid(particlePos);
            int gridX = gridPos.x;
            int gridY = gridPos.y;

            // If the grid cell is a wall, compute the horizontal correction.
            if (mazeGenerator->IsWall(gridY, gridX))
            {
                //std::cout << "inside the wall gridX: " << gridX << " gridY: " << gridY << std::endl;
                // The TILE_SIZE is assumed to be 35.0f (i.e. 1.0 * 7.0 * 5.0)
             

                // Determine cell boundaries in world space.
                float cellMinX = gridX * TILE_SIZE;
                float cellMaxX = (gridX + 1) * TILE_SIZE;
                float cellMinZ = gridY * TILE_SIZE;
                float cellMaxZ = (gridY + 1) * TILE_SIZE;

                // Calculate distances from the particle position to each side of the cell.
                float distLeft = particlePos.x - cellMinX;
                float distRight = cellMaxX - particlePos.x;
                float distBack = particlePos.z - cellMinZ;
                float distFront = cellMaxZ - particlePos.z;

                // Determine which side has the smallest penetration.
                float minDist = distLeft;
                correction.x = -distLeft;  // push left

                if (distRight < minDist)
                {
                    minDist = distRight;
                    correction.x = distRight;  // push right
                    correction.z = 0.0f;
                }
                if (distBack < minDist)
                {
                    minDist = distBack;
                    correction.x = 0.0f;
                    correction.z = -distBack;  // push backward (negative z)
                }
                if (distFront < minDist)
                {
                    minDist = distFront;
                    correction.x = 0.0f;
                    correction.z = distFront;  // push forward (positive z)
                }
            }


            if (mazeGenerator->IsFloor(gridY, gridX))
                if (particlePos.y < 3.f)
                {
                    correction.y = 3.f - particlePos.y;
                }
            // If above the ceiling (y = 62), push it down.
                else if (particlePos.y > 62.0f)
                {
                    //       correction.y = 62.0f - particlePos.y;
                }

        }

        // Vertical collision check for floor and ceiling.
        // If the particle is below the floor (y = 22), push it up.
      
        return correction;
	}

    glm::vec3 ProcessCapsuleCollision(glm::vec3 particlePos)
    {
        glm::vec3 correction = glm::vec3(0);
        glm::vec3 particlePositionXZ = glm::vec3(particlePos.x, 0, particlePos.z);
        glm::vec3 cylinderPositionXZ = glm::vec3(cylinder->cylinderPos.x, 0, cylinder->cylinderPos.z);

        float distanceBetween = glm::distance(particlePositionXZ, cylinderPositionXZ);

        if (distanceBetween > cylinder->cylinderRadius)
        {
            glm::vec3 direction = cylinder->cylinderPos - particlePos;
            direction = glm::normalize(direction);
            
            float moveMagnitude = distanceBetween - cylinder->cylinderRadius;
            correction += moveMagnitude * direction;

        }

        if (particlePos.y > cylinder->cylinderHeight + cylinder->cylinderPos.y)
        {
            float distance = particlePos.y - cylinder->cylinderHeight - cylinder->cylinderPos.y;

            correction.y = -distance;

        }

        if (particlePos.y <  cylinder->cylinderPos.y)
        {
            float distance = particlePos.y - cylinder->cylinderPos.y;

            correction.y = -distance;

        }

        return correction;
    }

    //glm::vec3 ClosestPointOnLineSegment(glm::vec3 A, glm::vec3 B, glm::vec3 P)
    //{
    //    glm::vec3 AB = B - A;
    //    float t = glm::dot(P - A, AB) / glm::dot(AB, AB);
    //    t = glm::clamp(t, 0.0f, 1.0f);
    //    return A + t * AB;
    //}

    //glm::vec3 ProcessCapsuleCollision(glm::vec3 particlePos)
    //{
    //    glm::vec3 closestPoint = ClosestPointOnLineSegment(capsule->start, capsule->end, particlePos);

    //    glm::vec3 toParticle = particlePos - closestPoint;
    //    float distSq = glm::dot(toParticle, toParticle);
    //    float radiusSq = capsule->capRadius * capsule->capRadius;

    //    if (distSq < radiusSq)
    //    {
    //        float dist = glm::sqrt(distSq);
    //        glm::vec3 normal = dist > 0 ? toParticle / dist : glm::vec3(1.0f, 0.0f, 0.0f);
    //        return closestPoint + normal * capsule->capRadius;
    //    }

    //    return particlePos;
    //}
};
