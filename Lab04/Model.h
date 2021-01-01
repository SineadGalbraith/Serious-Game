#pragma once

// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
//#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

//Include GLM functions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"

//#include <glm/gtx/string_cast.hpp>

// GLFW includes
#include <GLFW\glfw3.h>
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
	std::vector < ModelData > dataArray;
	std::vector < std::string > textureArray;
private:
	ModelData load_mesh(const char* file_name);
};