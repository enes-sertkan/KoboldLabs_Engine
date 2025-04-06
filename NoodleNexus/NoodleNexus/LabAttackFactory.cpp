#pragma once;
#include "LabAttackFactory.h"

#include "aRotate.h"
#include "BruteEnemy.h"
#include "aBullet.h"
#include "aTurretBody.h"
#include "aTurretNeckRotate.hpp"
#include "TurretNeckAim.h"
#include "aTurretHead.hpp"
#include "cTurret.h"
#include "aEnemyBullet.h"
#include "aPlayerBullet.h"
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
        
            
          
            
            BruteEnemy* brut = new BruteEnemy();
            brut->maze = maze;
            brut->factory = this;
        
            scene->AddActionToObj(brut, enemy);

            brut->Start();
            
            if (grass != nullptr)
            {
                aGrassCollider* grassCollider = new aGrassCollider();
                grassCollider->SetGrass(grass);
                scene->AddActionToObj(grassCollider, enemy);
            }
            
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

Turret* LabAttackFactory::SpawnTurretGhost(const glm::vec3& position, eTurretBodyID bodyID, eTurretNeckID neckID, eTurretHeadID headID)
{
    Turret* newTurret = new Turret();
    newTurret->factory = this;
    newTurret->position = position;

    sTurretCofig config;
    config.bodyID = bodyID;
    config.neckID = neckID;
    config.headID = headID;

    newTurret->RebuildTurretGhost(&config);
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
    newObj->mesh->textures[0] = templateBody->object->mesh->textures[0];
    newObj->mesh->blendRatio[0] = templateBody->object->mesh->blendRatio[0];
    newObj->mesh->NMTexture = templateBody->object->mesh->NMTexture;
    newObj->mesh->AOtexture = templateBody->object->mesh->AOtexture;
    newObj->mesh->STTexture = templateBody->object->mesh->STTexture;
    cTurretBody* newBody = new cTurretBody();
    newBody->object = newObj;
    newBody->action = templateBody->action->Clone(); // Implement Clone() for actions
    newBody->ID = bodyID;
    newBody->connectionTransform = templateBody->connectionTransform;

    if (grass != nullptr)
    {
        aGrassCollider* grassCollider = new aGrassCollider();
        grassCollider->SetGrass(grass);
        grassCollider->colliderRadius = 1.2f;
        grassCollider->colliderBlendRadius = 2.f;
        scene->AddActionToObj(grassCollider, newBody->object);
    }


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

    newObj->mesh->textures[0] = templateHead->object->mesh->textures[0];
    newObj->mesh->blendRatio[0] = templateHead->object->mesh->blendRatio[0];
    newObj->mesh->NMTexture = templateHead->object->mesh->NMTexture;
    newObj->mesh->AOtexture = templateHead->object->mesh->AOtexture;
    newObj->mesh->STTexture = templateHead->object->mesh->STTexture;

    cTurretHead* newHead = new cTurretHead();
    newHead->object = newObj;
    newHead->action = templateHead->action->Clone();
    newHead->ID = headID;
    newHead->connectionTransform = templateHead->connectionTransform;
    newHead->barrelsPos = templateHead->barrelsPos;

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


    newObj->mesh->textures[0] = templateNeck->object->mesh->textures[0];
    newObj->mesh->blendRatio[0] = templateNeck->object->mesh->blendRatio[0];
    newObj->mesh->NMTexture = templateNeck->object->mesh->NMTexture;
    newObj->mesh->AOtexture = templateNeck->object->mesh->AOtexture;
    newObj->mesh->STTexture = templateNeck->object->mesh->STTexture;

    // Create head connection object
    Object* connectionObj = scene->GenerateMeshObjectsFromObject(
        "",
        position,
        templateNeck->object->mesh->uniformScale,
        templateNeck->object->mesh->rotationEulerXYZ,
        templateNeck->object->mesh->bOverrideObjectColour,
        templateNeck->object->mesh->objectColourRGBA,
        !templateNeck->object->mesh->bDoNotLight,
        scene->sceneObjects
    );

    connectionObj->name = "HeadConnection";
    newObj->AddChild(connectionObj);
    connectionObj->mesh->positionXYZ = templateNeck->connectionTransform;
    connectionObj->startTranform->position = templateNeck->connectionTransform;


    cTurretNeck* newNeck = new cTurretNeck();
    newNeck->object = newObj;
    newNeck->action = templateNeck->action->Clone();
    newNeck->action->headConnection = connectionObj;
    newNeck->ID = neckID;
    newNeck->connectionTransform = templateNeck->connectionTransform;
    newNeck->headConnection = connectionObj;
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
        "assets/models/Turret/StandartTurretBody.ply",
        glm::vec3(0), 0.07f, glm::vec3(0.f),
        false, glm::vec4(0.1f, 0.6f, 0.f, 1.f),
        true, scene->sceneObjects
    );

    bodyObj->mesh->textures[0] = "Turret/Turret_Albedo.bmp";
    bodyObj->mesh->blendRatio[0] = 1;
    bodyObj->mesh->NMTexture = "Turret/Turret_Normal.bmp";
    bodyObj->mesh->AOtexture = "Turret/Turret_Occlusion.bmp";
    bodyObj->mesh->STTexture = "Turret/Turret_ST.bmp";
    standardBody->ID = STANDARTBODY;
    standardBody->object = bodyObj;
    standardBody->action = new aTurretBody();
    standardBody->connectionTransform = glm::vec3(0, 0.3f, 0); // Example offset
    turretBodies.push_back(standardBody);
    bodyObj->isActive = false;


    // Standard Body
    cTurretBody* tripleBody = new cTurretBody();
    Object* tribodyObj = scene->GenerateMeshObjectsFromObject(
        "assets/models/Turret2/Turret2_body.ply",
        glm::vec3(0), 0.07f, glm::vec3(0.f),
        false, glm::vec4(0.1f, 0.6f, 0.f, 1.f),
        true, scene->sceneObjects
    );

    tribodyObj->mesh->textures[0] = "Turret2/Turret2_albedo.bmp";
    tribodyObj->mesh->blendRatio[0] = 1;
    tribodyObj->mesh->NMTexture = "Turret2/Turret2_normal.bmp";
    tribodyObj->mesh->AOtexture = "Turret2/Turret2_AO.bmp";
    tribodyObj->mesh->STTexture = "Turret2/Turret2_MS.bmp";

    tripleBody->ID = TRIPLEBODY;
    tripleBody->object = tribodyObj;
    tripleBody->action = new aTurretBody();
    tripleBody->connectionTransform = glm::vec3(0, 0.3f, 0); // Example offset
    turretBodies.push_back(tripleBody);
    tribodyObj->isActive = false;





    cTurretNeck* aimNeck = new cTurretNeck();
    Object* aimNeckObj = scene->GenerateMeshObjectsFromObject(
        "assets/models/Turret2/Turret2_neck.ply",
        glm::vec3(0),
        0.07f,
        glm::vec3(0.f),
        false,
        glm::vec4(0.8f, 0.8f, 0.8f, 1.f),
        true,
        scene->sceneObjects
    );

    aimNeckObj->mesh->textures[0] = "Turret2/Turret2_albedo.bmp";
    aimNeckObj->mesh->blendRatio[0] = 1;
    aimNeckObj->mesh->NMTexture = "Turret2/Turret2_normal.bmp";
    aimNeckObj->mesh->AOtexture = "Turret2/Turret2_AO.bmp";
    aimNeckObj->mesh->STTexture = "Turret2/Turret2_MS.bmp";

    aimNeck->ID = AIMNECK;
    aimNeck->object = aimNeckObj;
    aimNeck->action = new TurretNeckAim();
    aimNeck->action->factory = this;
    aimNeck->connectionTransform = glm::vec3(0, 1.275f, 0); // Example offset
    turretNecks.push_back(aimNeck);
    aimNeckObj->isActive = false;


    // Aim Neck
    cTurretNeck* standardNeck = new cTurretNeck();
    Object* neckObj = scene->GenerateMeshObjectsFromObject(
        "assets/models/Turret/StandartTurretNeck.ply",
        glm::vec3(0),
        0.07f,
        glm::vec3(0.f),
        false,
        glm::vec4(0.8f, 0.8f, 0.8f, 1.f),
        true,
        scene->sceneObjects
    );

    neckObj->mesh->textures[0] = "Turret/Turret_Albedo.bmp";
    neckObj->mesh->blendRatio[0] = 1;
    neckObj->mesh->NMTexture = "Turret/Turret_Normal.bmp";
    neckObj->mesh->AOtexture = "Turret/Turret_Occlusion.bmp";
    neckObj->mesh->STTexture = "Turret/Turret_ST.bmp";


    standardNeck->ID = STANDARTNECK;
    standardNeck->object = neckObj;
    standardNeck->action = new aTurretNeckRotate();
    standardNeck->connectionTransform = glm::vec3(0, 1.265f, 0); // Example offset
    turretNecks.push_back(standardNeck);
    neckObj->isActive = false;

    // Standard Head
    cTurretHead* standardHead = new cTurretHead();
    Object* headObj = scene->GenerateMeshObjectsFromObject(
        "assets/models/Turret/StandartTurretHead.ply",
        glm::vec3(0),
        0.07f,
        glm::vec3(0.f),
        false,
        glm::vec4(1.f, 0.f, 0.f, 1.f),
        true,
        scene->sceneObjects
    );


    headObj->mesh->textures[0] = "Turret/Turret_Albedo.bmp";
    headObj->mesh->blendRatio[0] = 1;
    headObj->mesh->NMTexture = "Turret/Turret_Normal.bmp";
    headObj->mesh->AOtexture = "Turret/Turret_Occlusion.bmp";
    headObj->mesh->STTexture = "Turret/Turret_ST.bmp";
    standardHead->barrelsPos.push_back(glm::vec3(0.f, 0.f, 0.7f));


    standardHead->ID = STANDARTHEAD;
    standardHead->object = headObj;
    aTurretHead* standartHeadAction = new aTurretHead();
    standartHeadAction->m_recoilDistance = 0.6f;
    standartHeadAction->m_recoilRecoverySpeed = 1.f;

    standartHeadAction->factory = this;
    standardHead->action = standartHeadAction;
     
    standardHead->connectionTransform = glm::vec3(0, 0.3f, 0);
    turretHeads.push_back(standardHead);

    headObj->isActive = false;


    //Triple Head
    cTurretHead* tripleHead = new cTurretHead();
    Object* triheadObj = scene->GenerateMeshObjectsFromObject(
        "assets/models/Turret2/Turret2_head.ply",
        glm::vec3(0),
        0.07f,
        glm::vec3(0.f),
        false,
        glm::vec4(1.f, 0.f, 0.f, 1.f),
        true,
        scene->sceneObjects
    );


    triheadObj->mesh->textures[0] = "Turret2/Turret2_albedo.bmp";
    triheadObj->mesh->blendRatio[0] = 1;
    triheadObj->mesh->NMTexture = "Turret2/Turret2_normal.bmp";
    triheadObj->mesh->AOtexture = "Turret2/Turret2_AO.bmp"; 
    triheadObj->mesh->STTexture = "Turret2/Turret2_MS.bmp";
    tripleHead->barrelsPos.push_back(glm::vec3(0.f, 0.2f, 0.7f));
    tripleHead->barrelsPos.push_back(glm::vec3(0.2f, 0.f, 0.7f));
    tripleHead->barrelsPos.push_back(glm::vec3(-0.2f, 0.f, 0.7f));


    tripleHead->ID = TRIPLEHEAD;
    tripleHead->object = triheadObj;
    aTurretHead* triHeadAction = new aTurretHead();
    triHeadAction->factory = this;
    triHeadAction->m_shootCooldown = 0.25f;
    tripleHead->action = triHeadAction;

    tripleHead->connectionTransform = glm::vec3(0, 0.3f, 0);
    turretHeads.push_back(tripleHead);

    triheadObj->isActive = false;
}




