#include "KLFileManager.hpp"


void CreateModelFiles(KLFileManager* fileManager)
{


    // Create an sModelDrawInfo object for testing
    sModelDrawInfo modelInfo;


    modelInfo.modelName = "Cube";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    // Call WriteModelFile to save the model info
    fileManager->WriteModelFile(&modelInfo, "Cube.txt", "XYZNUV");

    // Millenium Falcon
    modelInfo.modelName = "Falcon";
    modelInfo.meshPath = "assets/models/Millennium_Falcon/Falcon_Body_281291_verts.ply";
    fileManager->WriteModelFile(&modelInfo, "Falcon.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FaclonBody";
    modelInfo.meshPath = "assets/models/Millennium_Falcon/Parts/Falcon_Body.ply";
    fileManager->WriteModelFile(&modelInfo, "FaclonBody.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FaclonGear";
    modelInfo.meshPath = "assets/models/Millennium_Falcon/Parts/Front_Gear.ply";
    fileManager->WriteModelFile(&modelInfo, "FaclonGear.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FaclonGearDoor";
    modelInfo.meshPath = "assets/models/Millennium_Falcon/Parts/Front_Gear_Door_1.ply";
    fileManager->WriteModelFile(&modelInfo, "FaclonGearDoor.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FaclonGearDoor2";
    modelInfo.meshPath = "assets/models/Millennium_Falcon/Parts/Front_Gear_Door_2.ply";
    fileManager->WriteModelFile(&modelInfo, "FaclonGearDoor2.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FaclonMainRamp";
    modelInfo.meshPath = "assets/models/Millennium_Falcon/Parts/Main_ramp.ply";
    fileManager->WriteModelFile(&modelInfo, "FaclonMainRamp.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FaclonPortRearDoor";
    modelInfo.meshPath = "assets/models/Millennium_Falcon/Parts/Port_Rear_Door_1.ply";
    fileManager->WriteModelFile(&modelInfo, "FaclonPortRearDoor.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FaclonPortRearDoor2";
    modelInfo.meshPath = "assets/models/Millennium_Falcon/Parts/Port_Rear_Door_2.ply";
    fileManager->WriteModelFile(&modelInfo, "FaclonPortRearDoor2.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FaclonPortRearGear";
    modelInfo.meshPath = "assets/models/Millennium_Falcon/Parts/Port_Rear_Gear.ply";
    fileManager->WriteModelFile(&modelInfo, "FaclonPortRearGear.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FaclonReadDoor";
    modelInfo.meshPath = "assets/models/Millennium_Falcon/Parts/Starboard_Read_Door_1.ply";
    fileManager->WriteModelFile(&modelInfo, "FaclonReadDoor.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FaclonReadDoor2";
    modelInfo.meshPath = "assets/models/Millennium_Falcon/Parts/Starboard_Read_Door_2.ply";
    fileManager->WriteModelFile(&modelInfo, "FaclonReadDoor2.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FaclonStartRearGear";
    modelInfo.meshPath = "assets/models/Millennium_Falcon/Parts/Starboard_Rear_Gear.ply";
    fileManager->WriteModelFile(&modelInfo, "FaclonStartRearGear.txt", "XYZNUVRGBA");

    // Tie Fighter
    modelInfo.modelName = "TieFighter";
    modelInfo.meshPath = "assets/models/Tie_Fighter/fighter_xyz_n_uv_rgba_130744_verts.ply";
    fileManager->WriteModelFile(&modelInfo, "TieFighter.txt", "XYZNUVRGBA");

    modelInfo.modelName = "TieFighterBody";
    modelInfo.meshPath = "assets/models/Tie_Fighter/parts/Body_Only_67389_verts.ply";
    fileManager->WriteModelFile(&modelInfo, "TieFighterBody.txt", "XYZNUVRGBA");

    modelInfo.modelName = "TieFighterPortPanel";
    modelInfo.meshPath = "assets/models/Tie_Fighter/parts/Port_Panel_only_31744_verts.ply";
    fileManager->WriteModelFile(&modelInfo, "TieFighterPortPanel.txt", "XYZNUVRGBA");

    modelInfo.modelName = "TieFighterStarBoardPanel";
    modelInfo.meshPath = "assets/models/Tie_Fighter/parts/Starboard_Panel_only_31675_verts.ply";
    fileManager->WriteModelFile(&modelInfo, "TieFighterStarBoardPanel.txt", "XYZNUVRGBA");

    // Asteroids
    modelInfo.modelName = "Asteroid01";
    modelInfo.meshPath = "assets/models/Asteroids/Asteroid_001_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "Asteroid01.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Asteroid02";
    modelInfo.meshPath = "assets/models/Asteroids/Asteroid_006_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "Asteroid02.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Asteroid03";
    modelInfo.meshPath = "assets/models/Asteroids/Asteroid_010_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "Asteroid03.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Asteroid04";
    modelInfo.meshPath = "assets/models/Asteroids/asteroid_group_xyznuvrbga_01.ply";
    fileManager->WriteModelFile(&modelInfo, "Asteroid04.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Asteroid05";
    modelInfo.meshPath = "assets/models/Asteroids/asteroid_group_xyznuvrbga_02.ply";
    fileManager->WriteModelFile(&modelInfo, "Asteroid05.txt", "XYZNUVRGBA");

    //SelectBox
    modelInfo.modelName = "Select_Box";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "selectBox.txt", "XYZNUV");

}