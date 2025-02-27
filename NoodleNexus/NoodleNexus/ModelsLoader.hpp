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
    modelInfo.modelName = "Mountain";
    modelInfo.meshPath = "assets/models/Mountain2.ply";
    fileManager->WriteModelFile(&modelInfo, "mountain.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Flag";
    modelInfo.meshPath = "assets/models/flag2.ply";
    fileManager->WriteModelFile(&modelInfo, "flag.txt", "XYZNUV");

    //modelInfo.modelName = "StuffMainAst";
    //modelInfo.meshPath = "assets/models/Miner/stuff_main_asteroid_xyz_n_rgba_uv.ply";
    //fileManager->WriteModelFile(&modelInfo, "stuffMainAst.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "IndusAsteroid";
    //modelInfo.meshPath = "assets/models/Miner/asteroid_industrial_xyz_n_rgba_uv.ply";
    //fileManager->WriteModelFile(&modelInfo, "industrialAst.txt", "XYZNUVRGBA");

    //// Consoles
    //modelInfo.modelName = "ConsoleStation";
    //modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_01_xyz_n_rgba_uv.ply";
    //fileManager->WriteModelFile(&modelInfo, "console.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "CornerConsole";
    //modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_Corner_01_xyz_n_rgba_uv.ply";
    //fileManager->WriteModelFile(&modelInfo, "cornerConsole.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "CornerConsoleRight";
    //modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_Corner_01_xyz_n_rgba_uv._Right.ply";
    //fileManager->WriteModelFile(&modelInfo, "cornerConsoleRight.txt", "XYZNUVRGBA");

    //// Screens
    //modelInfo.modelName = "Screen1";
    //modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_01_screen_1_xyz_n_rgba_uv.ply";
    //fileManager->WriteModelFile(&modelInfo, "screen1.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "Screen2";
    //modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_01_screen_2_xyz_n_rgba_uv.ply";
    //fileManager->WriteModelFile(&modelInfo, "screen2.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "Screen3";
    //modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_01_screen_3_xyz_n_rgba_uv.ply";
    //fileManager->WriteModelFile(&modelInfo, "screen3.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "CornerScreen1";
    //modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_Corner_01_screen_1_xyz_n_rgba_uv.ply";
    //fileManager->WriteModelFile(&modelInfo, "cScreen1.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "CornerScreen2";
    //modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_Corner_01_screen_2_xyz_n_rgba_uv.ply";
    //fileManager->WriteModelFile(&modelInfo, "cScreen2.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "CornerScreenRight1";
    //modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_Corner_01_screen_1_xyz_n_rgba_uv_Right.ply";
    //fileManager->WriteModelFile(&modelInfo, "cScreenRight1.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "CornerScreenRight2";
    //modelInfo.meshPath = "assets/models/Room/SM_Env_Consoles_Corner_01_screen_2_xyz_n_rgba_uv_Right.ply";
    //fileManager->WriteModelFile(&modelInfo, "cScreenRight2.txt", "XYZNUVRGBA");

    //// Room
    //modelInfo.modelName = "Ceiling";
    //modelInfo.meshPath = "assets/models/Room/Ceilings_xyz_n_uv_rbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "ceiling.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "Floor";
    //modelInfo.meshPath = "assets/models/Room/Floors_xyz_n_uv_rgba.ply";
    //fileManager->WriteModelFile(&modelInfo, "floors.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "Wall";
    //modelInfo.meshPath = "assets/models/Room/Walls_xyz_n_uv_rbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "walls.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "Props";
    //modelInfo.meshPath = "assets/models/Room/Props_xyz_n_uv_rbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "props.txt", "XYZNUVRGBA");

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