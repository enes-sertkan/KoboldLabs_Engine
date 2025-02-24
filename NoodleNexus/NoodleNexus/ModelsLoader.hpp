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

    // Asteroid
    modelInfo.modelName = "MainAsteroid";
    modelInfo.meshPath = "assets/models/Miner/asteroid_main_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "mainAst.txt", "XYZNUVRGBA");

    modelInfo.modelName = "OtherAsteroid";
    modelInfo.meshPath = "assets/models/Miner/asteroid_other_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "otherAst.txt", "XYZNUVRGBA");

    modelInfo.modelName = "StuffMainAst";
    modelInfo.meshPath = "assets/models/Miner/stuff_main_asteroid_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "stuffMainAst.txt", "XYZNUVRGBA");

    modelInfo.modelName = "IndusAsteroid";
    modelInfo.meshPath = "assets/models/Miner/asteroid_industrial_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "industrialAst.txt", "XYZNUVRGBA");

    // Consoles
    modelInfo.modelName = "ConsoleStation";
    modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "console.txt", "XYZNUVRGBA");

    modelInfo.modelName = "CornerConsole";
    modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_Corner_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "cornerConsole.txt", "XYZNUVRGBA");

    // Screens
    modelInfo.modelName = "Screen1";
    modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_01_screen_1_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "screen1.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Screen2";
    modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_01_screen_2_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "screen2.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Screen3";
    modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_01_screen_3_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "screen3.txt", "XYZNUVRGBA");

    modelInfo.modelName = "CornerScreen1";
    modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_Corner_01_screen_1_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "cScreen1.txt", "XYZNUVRGBA");

    modelInfo.modelName = "CornerScreen2";
    modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_Corner_01_screen_2_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "cScreen2.txt", "XYZNUVRGBA");

    //// Room
    //modelInfo.modelName = "GodRay";
    //modelInfo.meshPath = "assets/models/Race_track/godray_xyznuvrgba.ply";
    //fileManager->WriteModelFile(&modelInfo, "godray.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "Clouds";
    //modelInfo.meshPath = "assets/models/Race_track/clouds_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "clouds.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "Plane";
    //modelInfo.meshPath = "assets/models/UFO.ply";
    //fileManager->WriteModelFile(&modelInfo, "Plane.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "Castle";
    //modelInfo.meshPath = "assets/models/Bodiam_Castle.ply";
    //fileManager->WriteModelFile(&modelInfo, "Castle.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "board";
    //modelInfo.meshPath = "assets/models/Class_Room/board_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "board.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "casier";
    //modelInfo.meshPath = "assets/models/Class_Room/casier_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "casier.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "ceiling";
    //modelInfo.meshPath = "assets/models/Class_Room/ceiling_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "ceiling.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "clock";
    //modelInfo.meshPath = "assets/models/Class_Room/clock_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "clock.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "desk";
    //modelInfo.meshPath = "assets/models/Class_Room/desk_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "desk.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "door";
    //modelInfo.meshPath = "assets/models/Class_Room/door_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "door.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "etagere";
    //modelInfo.meshPath = "assets/models/Class_Room/etagere_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "etagere.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "ground";
    //modelInfo.meshPath = "assets/models/Class_Room/ground_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "grand_casier.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "grand_casier";
    //modelInfo.meshPath = "assets/models/Class_Room/grand_casier_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "ground.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "teacher_desk";
    //modelInfo.meshPath = "assets/models/Class_Room/teacher_desk_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "teacher_d.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "wall_concrete";
    //modelInfo.meshPath = "assets/models/Class_Room/wall_concrete_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "wall_c.txt", "XYZNUVRGBA");

    //SelectBox
    modelInfo.modelName = "Select_Box";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "selectBox.txt", "XYZNUV");

}