#pragma once
#include "MazeGenerator.hpp"
#include <algorithm>

class StupidPathFinder
{
public:

	glm::vec2 movementDirections[4] = { glm::vec2(1, 0), glm::vec2(-1, 0), glm::vec2(0, 1), glm::vec2(0, -1) };

	std::vector<glm::vec2> path;
	glm::vec2 targetPos;
	glm::vec2 currentTile;

	MazeGenerator* maze;



	std::vector<glm::vec2> FindPath(glm::vec2 startPos, glm::vec2 _targetPos)
	{
		std::cout << "Searching for path" << std::endl;
		std::vector<glm::vec2> currentPath;
		currentPath.clear();
		int pathNum = 0;
		while (currentPath.empty())
		{
			pathNum++;
			targetPos = _targetPos;
			currentPath = LookForPath(startPos);
			
		}
		std::cout << "Path Searh Ammount: " << pathNum <<" Size: "<<currentPath.size()<< std::endl;
		return currentPath;
	}

	std::vector<glm::vec2> LookForPath(glm::vec2 startPos)
	{
		std::vector<glm::vec2> currentPath;
		currentPath.clear(); // Clear previous path
		glm::vec2 curPos = startPos;
		if (CheckTile(curPos.x, curPos.y)) // Check start position
			return currentPath;


		do
		{

			std::vector<glm::vec2> possibleTiles = FindPossibleTiles(curPos.x, curPos.y, currentPath);
			if (possibleTiles.empty())
			{
				std::cout << "Tried Path. Size: " << currentPath.size() << std::endl;
				return possibleTiles;
			}


			int tileID = rand() % possibleTiles.size();

			if (CheckTile(possibleTiles[tileID].x, possibleTiles[tileID].y))
			{
				currentPath.push_back(glm::vec2(possibleTiles[tileID].x, possibleTiles[tileID].y));
				return currentPath;
			}


			currentPath.push_back(glm::vec2(possibleTiles[tileID].x, possibleTiles[tileID].y));


			curPos.x = possibleTiles[tileID].x;
			curPos.y = possibleTiles[tileID].y;

			} while (true);
		

	}


	std::vector<glm::vec2> FindPossibleTiles(int x, int y, std::vector<glm::vec2> currentPath)
	{
		std::vector<glm::vec2> possibleTiles;
		for(glm::vec2 dir:movementDirections)
		{
			if (IsTileCheckable(x + dir.x, y + dir.y,currentPath))
				possibleTiles.push_back(glm::vec2(x + dir.x, y + dir.y));
		}

		return possibleTiles;
	}

	bool CheckTile(int x, int y)
	{
	

		if (targetPos.x == x && targetPos.y == y)
		{
			return true;
		}



		return false;
	}

	bool IsTileCheckable(int x, int y, std::vector<glm::vec2> currentPath)
	{
		if (maze->IsWall(y, x)) return false;

		if (std::find(currentPath.begin(), currentPath.end(), glm::vec2(x,y)) != currentPath.end()) {
			return false;
		}

		return true; // Added return
	}


};

