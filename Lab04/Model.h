#pragma once

// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <vector> 

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"

typedef struct
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;

class Model {
public:
	Model();
	Model(const char* mesh, const char* texture);
	Model(const char* mesh);
	Model::ModelData modelMesh;
	std::vector < ModelData > getDataArray();
	std::vector < std::string > getTextureArray(); 
	std::vector < ModelData > getNonTextureObjects();
private:
	ModelData load_mesh(const char* file_name);
};