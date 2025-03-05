#pragma once
#include "MazeGenerator.hpp"
#include <algorithm>
#include <vector>

#define NOMINMAX
#include <Windows.h>
#include <process.h>

class StupidPathFinder
{
public:
    glm::vec2 movementDirections[4] = { glm::vec2(1, 0), glm::vec2(-1, 0), glm::vec2(0, 1), glm::vec2(0, -1) };
    MazeGenerator* maze;

    CRITICAL_SECTION cs;
    HANDLE foundEvent;
    HANDLE timeoutEvent;
    volatile bool pathFound;
    volatile bool timeoutTriggered;
    std::vector<glm::vec2> foundPath;
    volatile long pathsChecked;

    StupidPathFinder() : pathFound(false), timeoutTriggered(false), pathsChecked(0) {
        InitializeCriticalSection(&cs);
        foundEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        timeoutEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    }

    ~StupidPathFinder() {
        DeleteCriticalSection(&cs);
        CloseHandle(foundEvent);
        CloseHandle(timeoutEvent);
    }

    struct ThreadParams {
        StupidPathFinder* finder;
        glm::vec2 startPos;
        glm::vec2 targetPos;
        unsigned int seed;
    };

    static inline int thread_rand(unsigned int* seed) {
        *seed = (*seed * 1103515245 + 12345) % 2147483648;
        return (int)*seed;
    }

    std::vector<glm::vec2> FindPath(glm::vec2 startPos, glm::vec2 targetPos) {
        std::cout << "Searching with threading for possible paths" << std::endl;

        EnterCriticalSection(&cs);
        pathFound = false;
        timeoutTriggered = false;
        foundPath.clear();
        pathsChecked = 0;
        LeaveCriticalSection(&cs);
        ResetEvent(foundEvent);
        ResetEvent(timeoutEvent);

        const int numThreads = 50;
        HANDLE threads[numThreads];

        for (int i = 0; i < numThreads; ++i) {
            ThreadParams* params = new ThreadParams;
            params->finder = this;
            params->startPos = startPos;
            params->targetPos = targetPos;
            params->seed = GetTickCount() + i;

            unsigned threadID;
            threads[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, params, 0, &threadID);
            if (threads[i] == NULL) {
                delete params;
                std::cerr << "Failed to create thread " << i << std::endl;
            }
        }

        HANDLE waitEvents[2] = { foundEvent, timeoutEvent };
        DWORD waitResult = WaitForMultipleObjects(2, waitEvents, FALSE, INFINITE);

        if (waitResult == WAIT_OBJECT_0 + 1) { // Timeout triggered
            EnterCriticalSection(&cs);
            foundPath.clear();
            LeaveCriticalSection(&cs);
        }

        WaitForMultipleObjects(numThreads, threads, TRUE, INFINITE);

        for (int i = 0; i < numThreads; ++i) {
            CloseHandle(threads[i]);
        }

        std::cout << "Total # of paths checked: " << pathsChecked << std::endl;
        return foundPath;
    }

    static unsigned __stdcall ThreadFunc(void* param) {
        ThreadParams* params = (ThreadParams*)param;
        StupidPathFinder* finder = params->finder;
        unsigned int seed = params->seed;

        while (true) {
            EnterCriticalSection(&finder->cs);
            bool found = finder->pathFound;
            bool timeout = finder->timeoutTriggered;
            LeaveCriticalSection(&finder->cs);

            if (found || timeout) {
                break;
            }

            std::vector<glm::vec2> path = finder->LookForPath(params->startPos, params->targetPos, &seed);

            if (!path.empty() && path.back().x == params->targetPos.x && path.back().y == params->targetPos.y) {
                EnterCriticalSection(&finder->cs);
                if (!finder->pathFound) {
                    finder->pathFound = true;
                    finder->foundPath = path;
                    SetEvent(finder->foundEvent);
                }
                LeaveCriticalSection(&finder->cs);
                break;
            } else {
                long newChecks = InterlockedIncrement(&finder->pathsChecked);
                if (newChecks >= 110000) {
                    EnterCriticalSection(&finder->cs);
                    if (!finder->timeoutTriggered) {
                        finder->timeoutTriggered = true;
                        SetEvent(finder->timeoutEvent);
                    }
                    LeaveCriticalSection(&finder->cs);
                }
                seed = thread_rand(&seed);
            }
        }

        delete params;
        return 0;
    }

    std::vector<glm::vec2> LookForPath(glm::vec2 startPos, glm::vec2 targetPos, unsigned int* seed) {
        std::vector<glm::vec2> currentPath;
        currentPath.clear();
        glm::vec2 curPos = startPos;

        if (CheckTile(curPos.x, curPos.y, targetPos)) {
            currentPath.push_back(curPos);
            return currentPath;
        }

        do {
            std::vector<glm::vec2> possibleTiles = FindPossibleTiles(curPos.x, curPos.y, currentPath);
            if (possibleTiles.empty()) {
                return std::vector<glm::vec2>();
            }

            int tileID = thread_rand(seed) % possibleTiles.size();
            glm::vec2 nextTile = possibleTiles[tileID];

            if (CheckTile(nextTile.x, nextTile.y, targetPos)) {
                currentPath.push_back(nextTile);
                return currentPath;
            }

            currentPath.push_back(nextTile);
            curPos = nextTile;
        } while (true);
    }

    bool CheckTile(int x, int y, glm::vec2 targetPos) {
        return (targetPos.x == x && targetPos.y == y);
    }

    std::vector<glm::vec2> FindPossibleTiles(int x, int y, std::vector<glm::vec2> currentPath) {
        std::vector<glm::vec2> possibleTiles;
        for (glm::vec2 dir : movementDirections) {
            if (IsTileCheckable(x + dir.x, y + dir.y, currentPath))
                possibleTiles.push_back(glm::vec2(x + dir.x, y + dir.y));
        }
        return possibleTiles;
    }

    bool IsTileCheckable(int x, int y, std::vector<glm::vec2> currentPath) {
        if (maze->IsWall(y, x)) return false;
        if (std::find(currentPath.begin(), currentPath.end(), glm::vec2(x, y)) != currentPath.end()) {
            return false;
        }
        return true;
    }
};