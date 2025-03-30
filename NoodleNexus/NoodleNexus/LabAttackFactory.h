#pragma once
#ifndef LAB_ATTACK_FACTORY_HPPm
#define LAB_ATTACK_FACTORY_HPP

#include <glm/vec3.hpp>
#include <vector>
#include "Scene.hpp"
#include "sObject.h"
#include "aProjectileMovement.hpp"
#include "MazeGenerator.hpp"

class LabAttackFactory {
public:
    Object* player = nullptr;
    MazeGenerator* maze = nullptr;
    // Constructor with default initial pool sizes
    LabAttackFactory(int creepPoolSize = 10, int avoiderPoolSize = 10, int shooterPoolSize = 10, int wandererPoolSize = 10,
        int playerBulletPoolSize = 20, int enemyBulletPoolSize = 20);
    ~LabAttackFactory();



    Scene* scene = nullptr;

    // Spawn functions:
    // For enemies, only a position is needed.
    Object* SpawnCreep(const glm::vec3 position);
    Object* SpawnAvoider(const glm::vec3& position);
    Object* SpawnShooter(const glm::vec3& position);
    Object* SpawnWanderer(const glm::vec3& position);
    Object* SpawnBrut(const glm::vec3& position);

    // For bullets, a position and a speed vector are provided.
    Object* SpawnPlayerBullet(const glm::vec3& position, const glm::vec3& speed);
    Object* SpawnEnemyBullet(const glm::vec3& position, const glm::vec3& speed);


    std::vector<Object*>         m_creepPool;
    std::vector<Object*>       m_avoiderPool;
    std::vector<Object*>       m_shooterPool;
    std::vector<Object*>      m_wandererPool;
    std::vector<Object*>  m_playerBulletPool;
    std::vector<Object*>   m_enemyBulletPool;
private:
    // Object pools for each type.
   

    // Current sizes (or capacities) for the pools
    int m_creepPoolSize;
    int m_avoiderPoolSize;
    int m_shooterPoolSize;
    int m_wandererPoolSize;
    int m_playerBulletPoolSize;
    int m_enemyBulletPoolSize;

};

#endif 