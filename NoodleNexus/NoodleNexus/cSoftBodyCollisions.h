#pragma once
#include "MazeGenerator.hpp"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

class SoftBodyCollision
{
public:
	MazeGenerator* mazeGenerator = nullptr;

	glm::vec3 ProcessMazeCollision(glm::vec3& particlePos)
	{
        // Make sure we have a valid mazeGenerator.
        if (!mazeGenerator) {
            return glm::vec3(0.0f);
        }

        // Convert the particle's world position to grid coordinates.
        glm::vec2 gridPos = mazeGenerator->WorldToGrid(particlePos);
        int gridX = static_cast<int>(gridPos.x);
        int gridY = static_cast<int>(gridPos.y);

        // If this grid cell is not a wall, then no collision response is needed.
        if (!mazeGenerator->IsWall(gridX, gridY))
        {
            return glm::vec3(0.0f);
        }

        // Assume the TILE_SIZE is defined as in WorldToGrid (1.0f * 7.0f * 5.0f = 35.0f)
        //THATS REALLY BAD TODO FIX
        const float TILE_SIZE = 35.0f;

        // Calculate the boundaries of the cell in world coordinates.
        float cellMinX = gridX * TILE_SIZE;
        float cellMaxX = (gridX + 1) * TILE_SIZE;
        float cellMinZ = gridY * TILE_SIZE;
        float cellMaxZ = (gridY + 1) * TILE_SIZE;

        // Determine distances from the particle position to each cell boundary.
        float distLeft = particlePos.x - cellMinX;
        float distRight = cellMaxX - particlePos.x;
        float distBottom = particlePos.z - cellMinZ;
        float distTop = cellMaxZ - particlePos.z;

        // Determine the smallest penetration distance and choose the correction direction.
        float minDist = distLeft;
        glm::vec3 correction(-distLeft, 0.0f, 0.0f);  // push left

        if (distRight < minDist)
        {
            minDist = distRight;
            correction = glm::vec3(distRight, 0.0f, 0.0f);  // push right
        }
        if (distBottom < minDist)
        {
            minDist = distBottom;
            correction = glm::vec3(0.0f, 0.0f, -distBottom);  // push backwards (negative z)
        }
        if (distTop < minDist)
        {
            minDist = distTop;
            correction = glm::vec3(0.0f, 0.0f, distTop);  // push forwards (positive z)
        }

        //FOR FLOOR AND CEILING
        if (particlePos.y < 22.0f)
        {
            correction.y = 22.0f - particlePos.y;
        }
        // If above the ceiling (y = 62), push it down.
        else if (particlePos.y > 62.0f)
        {
            correction.y = 62.0f - particlePos.y;
        }




        return correction;
	}

};
