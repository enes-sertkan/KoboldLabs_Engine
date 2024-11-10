// Projectile.h
#pragma once
#include "UsableItem.hpp"
#include <vector>
#include <glm/glm.hpp>

class aProjectile : public UsableItem
{
public:
    glm::vec3 target;
    glm::vec3 velocity;
    std::vector<glm::vec3> exhaustTrail;

    // Starts the projectile
    void Start(glm::vec3 startPosition, glm::vec3 targetPosition)
    {
        position = startPosition;
        target = targetPosition;
        velocity = glm::normalize(target - position) * 5.0f; 
    }

    void Update()
    {
        position += velocity;
        exhaustTrail.push_back(position);

        if (glm::distance(position, target) < 1.0f) 
        {
            ExplosionEffect();
        }
    }

    void ExplosionEffect()
    {
        // Explosion logic
        std::cout << "Boom!" << std::endl;
    }

    //create new action checkDirectionAndExplode
    //When use
    //Create 1(!!) object
    //Make it temporarily
    //You give this object all actions it need
};
