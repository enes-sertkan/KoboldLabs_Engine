#include "KLFileManager.hpp"


void CreateModelFiles(KLFileManager* fileManager)
{


    // Create an sModelDrawInfo object for testing
    sModelDrawInfo modelInfo;


    modelInfo.modelName = "Cube";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    // Call WriteModelFile to save the model info
    fileManager->WriteModelFile(&modelInfo, "Cube.txt", "XYZNUV");

    // Sky Sphere
    modelInfo.modelName = "SkySphere";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "skysphere.txt", "XYZNUV");

    modelInfo.modelName = "wall";
    modelInfo.meshPath = "assets/models/objects/wall01.ply";
    fileManager->WriteModelFile(&modelInfo, "wall.txt", "XYZNUVRGBA");

    modelInfo.modelName = "castleWall";
    modelInfo.meshPath = "assets/models/objects/castleWall1.ply";
    fileManager->WriteModelFile(&modelInfo, "cWall.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Floor";
    modelInfo.meshPath = "assets/models/objects/floor.ply";
    fileManager->WriteModelFile(&modelInfo, "floor.txt", "XYZNUVRGBA");

    modelInfo.modelName = "SideWall";
    modelInfo.meshPath = "assets/models/objects/wall01side.ply";
    fileManager->WriteModelFile(&modelInfo, "sideWall.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Comp";
    modelInfo.meshPath = "assets/models/objects/comp1.ply";
    fileManager->WriteModelFile(&modelInfo, "comp.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Screen";
    modelInfo.meshPath = "assets/models/objects/screen.ply";
    fileManager->WriteModelFile(&modelInfo, "screen.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Tube";
    modelInfo.meshPath = "assets/models/objects/tube.ply";
    fileManager->WriteModelFile(&modelInfo, "tube.txt", "XYZNUVRGBA");

    modelInfo.modelName = "TubeGlass";
    modelInfo.meshPath = "assets/models/objects/tubeGlass.ply";
    fileManager->WriteModelFile(&modelInfo, "tubeGlass.txt", "XYZNUVRGBA");

    modelInfo.modelName = "TubeBroken";
    modelInfo.meshPath = "assets/models/objects/tubeBroken.ply";
    fileManager->WriteModelFile(&modelInfo, "tubeBroken.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Shards";
    modelInfo.meshPath = "assets/models/objects/shards.ply";
    fileManager->WriteModelFile(&modelInfo, "shards.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Reactor";
    modelInfo.meshPath = "assets/models/objects/reactor.ply";
    fileManager->WriteModelFile(&modelInfo, "reactor.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Table";
    modelInfo.meshPath = "assets/models/objects/table.ply";
    fileManager->WriteModelFile(&modelInfo, "table.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Light";
    modelInfo.meshPath = "assets/models/objects/light.ply";
    fileManager->WriteModelFile(&modelInfo, "light.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Frog";
    modelInfo.meshPath = "assets/models/objects/frog.ply";
    fileManager->WriteModelFile(&modelInfo, "frog.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Slime";
    modelInfo.meshPath = "assets/models/objects/slime.ply";
    fileManager->WriteModelFile(&modelInfo, "slime.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Sphere";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "sphere.txt", "XYZNUVRGBA");

    //Props
    //small
    modelInfo.modelName = "Plants";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_Plant_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Plants.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Chair";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_SwivelChair_04_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Chair.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Bed";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_Bed_02_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Bed.txt", "XYZNUVRGBA");

    //medium
    modelInfo.modelName = "console";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_Console_05_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "console.txt", "XYZNUVRGBA");

    modelInfo.modelName = "SLadder";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_StepLadder_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "SLadder.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Cdesk";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_ControlDesk_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Cdesk.txt", "XYZNUVRGBA");

    //large
    modelInfo.modelName = "printer";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_3DPrinter_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "printer.txt", "XYZNUVRGBA");

    modelInfo.modelName = "treadmill";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_Treadmill_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "treadmill.txt", "XYZNUVRGBA");

    modelInfo.modelName = "stairs";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_Stairs_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "stairs.txt", "XYZNUVRGBA");


    //SelectBox
    modelInfo.modelName = "Select_Box";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "selectBox.txt", "XYZNUV");

    //Screen Quaad
    modelInfo.modelName = "Screen_quad";
    modelInfo.meshPath = "assets/models/screen_quad.ply";
    fileManager->WriteModelFile(&modelInfo, "screen_quad.txt", "XYZNUVRGBA");

    //Ico Sphere
    modelInfo.modelName = "ISphere";
    modelInfo.meshPath = "assets/models/IcoSphere.ply";
    fileManager->WriteModelFile(&modelInfo, "ISphere.txt", "XYZNUVRGBA");

    //Ico Sphere
    modelInfo.modelName = "Rope";
    modelInfo.meshPath = "assets/models/Rope.ply";
    fileManager->WriteModelFile(&modelInfo, "Rope.txt", "XYZNUVRGBA");
    

    modelInfo.modelName = "Plane_1x1";
    modelInfo.meshPath = "assets/models/plene_1x1.ply";
    fileManager->WriteModelFile(&modelInfo, "Plane_1x1.txt", "XYZNUVRGBA");


    modelInfo.modelName = "StandartTurretHead";
    modelInfo.meshPath = "assets/models/Turret/StandartTurretHead.ply";
    fileManager->WriteModelFile(&modelInfo, "StandartTurretHead.txt", "XYZNUVRGBA"); 
    
    modelInfo.modelName = "StandartTurretNeck";
    modelInfo.meshPath = "assets/models/Turret/StandartTurretNeck.ply";
    fileManager->WriteModelFile(&modelInfo, "StandartTurretNeck.txt", "XYZNUVRGBA");

    modelInfo.modelName = "StandartTurretBody";
    modelInfo.meshPath = "assets/models/Turret/StandartTurretBody.ply";
    fileManager->WriteModelFile(&modelInfo, "StandartTurretBody.txt", "XYZNUVRGBA");

    modelInfo.modelName = "TripleTurretBody";
    modelInfo.meshPath = "assets/models/Turret2/Turret2_body.ply";
    fileManager->WriteModelFile(&modelInfo, "TripleTurretBody.txt", "XYZNUVRGBA");
    modelInfo.modelName = "TripleTurretNeck";
    modelInfo.meshPath = "assets/models/Turret2/Turret2_neck.ply";
    fileManager->WriteModelFile(&modelInfo, "TripleTurretNeck.txt", "XYZNUVRGBA");
    modelInfo.modelName = "TripleTurretHead";
    modelInfo.meshPath = "assets/models/Turret2/Turret2_head.ply";
    fileManager->WriteModelFile(&modelInfo, "TripleTurretHead.txt", "XYZNUVRGBA");

    modelInfo.modelName = "ProjectileTurretBody";
    modelInfo.meshPath = "assets/models/Turret3/Turret3_body.ply";
    fileManager->WriteModelFile(&modelInfo, "ProjectileTurretBody.txt", "XYZNUVRGBA");
    modelInfo.modelName = "ProjectileTurretNeck";
    modelInfo.meshPath = "assets/models/Turret3/Turret3_neck.ply";
    fileManager->WriteModelFile(&modelInfo, "ProjectileTurretNeck.txt", "XYZNUVRGBA");
    modelInfo.modelName = "ProjectileTurretHead";
    modelInfo.meshPath = "assets/models/Turret3/Turret3_head.ply";
    fileManager->WriteModelFile(&modelInfo, "ProjectileTurretHead.txt", "XYZNUVRGBA");
    
    modelInfo.modelName = "EnemyWheeelBody";
    modelInfo.meshPath = "assets/models/Enemy_Wheel/Enenemy_wheel_body.ply";
    fileManager->WriteModelFile(&modelInfo, "EnemyWheeelBody.txt", "XYZNUVRGBA");
    modelInfo.modelName = "EnemyWheeelWheel";
    modelInfo.meshPath = "assets/models/Enemy_Wheel/Enenemy_wheel_wheel.ply";
    fileManager->WriteModelFile(&modelInfo, "EnemyWheeelWheel.txt", "XYZNUVRGBA");
    modelInfo.modelName = "EnemyWheeelHead";
    modelInfo.meshPath = "assets/models/Enemy_Wheel/Enenemy_wheel_head.ply";
    fileManager->WriteModelFile(&modelInfo, "EnemyWheeelBody.txt", "XYZNUVRGBA");
    modelInfo.meshPath = "assets/models/Enemy_Wheel/Enenemy_wheel_body.ply";
    fileManager->WriteModelFile(&modelInfo, "EnemyWheeelBody.txt", "XYZNUVRGBA");

    modelInfo.modelName = "ToolDestroyer";
    modelInfo.meshPath = "assets/models/Tools/tool_destroyer.ply";
    fileManager->WriteModelFile(&modelInfo, "ToolDestroyer.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Bullet";
    modelInfo.meshPath = "assets/models/bullet.ply";
    fileManager->WriteModelFile(&modelInfo, "Bullet.txt", "XYZNUVRGBA");

    modelInfo.modelName = "EnemyShooterBody";
    modelInfo.meshPath = "assets/models/Enemy_Shooter/Enemy_shooter_body.ply";
    fileManager->WriteModelFile(&modelInfo, "EnemyShooterBody.txt", "XYZNUVRGBA");
    modelInfo.modelName = "EnemyShooterHead";
    modelInfo.meshPath = "assets/models/Enemy_Shooter/Enemy_shooter_head.ply";
    fileManager->WriteModelFile(&modelInfo, "EnemyShooterHead.txt", "XYZNUVRGBA");
    modelInfo.modelName = "EnemyShooterWheel";
    modelInfo.meshPath = "assets/models/Enemy_Shooter/Enemy_shooter_wheel.ply";
    fileManager->WriteModelFile(&modelInfo, "EnemyShooterWheel.txt", "XYZNUVRGBA");


    modelInfo.modelName = "BigTowersCastle_e_10";
    modelInfo.meshPath = "assets/models/CastleModels/BigTowersCastle_e_10.ply";
    fileManager->WriteModelFile(&modelInfo, "BigTowersCastle_e_10.txt", "XYZNUVRGBA");
    modelInfo.modelName = "MainCastle_e_9";
    modelInfo.meshPath = "assets/models/CastleModels/MainCastle_e_9.ply";
    fileManager->WriteModelFile(&modelInfo, "MainCastle_e_9.txt", "XYZNUVRGBA");
    modelInfo.modelName = "SmallTowersCastle_e_11";
    modelInfo.meshPath = "assets/models/CastleModels/SmallTowersCastle_e_11.ply";
    fileManager->WriteModelFile(&modelInfo, "SmallTowersCastle_e_11.txt", "XYZNUVRGBA");
    modelInfo.modelName = "TowersCastle_e_06";
    modelInfo.meshPath = "assets/models/CastleModels/TowersCastle_e_06.ply";
    fileManager->WriteModelFile(&modelInfo, "TowersCastle_e_06.txt", "XYZNUVRGBA");
    modelInfo.modelName = "WallsCastle_e_05";
    modelInfo.meshPath = "assets/models/CastleModels/WallsCastle_e_05.ply";
    fileManager->WriteModelFile(&modelInfo, "WallsCastle_e_05.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Tree1";
    modelInfo.meshPath = "assets/models/Trees/tree1.ply";
    fileManager->WriteModelFile(&modelInfo, "Tree1.txt", "XYZNUVRGBA");
    modelInfo.modelName = "Tree2";
    modelInfo.meshPath = "assets/models/Trees/tree2.ply";
    fileManager->WriteModelFile(&modelInfo, "Tree2.txt", "XYZNUVRGBA");

}