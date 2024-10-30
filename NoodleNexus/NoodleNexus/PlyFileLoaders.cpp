#include "PlyFileLoaders.h"

//#include <string>
#include <fstream>

// Takes:
// * 
// Returns: true if the file is loaded
bool ReadPlyModelFromFile_xyz_ci(s3DFileData& allFileInfo)
{

    // ************************************************************
        // Read the top of the file to get some info.

        // Read all the text until I get to the word "vertex"
//    std::ifstream plyFile("assets/models/bun_zipper_res3.ply");
    std::ifstream plyFile(allFileInfo.fileName);     // May also see .c_str() "c style string, char*"
    std::string token = "";

    if ( ! plyFile.is_open() )
    {
        return false;
    }

    //element vertex 1889
    while (token != "vertex")
    {
        plyFile >> token;
    };
    // Next info is the number of vertices
    //int numberOfVertices = 0;
    plyFile >> allFileInfo.numberOfVertices;


    //element face 3851
    while (token != "face")
    {
        plyFile >> token;
    };
    // Next info is the number of vertices
    //int numberOfTriangles = 0;
    plyFile >> allFileInfo.numberOfTriangles;

    //end_header
    // -0.0369122 0.127512 0.00276757 0.850855 0.5 
    while (token != "end_header")
    {
        plyFile >> token;
    };


    //property float x
    //property float y
    //property float z
    //property float confidence
    //property float intensity
    //struct sPlyVertex
    //{
    //    float x, y, z, confidence, intensity;
    //};
    ////
    //// and 
    //// 
    //// 3 572 584 1040 
    //struct sTriangle
    //{
    //    unsigned int vertIndex_0;
    //    unsigned int vertIndex_1;
    //    unsigned int vertIndex_2;
    //};

    // Load the data from the file
    // 
    // 
    // 

    allFileInfo.pPlyVertices = new sPlyVertex[allFileInfo.numberOfVertices];

    // end_header
    // -0.0369122 0.127512 0.00276757 0.850855 0.5
    for (unsigned index = 0; index != allFileInfo.numberOfVertices; index++)
    {
        plyFile >> allFileInfo.pPlyVertices[index].x;
        plyFile >> allFileInfo.pPlyVertices[index].y;
        plyFile >> allFileInfo.pPlyVertices[index].z;
        plyFile >> allFileInfo.pPlyVertices[index].confidence;
        plyFile >> allFileInfo.pPlyVertices[index].intensity;
    }

    // Load the triangle info from the file
//    sTriangle* pPlyTriangles = new sTriangle[numberOfTriangles];
    allFileInfo.pPlyTriangles = new sTriangle[allFileInfo.numberOfTriangles];
    for (unsigned int index = 0; index != allFileInfo.numberOfTriangles; index++)
    {
        // 3 737 103 17 
        int discard = 0;
        plyFile >> discard;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_0;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_1;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_2;
    }



    return true;
}





// Takes:
// * 
// Returns: true if the file is loaded
bool ReadPlyModelFromFile_xyz(s3DFileData& allFileInfo)
{

    // ************************************************************
        // Read the top of the file to get some info.

        // Read all the text until I get to the word "vertex"
//    std::ifstream plyFile("assets/models/bun_zipper_res3.ply");
    std::ifstream plyFile(allFileInfo.fileName);     // May also see .c_str() "c style string, char*"
    std::string token = "";

    if (!plyFile.is_open())
    {
        return false;
    }

    //element vertex 1889
    while (token != "vertex")
    {
        plyFile >> token;
    };
    // Next info is the number of vertices
    //int numberOfVertices = 0;
    plyFile >> allFileInfo.numberOfVertices;


    //element face 3851
    while (token != "face")
    {
        plyFile >> token;
    };
    // Next info is the number of vertices
    //int numberOfTriangles = 0;
    plyFile >> allFileInfo.numberOfTriangles;

    //end_header
    // -0.0369122 0.127512 0.00276757 0.850855 0.5 
    while (token != "end_header")
    {
        plyFile >> token;
    };


    // 
//    std::cout << numberOfVertices << std::endl;
//    std::cout << numberOfTriangles << std::endl;

    //property float x
    //property float y
    //property float z

    //struct sPlyVertex
    //{
    //    float x, y, z, confidence, intensity;
    //};
    ////
    //// and 
    //// 
    //// 3 572 584 1040 
    //struct sTriangle
    //{
    //    unsigned int vertIndex_0;
    //    unsigned int vertIndex_1;
    //    unsigned int vertIndex_2;
    //};

    // Load the data from the file
//    sPlyVertex* pPlyVertices = new sPlyVertex[numberOfVertices];
    allFileInfo.pPlyVertices = new sPlyVertex[allFileInfo.numberOfVertices];

    // end_header
    // -0.0369122 0.127512 0.00276757 0.850855 0.5
    for (unsigned index = 0; index != allFileInfo.numberOfVertices; index++)
    {
        plyFile >> allFileInfo.pPlyVertices[index].x;
        plyFile >> allFileInfo.pPlyVertices[index].y;
        plyFile >> allFileInfo.pPlyVertices[index].z;
        
        // Only has xyz, so stop here
//        plyFile >> allFileInfo.pPlyVertices[index].confidence;
//        plyFile >> allFileInfo.pPlyVertices[index].intensity;
        allFileInfo.pPlyVertices[index].confidence = 0;
        allFileInfo.pPlyVertices[index].intensity = 0;
    }

    // Load the triangle info from the file
//    sTriangle* pPlyTriangles = new sTriangle[numberOfTriangles];
    allFileInfo.pPlyTriangles = new sTriangle[allFileInfo.numberOfTriangles];
    for (unsigned int index = 0; index != allFileInfo.numberOfTriangles; index++)
    {
        // 3 737 103 17 
        int discard = 0;
        plyFile >> discard;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_0;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_1;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_2;
    }



    return true;
}

