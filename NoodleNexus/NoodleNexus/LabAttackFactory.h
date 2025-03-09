#pragma once
#ifndef LAB_ATTACK_FACTORY_HPP
#define LAB_ATTACK_FACTORY_HPP

#include <glm/vec3.hpp>
#include <vector>
#include "Scene.hpp"
#include "sObject.h"
#include "aProjectileMovement.hpp"


// Forward declarations for enemy and bullet classes
class Creep;
class Avoider;
class Shooter;
class Wanderer;
class PlayerBullet;
class EnemyBullet;

class LabAttackFactory {
public:

    // Constructor with default initial pool sizes
    LabAttackFactory(int creepPoolSize = 10, int avoiderPoolSize = 10, int shooterPoolSize = 10, int wandererPoolSize = 10,
        int playerBulletPoolSize = 20, int enemyBulletPoolSize = 20);
    ~LabAttackFactory();



    Scene* scene = nullptr;
    // Call this once (or as needed) to preallocate object pools
    void InitializePools();

    // Set pool sizes (if you want to adjust them later)
    void SetCreepPoolSize(int size);
    void SetAvoiderPoolSize(int size);
    void SetShooterPoolSize(int size);
    void SetWandererPoolSize(int size);
    void SetPlayerBulletPoolSize(int size);
    void SetEnemyBulletPoolSize(int size);

    // Spawn functions:
    // For enemies, only a position is needed.
    Object* SpawnCreep(const glm::vec3& position);
    Object* SpawnAvoider(const glm::vec3& position);
    Object* SpawnShooter(const glm::vec3& position);
    Object* SpawnWanderer(const glm::vec3& position);

    // For bullets, a position and a speed vector are provided.
    Object* SpawnPlayerBullet(const glm::vec3& position, const glm::vec3& speed);
    Object* SpawnEnemyBullet(const glm::vec3& position, const glm::vec3& speed);

    // Return objects to the pool (for recycling)
    void ReturnCreep(Creep* creep);
    void ReturnAvoider(Avoider* avoider);
    void ReturnShooter(Shooter* shooter);
    void ReturnWanderer(Wanderer* wanderer);
    void ReturnPlayerBullet(PlayerBullet* bullet);
    void ReturnEnemyBullet(EnemyBullet* bullet);

private:
    // Object pools for each type.
    std::vector<Creep*>         m_creepPool;
    std::vector<Avoider*>       m_avoiderPool;
    std::vector<Shooter*>       m_shooterPool;
    std::vector<Wanderer*>      m_wandererPool;
    std::vector<PlayerBullet*>  m_playerBulletPool;
    std::vector<EnemyBullet*>   m_enemyBulletPool;

    // Current sizes (or capacities) for the pools
    int m_creepPoolSize;
    int m_avoiderPoolSize;
    int m_shooterPoolSize;
    int m_wandererPoolSize;
    int m_playerBulletPoolSize;
    int m_enemyBulletPoolSize;

    // Helper functions to create new objects (when pool is empty or to expand the pool)
    Creep* CreateCreep();
    Avoider* CreateAvoider();
    Shooter* CreateShooter();
    Wanderer* CreateWanderer();
    PlayerBullet* CreatePlayerBullet();
    EnemyBullet* CreateEnemyBullet();

    // Template helper function to expand any pool by a given additional count.
    template<typename T>
    void ExpandPool(std::vector<T*>& pool, int additionalCount);
};

#endif // LAB_ATTACK_FACTORY_HPP
