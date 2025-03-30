#pragma once;
#include "LabAttackFactory.h"

#include "aRotate.h"
#include "BruteEnemy.h"

// Constructor
LabAttackFactory::LabAttackFactory(int creepPoolSize, int avoiderPoolSize, int shooterPoolSize, int wandererPoolSize,
    int playerBulletPoolSize, int enemyBulletPoolSize)
    : m_creepPoolSize(creepPoolSize), m_avoiderPoolSize(avoiderPoolSize), m_shooterPoolSize(shooterPoolSize),
    m_wandererPoolSize(wandererPoolSize), m_playerBulletPoolSize(playerBulletPoolSize), m_enemyBulletPoolSize(enemyBulletPoolSize)
{
}



//// Spawn functions for enemies (using only a position)
//Object* LabAttackFactory::SpawnCreep(const glm::vec3 position)
//{
//
//    Object* enemy = scene->GenerateMeshObjectsFromObject("assets/models/objects/slime.ply", position, 10.f, glm::vec3(0.f), true, glm::vec4(1.f, 0.6f, 0.f, 0.f), true, scene->sceneObjects);
//    enemy->mesh->bOverrideObjectColour = false;
//    enemy->mesh->textures[0] = "main_camera";
//    enemy->mesh->textureSpeed.x = 10.f;
//    aCreepEnemy* creep = new aCreepEnemy();
//    creep->player = player;
//    creep->factory = this;
//
//    scene->AddActionToObj(creep, enemy);
//    m_creepPool.push_back(enemy);
//    return enemy;
//}
//
//Object* LabAttackFactory::SpawnAvoider(const glm::vec3& position)
//{
//    Object* enemy = scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply", position, 7.f, glm::vec3(0.f), true, glm::vec4(1.f, 0.1f, 0.f, 0.f), true, scene->sceneObjects);
//
//
//    aAvoiderEnemy* avoider = new aAvoiderEnemy();
//    avoider->player = player;
//    avoider->factory = this;
//
//    scene->AddActionToObj(avoider, enemy);
//    m_avoiderPool.push_back(enemy);
//    return enemy;
//}
//
//Object* LabAttackFactory::SpawnShooter(const glm::vec3& position)
//{
//
//    Object* enemy = scene->GenerateMeshObjectsFromObject("assets/models/objects/wall01.ply", position, 5.f, glm::vec3(0.f), true, glm::vec4(0.5f, 0.6f, 0.5f, 0.f), true, scene->sceneObjects);
//   
//    
//    aShooterEnemy* shooter = new aShooterEnemy();
//    shooter->player = player;
//    shooter->factory = this;
//
//    aRotate* rotate = new aRotate();
//
//    scene->AddActionToObj(rotate, enemy);
//
//    scene->AddActionToObj(shooter, enemy);
//    m_shooterPool.push_back(enemy);
//    return enemy;
//}
//
//Object* LabAttackFactory::SpawnWanderer(const glm::vec3& position)
//{
//    Object* enemy = scene->GenerateMeshObjectsFromObject("assets/models/objects/tube.ply", position, 9.f, glm::vec3(0.f), true, glm::vec4(0.f, 0.1f, 1.f, 0.f), true, scene->sceneObjects);
//
//
//    aWandererEnemy* wanderer = new aWandererEnemy();
//    wanderer->player = player;
//    wanderer->factory = this;
//
//    scene->AddActionToObj(wanderer, enemy);
//    m_wandererPool.push_back(enemy);
//    return enemy;
//}

// Spawn functions for bullets (position and speed)

Object* LabAttackFactory::SpawnBrut(const glm::vec3& position)
{
    
            Object* enemy = scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply", position, 1.2f, glm::vec3(0.f), true, glm::vec4(0.f, 0.1f, 1.f, 0.f), true, scene->sceneObjects);
        
            enemy->isTemporary = true;
            BruteEnemy* brut = new BruteEnemy();
            brut->maze = maze;
            brut->factory = this;
        
            scene->AddActionToObj(brut, enemy);
            brut->Start();
          //  m_wandererPool.push_back(enemy);
            return enemy;

}
Object* LabAttackFactory::SpawnPlayerBullet(const glm::vec3& position, const glm::vec3& speed)
{
    Object* bullet = scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply", position, 0.6f, glm::vec3(0.f), true, glm::vec4(0.1f, 0.6f, 0.f, 1.f), true, scene->sceneObjects);
    aProjectileMovement* projectileAction = new aProjectileMovement();
    projectileAction->speed = speed;

    bullet->isTemporary = true;

    scene->AddActionToObj(projectileAction, bullet);
    m_playerBulletPool.push_back(bullet);
    return bullet;
}

Object* LabAttackFactory::SpawnEnemyBullet(const glm::vec3& position, const glm::vec3& speed)
{
    Object* bullet = scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply", position, 0.5f, glm::vec3(0.f), true, glm::vec4(0.9f, 0.1f, 0.1f, 1.f), true, scene->sceneObjects);
    aProjectileMovement* projectileAction = new aProjectileMovement();
    projectileAction->speed = speed;

    bullet->isTemporary = true;

    scene->AddActionToObj(projectileAction, bullet);

    return bullet;
}