bool ReadPlyModelFromFile_xyz_uv(s3DFileData& allFileInfo) {
    std::ifstream plyFile(allFileInfo.fileName);
    std::string token = "";

    if (!plyFile.is_open()) {
        return false;
    }

    while (token != "vertex") {
        plyFile >> token;
    }
    plyFile >> allFileInfo.numberOfVertices;

    while (token != "face") {
        plyFile >> token;
    }
    plyFile >> allFileInfo.numberOfTriangles;

    while (token != "end_header") {
        plyFile >> token;
    }

    allFileInfo.pPlyVertices = new sPlyVertex[allFileInfo.numberOfVertices];

    for (unsigned index = 0; index != allFileInfo.numberOfVertices; index++) {
        plyFile >> allFileInfo.pPlyVertices[index].x;
        plyFile >> allFileInfo.pPlyVertices[index].y;
        plyFile >> allFileInfo.pPlyVertices[index].z;

        // Skip UV coordinates
        float u, v;
        plyFile >> u; // Read and discard the s coordinate
        plyFile >> v; // Read and discard the t coordinate

        allFileInfo.pPlyVertices[index].confidence = 0;
        allFileInfo.pPlyVertices[index].intensity = 0;
    }

    allFileInfo.pPlyTriangles = new sTriangle[allFileInfo.numberOfTriangles];
    for (unsigned int index = 0; index != allFileInfo.numberOfTriangles; index++) {
        int discard = 0;
        plyFile >> discard; // Read the number of vertices in the face (usually 3 for triangles)
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_0;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_1;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_2;
    }

    return true;
}


bool ReadPlyModelFromFile_xyz_uv_rgba(s3DFileData& allFileInfo) {
    std::ifstream plyFile(allFileInfo.fileName);
    std::string token = "";

    if (!plyFile.is_open()) {
        return false;
    }

    // Read until the "vertex" keyword, then get the vertex count
    while (token != "vertex") {
        plyFile >> token;
    }
    plyFile >> allFileInfo.numberOfVertices;

    // Read until the "face" keyword, then get the face count
    while (token != "face") {
        plyFile >> token;
    }
    plyFile >> allFileInfo.numberOfTriangles;

    // Continue until end of header is reached
    while (token != "end_header") {
        plyFile >> token;
    }

    // Allocate memory for vertices based on count
    allFileInfo.pPlyVertices = new sPlyVertex[allFileInfo.numberOfVertices];

    // Read vertex data including position, UV, and RGBA color
    for (unsigned index = 0; index != allFileInfo.numberOfVertices; index++) {
        plyFile >> allFileInfo.pPlyVertices[index].x;
        plyFile >> allFileInfo.pPlyVertices[index].y;
        plyFile >> allFileInfo.pPlyVertices[index].z;

        // Read UV coordinates
        plyFile >> allFileInfo.pPlyVertices[index].u;
        plyFile >> allFileInfo.pPlyVertices[index].v;

        // Read RGBA values (assuming values are given as 0-255 integers)
        int r, g, b, a;
        plyFile >> r >> g >> b >> a;
        allFileInfo.pPlyVertices[index].r = r / 255.0f; // Normalize to 0.0-1.0
        allFileInfo.pPlyVertices[index].g = g / 255.0f;
        allFileInfo.pPlyVertices[index].b = b / 255.0f;
        allFileInfo.pPlyVertices[index].a = a / 255.0f;

        // Optional: Set default values for confidence and intensity if needed
        allFileInfo.pPlyVertices[index].confidence = 0;
        allFileInfo.pPlyVertices[index].intensity = 0;
    }

    // Allocate memory for triangles based on count
    allFileInfo.pPlyTriangles = new sTriangle[allFileInfo.numberOfTriangles];
    for (unsigned int index = 0; index != allFileInfo.numberOfTriangles; index++) {
        int discard = 0;
        plyFile >> discard; // Read the number of vertices in the face (usually 3 for triangles)
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_0;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_1;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_2;
    }

    return true;
}