Object* LabAttackFactory::SpawnPlayerBullet(const glm::vec3& position, const glm::vec3& speed)
{
    Object* bullet = scene->GenerateMeshObjectsFromObject("assets/models/bullet.ply", position, 0.5f, glm::vec3(0.f), false, glm::vec4(0.5f, 0.4f, 0.4f, 1.f), true, scene->sceneObjects);
    bullet->mesh->metal = 0.8;
    bullet->mesh->smoothness = 0.7f;
    bullet->name = "PBullet";
    bullet->mesh->textures[0] = "Bullet/Bullet_colour.bmp";
    bullet->mesh->AOtexture = "Bullet/Bullet_AO.bmp";
    bullet->mesh->STTexture = "Bullet/Bullet_met_smothness.bmp";
    bullet->mesh->NMTexture = "Bullet/Bullet_normals.bmp";

    aProjectileMovement* projectileAction = new aProjectileMovement();
    projectileAction->speed = speed;

    bullet->isTemporary = true;

    scene->AddActionToObj(projectileAction, bullet);
    projectileAction->Start();
    m_playerBulletPool.push_back(bullet);

    aPlayerBullet* bulletCol = new aPlayerBullet();
    bulletCol->factory = this;
    scene->AddActionToObj(bulletCol, bullet);


    return bullet;
}

Object* LabAttackFactory::SpawnEnemyBullet(const glm::vec3& position, const glm::vec3& speed)
{
    Object* bullet = scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply", position, 0.5f, glm::vec3(0.f), true, glm::vec4(0.9f, 0.1f, 0.1f, 1.f), true, scene->sceneObjects);
   
    bullet->name = "EBullet";
    aProjectileMovement* projectileAction = new aProjectileMovement();
    projectileAction->speed = speed;

    bullet->isTemporary = true;

    scene->AddActionToObj(projectileAction, bullet);
    projectileAction->Start();

    aEnemyBullet* bulletCol = new aEnemyBullet();
    bulletCol->factory = this;
    scene->AddActionToObj(bulletCol, bullet);

    return bullet;
}



