#include "LabAttackFactory.h"

// Constructor
LabAttackFactory::LabAttackFactory(int creepPoolSize, int avoiderPoolSize, int shooterPoolSize, int wandererPoolSize,
    int playerBulletPoolSize, int enemyBulletPoolSize)
    : m_creepPoolSize(creepPoolSize), m_avoiderPoolSize(avoiderPoolSize), m_shooterPoolSize(shooterPoolSize),
    m_wandererPoolSize(wandererPoolSize), m_playerBulletPoolSize(playerBulletPoolSize), m_enemyBulletPoolSize(enemyBulletPoolSize)
{
}

// Destructor
LabAttackFactory::~LabAttackFactory()
{
    // Here you would typically iterate through each pool and delete the allocated objects.
}

// Preallocate the object pools.
void LabAttackFactory::InitializePools()
{
    // Empty implementation.
}

// Setters for pool sizes (if needed to adjust dynamically)
void LabAttackFactory::SetCreepPoolSize(int size) { m_creepPoolSize = size; }
void LabAttackFactory::SetAvoiderPoolSize(int size) { m_avoiderPoolSize = size; }
void LabAttackFactory::SetShooterPoolSize(int size) { m_shooterPoolSize = size; }
void LabAttackFactory::SetWandererPoolSize(int size) { m_wandererPoolSize = size; }
void LabAttackFactory::SetPlayerBulletPoolSize(int size) { m_playerBulletPoolSize = size; }
void LabAttackFactory::SetEnemyBulletPoolSize(int size) { m_enemyBulletPoolSize = size; }

// Spawn functions for enemies (using only a position)
Object* LabAttackFactory::SpawnCreep(const glm::vec3& position)
{
    // Empty: return an object from the creep pool (or create one if needed)
    return nullptr;
}

Object* LabAttackFactory::SpawnAvoider(const glm::vec3& position)
{
    return nullptr;
}

Object* LabAttackFactory::SpawnShooter(const glm::vec3& position)
{
    return nullptr;
}

Object* LabAttackFactory::SpawnWanderer(const glm::vec3& position)
{
    return nullptr;
}

// Spawn functions for bullets (position and speed)
Object* LabAttackFactory::SpawnPlayerBullet(const glm::vec3& position, const glm::vec3& speed)
{
    Object* bullet = scene->GenerateMeshObjectsFromObject("model", position, 1.f, glm::vec3(0.f), true, glm::vec4(0.7f, 0.6f, 0.f, 1.f), true, scene->sceneObjects);
    aProjectileMovement* projectileAction = new aProjectileMovement();
    projectileAction->speed = speed;



    scene->AddActionToObj(projectileAction, bullet);

    return bullet;
}

Object* LabAttackFactory::SpawnEnemyBullet(const glm::vec3& position, const glm::vec3& speed)
{
    return nullptr;
}

// Return functions for recycling objects into the pool.
void LabAttackFactory::ReturnCreep(Creep* creep) { /* Empty */ }
void LabAttackFactory::ReturnAvoider(Avoider* avoider) { /* Empty */ }
void LabAttackFactory::ReturnShooter(Shooter* shooter) { /* Empty */ }
void LabAttackFactory::ReturnWanderer(Wanderer* wanderer) { /* Empty */ }
void LabAttackFactory::ReturnPlayerBullet(PlayerBullet* bullet) { /* Empty */ }
void LabAttackFactory::ReturnEnemyBullet(EnemyBullet* bullet) { /* Empty */ }

// Private helper functions to create new objects (if pool is empty)
Creep* LabAttackFactory::CreateCreep() { return nullptr; }
Avoider* LabAttackFactory::CreateAvoider() { return nullptr; }
Shooter* LabAttackFactory::CreateShooter() { return nullptr; }
Wanderer* LabAttackFactory::CreateWanderer() { return nullptr; }
PlayerBullet* LabAttackFactory::CreatePlayerBullet() { return nullptr; }
EnemyBullet* LabAttackFactory::CreateEnemyBullet() { return nullptr; }

// Template helper function to expand any pool by a given additional count.
// (Template functions are typically defined in headers, but here we provide a stub.)
template<typename T>
void LabAttackFactory::ExpandPool(std::vector<T*>& pool, int additionalCount)
{
    // Empty implementation.
}

// Explicit template instantiations for the known object types
template void LabAttackFactory::ExpandPool<Creep>(std::vector<Creep*>& pool, int additionalCount);
template void LabAttackFactory::ExpandPool<Avoider>(std::vector<Avoider*>& pool, int additionalCount);
template void LabAttackFactory::ExpandPool<Shooter>(std::vector<Shooter*>& pool, int additionalCount);
template void LabAttackFactory::ExpandPool<Wanderer>(std::vector<Wanderer*>& pool, int additionalCount);
template void LabAttackFactory::ExpandPool<PlayerBullet>(std::vector<PlayerBullet*>& pool, int additionalCount);
template void LabAttackFactory::ExpandPool<EnemyBullet>(std::vector<EnemyBullet*>& pool, int additionalCount);
