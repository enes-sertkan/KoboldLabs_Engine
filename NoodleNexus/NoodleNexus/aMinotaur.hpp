// aMinotaur.hpp
#pragma once
#include "Action.h"
#include "MazeGenerator.hpp"
#include "aTheseus.hpp"

class aMinotaur : public Action {
public:
    glm::ivec2 gridPosition;
    MazeGenerator* maze = nullptr;
    aTheseus* theseus = nullptr;

    void Update() override {
        // AI movement logic here
    }
};