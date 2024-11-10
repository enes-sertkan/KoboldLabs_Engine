// Explosion.h
#pragma once
#include "UsableItem.hpp"
#include <string>
#include <iostream>

class aExplosion : public UsableItem
{
public:


    void Start(glm::vec3 position)
    {
        this->position = position;
    }

    void Update()
    {
        // Implement explosion behavior here if needed
    }
};