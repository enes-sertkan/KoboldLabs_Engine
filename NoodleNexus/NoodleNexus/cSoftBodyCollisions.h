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
        glm::vec3 correction(0.0f);

        // Horizontal collision check with maze walls.
        if (mazeGenerator)
        {
            // Convert the world position to grid coordinates.
            glm::vec2 gridPos = mazeGenerator->WorldToGrid(particlePos);
            int gridX = static_cast<int>(gridPos.x);
            int gridY = static_cast<int>(gridPos.y);

            // If the grid cell is a wall, compute the horizontal correction.
            if (mazeGenerator->IsWall(gridX, gridY))
            {
                // The TILE_SIZE is assumed to be 35.0f (i.e. 1.0 * 7.0 * 5.0)
                const float TILE_SIZE = 35.0f;

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
        }

        // Vertical collision check for floor and ceiling.
        // If the particle is below the floor (y = 22), push it up.
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
