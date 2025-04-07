#pragma once
#ifndef LAB_ATTACK_FACTORY_HPPm
#define LAB_ATTACK_FACTORY_HPP

#include <glm/vec3.hpp>
#include <vector>
#include "Scene.hpp"
#include "sObject.h"
#include "aProjectileMovement.hpp"
#include "MazeGenerator.hpp"
#include "aGrassCollider .h"
#include "TurretConfig.h"


class Turret;
class BruteEnemy;

class LabAttackFactory {
public:
    Object* player = nullptr;
    MazeGenerator* maze = nullptr;
    // Constructor with default initial pool sizes
    LabAttackFactory(int creepPoolSize = 10, int avoiderPoolSize = 10, int shooterPoolSize = 10, int wandererPoolSize = 10,
        int playerBulletPoolSize = 20, int enemyBulletPoolSize = 20);
    ~LabAttackFactory();

    Object* grass = nullptr;


    Scene* scene = nullptr;

    // Spawn functions:
    // For enemies, only a position is needed.
    Object* SpawnCreep(const glm::vec3 position);
    Object* SpawnAvoider(const glm::vec3& position);
    Object* SpawnShooter(const glm::vec3& position);
    Object* SpawnWanderer(const glm::vec3& position);
    Object* SpawnBrut(glm::vec3 position);

    void Start();

    cTurretHead* SpawnTurretHead(const glm::vec3& position, eTurretHeadID headID);
    cTurretNeck* SpawnTurretNeck(const glm::vec3& position, eTurretNeckID headID);
    cTurretBody* SpawnTurretBody(const glm::vec3& position, eTurretBodyID headID);

    Turret* SpawnTurret(const glm::vec3& position, eTurretBodyID bodyID, eTurretNeckID neckID, eTurretHeadID headID);
    Turret* SpawnTurretGhost(const glm::vec3& position, eTurretBodyID bodyID, eTurretNeckID neckID, eTurretHeadID headID);

    // For bullets, a position and a speed vector are provided.
    Object* SpawnPlayerBullet(const glm::vec3& position, const glm::vec3& speed);
    Object* SpawnEnemyBullet(const glm::vec3& position, const glm::vec3& speed);
    // Spawn turret based on configuration



   
    std::vector<cTurretHead*> turretHeads;
    std::vector<cTurretNeck*> turretNecks;
    std::vector<cTurretBody*> turretBodies;

    cTurretBody* FindTurretBodyTemplate(eTurretBodyID id);
    cTurretNeck* FindTurretNeckTemplate(eTurretNeckID id);
    cTurretHead* FindTurretHeadTemplate(eTurretHeadID id);


    std::vector<Turret*>         turrets;

    std::vector<BruteEnemy*>         m_creepPool;

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