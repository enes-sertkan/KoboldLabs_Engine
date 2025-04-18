# Install script for directory: D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.4.3-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/lib/Debug/assimp-vc143-mtd.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/lib/Release/assimp-vc143-mt.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/lib/MinSizeRel/assimp-vc143-mt.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/lib/RelWithDebInfo/assimp-vc143-mt.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.4.3" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/bin/Debug/assimp-vc143-mtd.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/bin/Release/assimp-vc143-mt.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/bin/MinSizeRel/assimp-vc143-mt.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/bin/RelWithDebInfo/assimp-vc143-mt.dll")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/anim.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/aabb.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/ai_assert.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/camera.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/color4.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/color4.inl"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/code/../include/assimp/config.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/ColladaMetaData.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/commonMetaData.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/defs.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/cfileio.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/light.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/material.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/material.inl"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/matrix3x3.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/matrix3x3.inl"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/matrix4x4.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/matrix4x4.inl"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/mesh.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/ObjMaterial.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/pbrmaterial.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/GltfMaterial.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/postprocess.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/quaternion.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/quaternion.inl"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/scene.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/metadata.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/texture.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/types.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/vector2.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/vector2.inl"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/vector3.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/vector3.inl"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/version.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/cimport.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/AssertHandler.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/importerdesc.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Importer.hpp"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/DefaultLogger.hpp"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/ProgressHandler.hpp"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/IOStream.hpp"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/IOSystem.hpp"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Logger.hpp"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/LogStream.hpp"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/NullLogger.hpp"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/cexport.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Exporter.hpp"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/DefaultIOStream.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/DefaultIOSystem.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/SceneCombiner.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/fast_atof.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/qnan.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/BaseImporter.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Hash.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/ParsingUtils.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/StreamReader.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/StreamWriter.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/StringComparison.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/StringUtils.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/SGSpatialSort.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/GenericProperty.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/SpatialSort.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/SmallVector.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/SmoothingGroups.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/SmoothingGroups.inl"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/StandardShapes.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/RemoveComments.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Subdivision.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Vertex.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/LineSplitter.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/TinyFormatter.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Profiler.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/LogAux.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Bitmap.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/XMLTools.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/IOStreamBuffer.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/CreateAnimMesh.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/XmlParser.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/BlobIOSystem.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/MathFunctions.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Exceptional.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/ByteSwapper.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Compiler/poppack1.h"
    "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/bin/Debug/assimp-vc143-mtd.pdb")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/bin/Release/assimp-vc143-mt.pdb")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/bin/MinSizeRel/assimp-vc143-mt.pdb")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Fork/KoboldLabs_Engine/NoodleNexus/NoodleNexus/assimp/build/bin/RelWithDebInfo/assimp-vc143-mt.pdb")
  endif()
endif()

