#pragma once;
#include "LabAttackFactory.h"

#include "aRotate.h"
#include "BruteEnemy.h"
#include "aBullet.h"
#include "aTurretBody.h"
#include "aTurretNeckRotate.hpp"
#include "aTurretHead.hpp"
#include "cTurret.h"
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
    
            Object* enemy = scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply", position, 0.4f, glm::vec3(0.f), true, glm::vec4(0.f, 0.1f, 1.f, 0.f), true, scene->sceneObjects);
        
            enemy->isTemporary = true;
        
            
            if (grass != nullptr)
            {
                aGrassCollider* grassCollider = new aGrassCollider();
                grassCollider->SetGrass(grass);
                scene->AddActionToObj(grassCollider, enemy);
            }
            
            BruteEnemy* brut = new BruteEnemy();
            brut->maze = maze;
            brut->factory = this;
        
            scene->AddActionToObj(brut, enemy);
            brut->Start();
            m_creepPool.push_back(brut);

   

            return enemy;

}


Turret* LabAttackFactory::SpawnTurret(const glm::vec3& position, eTurretBodyID bodyID, eTurretNeckID neckID, eTurretHeadID headID)
{
    Turret* newTurret = new Turret();
    newTurret->factory = this;
    newTurret->position = position;

    sTurretCofig config;
    config.bodyID = bodyID;
    config.neckID = neckID;
    config.headID = headID;

    newTurret->RebuildTurret(&config);
    return newTurret;
}


cTurretBody* LabAttackFactory::SpawnTurretBody(const glm::vec3& position, eTurretBodyID bodyID)
{
    cTurretBody* templateBody = FindTurretBodyTemplate(bodyID);
    if (!templateBody) {
        // Fallback to standard
        templateBody = FindTurretBodyTemplate(STANDARTBODY);
        if (!templateBody) return nullptr;
    }

    // Create new instance
    Object* newObj = scene->GenerateMeshObjectsFromObject(
        templateBody->object->mesh->modelFileName,
        position,
        templateBody->object->mesh->uniformScale,
        templateBody->object->mesh->rotationEulerXYZ,
        templateBody->object->mesh->bOverrideObjectColour,
        templateBody->object->mesh->objectColourRGBA,
        !templateBody->object->mesh->bDoNotLight,
        scene->sceneObjects
    );

    newObj->name = "Body";

    cTurretBody* newBody = new cTurretBody();
    newBody->object = newObj;
    newBody->action = templateBody->action->Clone(); // Implement Clone() for actions
    newBody->ID = bodyID;
    newBody->connectionTransform = templateBody->connectionTransform;

    scene->AddActionToObj(newBody->action, newObj);
    return newBody;
}

cTurretHead* LabAttackFactory::SpawnTurretHead(const glm::vec3& position, eTurretHeadID headID)
{
    cTurretHead* templateHead = FindTurretHeadTemplate(headID);
    if (!templateHead) {
        // Fallback to standard head
        templateHead = FindTurretHeadTemplate(STANDARTHEAD);
        if (!templateHead) return nullptr;
    }

    // Create new instance
    Object* newObj = scene->GenerateMeshObjectsFromObject(
        templateHead->object->mesh->modelFileName,
        position,
        templateHead->object->mesh->uniformScale,
        templateHead->object->mesh->rotationEulerXYZ,
        templateHead->object->mesh->bOverrideObjectColour,
        templateHead->object->mesh->objectColourRGBA,
        !templateHead->object->mesh->bDoNotLight,
        scene->sceneObjects
    );

    newObj->name = "Head";

    cTurretHead* newHead = new cTurretHead();
    newHead->object = newObj;
    newHead->action = templateHead->action->Clone();
    newHead->ID = headID;
    newHead->connectionTransform = templateHead->connectionTransform;

    scene->AddActionToObj(newHead->action, newObj);
    return newHead;
}

cTurretNeck* LabAttackFactory::SpawnTurretNeck(const glm::vec3& position, eTurretNeckID neckID)
{
    cTurretNeck* templateNeck = FindTurretNeckTemplate(neckID);
    if (!templateNeck) {
        // Fallback to standard neck
        templateNeck = FindTurretNeckTemplate(STANDARTNECK);
        if (!templateNeck) return nullptr;
    }

    // Create new instance
    Object* newObj = scene->GenerateMeshObjectsFromObject(
        templateNeck->object->mesh->modelFileName,
        position,
        templateNeck->object->mesh->uniformScale,
        templateNeck->object->mesh->rotationEulerXYZ,
        templateNeck->object->mesh->bOverrideObjectColour,
        templateNeck->object->mesh->objectColourRGBA,
        !templateNeck->object->mesh->bDoNotLight,
        scene->sceneObjects
    );

    newObj->name = "Neck";

    cTurretNeck* newNeck = new cTurretNeck();
    newNeck->object = newObj;
    newNeck->action = templateNeck->action->Clone();
    newNeck->ID = neckID;
    newNeck->connectionTransform = templateNeck->connectionTransform;

    scene->AddActionToObj(newNeck->action, newObj);
    return newNeck;
}


cTurretBody* LabAttackFactory::FindTurretBodyTemplate(eTurretBodyID id)
{
    for (cTurretBody* body : turretBodies) {
        if (body->ID == id) return body;
    }
    return nullptr;
}

cTurretHead* LabAttackFactory::FindTurretHeadTemplate(eTurretHeadID id)
{
    for (cTurretHead* head : turretHeads) {
        if (head->ID == id) return head;
    }
    return nullptr;
}

cTurretNeck* LabAttackFactory::FindTurretNeckTemplate(eTurretNeckID id)
{
    for (cTurretNeck* neck : turretNecks) {
        if (neck->ID == id) return neck;
    }
    return nullptr;
}


void LabAttackFactory::Start()
{
    // Standard Body
    cTurretBody* standardBody = new cTurretBody();
    Object* bodyObj = scene->GenerateMeshObjectsFromObject(
        "assets/models/Cube_xyz_n_uv.ply",
        glm::vec3(0), 1.f, glm::vec3(0.f),
        true, glm::vec4(0.1f, 0.6f, 0.f, 1.f),
        true, scene->sceneObjects
    );
    standardBody->ID = STANDARTBODY;
    standardBody->object = bodyObj;
    standardBody->action = new aTurretBody();
    standardBody->connectionTransform = glm::vec3(0, 1, 0); // Example offset
    turretBodies.push_back(standardBody);
    bodyObj->isActive = false;


    cTurretNeck* standardNeck = new cTurretNeck();
    Object* neckObj = scene->GenerateMeshObjectsFromObject(
        "assets/models/Cube_xyz_n_uv.ply",
        glm::vec3(0),
        0.5f,
        glm::vec3(0.f),
        true,
        glm::vec4(0.8f, 0.8f, 0.8f, 1.f),
        true,
        scene->sceneObjects
    );
    standardNeck->ID = STANDARTNECK;
    standardNeck->object = neckObj;
    standardNeck->action = new aTurretNeckRotate();
    standardNeck->connectionTransform = glm::vec3(0, 0.8f, 0); // Example offset
    turretNecks.push_back(standardNeck);
    neckObj->isActive = false;
    // Standard Head
    cTurretHead* standardHead = new cTurretHead();
    Object* headObj = scene->GenerateMeshObjectsFromObject(
        "assets/models/Cube_xyz_n_uv.ply",
        glm::vec3(0),
        0.8f,
        glm::vec3(0.f),
        true,
        glm::vec4(1.f, 0.f, 0.f, 1.f),
        true,
        scene->sceneObjects
    );
    standardHead->ID = STANDARTHEAD;
    standardHead->object = headObj;
    aTurretHead* standartHeadAction = new aTurretHead();
    standartHeadAction->factory = this;
    standardHead->action = standartHeadAction;
     
    standardHead->connectionTransform = glm::vec3(0, 0.3f, 0);
    turretHeads.push_back(standardHead);

    headObj->isActive = false;

}




Object* LabAttackFactory::SpawnPlayerBullet(const glm::vec3& position, const glm::vec3& speed)
{
    Object* bullet = scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply", position, 0.2f, glm::vec3(0.f), true, glm::vec4(0.1f, 0.6f, 0.f, 1.f), true, scene->sceneObjects);

    bullet->name = "PBullet";
    aProjectileMovement* projectileAction = new aProjectileMovement();
    projectileAction->speed = speed;

    bullet->isTemporary = true;

    scene->AddActionToObj(projectileAction, bullet);
    m_playerBulletPool.push_back(bullet);

    aBullet* bulletCol = new aBullet();
    bulletCol->factory = this;
    scene->AddActionToObj(bulletCol, bullet);
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



