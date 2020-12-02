

// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Include GLM functions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"

// Loading photos
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/gtx/string_cast.hpp>

// GLFW includes
#include <GLFW\glfw3.h>

/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/

#define BIN_MESH "./models/rubbishBinCircular.dae"
#define FOOTPATH_MESH "./models/footpath.dae"
#define ROAD_MESH "./models/road.dae"
#define BUILDING1_MESH "./models/building1.dae"
#define BUILDING2_MESH "./models/building2.dae"
#define BUILDING3_MESH "./models/building3.dae"
#define GRASS_MESH "./models/grass.dae"
#define CAR_MESH "./models/car.obj"
#define WHEEL_MESH "./models/wheel.dae"
#define BOTTLE_MESH "./models/bottle.dae"

/*----------------------------------------------------------------------------
TEXTURES
----------------------------------------------------------------------------*/
const char *bin = "./textures/bin_texture.jpg";
const char *footpath = "./textures/footpath_texture.jpg";
const char *road = "./textures/road_texture.jpg";
const char *building1 = "./textures/building1_texture.jpg";
const char *building2 = "./textures/building2_texture.jpg";
const char *building3 = "./textures/building3_texture.jpg";
const char *grass = "./textures/grass_texture.jpg";
const char *bottle = "./textures/bottle_texture.jpg";

#pragma region SimpleTypes
typedef struct
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;
#pragma endregion SimpleTypes

using namespace std;

int width = 800;
int height = 600;

// ------------ CAMERA ------------
glm::vec3 cameraPosition = glm::vec3(0.0f, 20.0f, 220.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::vec3(0.0f);


bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = (float)width / 2.0;
float lastY = (float)height / 2.0;
float fov = 45.0f;

float delta;
static DWORD last_time = 0;

bool keyC = false;

// ------------ SHADER ------------
GLuint textureShaderProgramID, skyboxShaderProgramID, objectShaderProgramID;

// ------------ VBO/VAO SETUP ------------
const int i = 16;
GLuint VAO[i], VBO[i * 3], VTO[i];

// ------------ MESH SETUP ------------
ModelData bin_data, footpath_data, wall_data, road_data, building1_data, building2_data, building3_data, grass_data, car_data, wheel_data, bottle_data;

GLuint loc1, loc2, loc3;
GLfloat rotate_y = 0.0f;

// ------------ SKYBOX ------------
unsigned int skyboxVAO, skyboxVBO;
unsigned int cubemapTexture;

// ------------ CAR ------------
unsigned int carVAO, carVBO1, carVBO2;
unsigned int wheelsVAO, wheelsVBO1, wheelsVBO2;

// ------------ TEXTURE ------------
float uvScalar = 0;

// ------------ LIGHTING ------------
glm::vec3 lightPos(-20.0f, 100.0f, 100.0f);

vector<std::string> faces
{
	"./skybox/right.bmp",
	"./skybox/left.bmp",
	"./skybox/top.bmp",
	"./skybox/bottom.bmp",
	"./skybox/front.bmp",
	"./skybox/back.bmp"
};

float skyboxVertices[] = {
	-500.0f,  500.0f, -500.0f,
	-500.0f, -500.0f, -500.0f,
	 500.0f, -500.0f, -500.0f,
	 500.0f, -500.0f, -500.0f,
	 500.0f,  500.0f, -500.0f,
	-500.0f,  500.0f, -500.0f,

	-500.0f, -500.0f,  500.0f,
	-500.0f, -500.0f, -500.0f,
	-500.0f,  500.0f, -500.0f,
	-500.0f,  500.0f, -500.0f,
	-500.0f,  500.0f,  500.0f,
	-500.0f, -500.0f,  500.0f,

	 500.0f, -500.0f, -500.0f,
	 500.0f, -500.0f,  500.0f,
	 500.0f,  500.0f,  500.0f,
	 500.0f,  500.0f,  500.0f,
	 500.0f,  500.0f, -500.0f,
	 500.0f, -500.0f, -500.0f,

	-500.0f, -500.0f,  500.0f,
	-500.0f,  500.0f,  500.0f,
	 500.0f,  500.0f,  500.0f,
	 500.0f,  500.0f,  500.0f,
	 500.0f, -500.0f,  500.0f,
	-500.0f, -500.0f,  500.0f,

	-500.0f,  500.0f, -500.0f,
	 500.0f,  500.0f, -500.0f,
	 500.0f,  500.0f,  500.0f,
	 500.0f,  500.0f,  500.0f,
	-500.0f,  500.0f,  500.0f,
	-500.0f,  500.0f, -500.0f,

	-500.0f, -500.0f, -500.0f,
	-500.0f, -500.0f,  500.0f,
	 500.0f, -500.0f, -500.0f,
	 500.0f, -500.0f, -500.0f,
	-500.0f, -500.0f,  500.0f,
	 500.0f, -500.0f,  500.0f
};


#pragma region MESH LOADING
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

ModelData load_mesh(const char* file_name) {
	ModelData modelData;

	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name,
		aiProcess_Triangulate | aiProcess_PreTransformVertices
	);

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return modelData;
	}

	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		modelData.mPointCount += mesh->mNumVertices;
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				modelData.mVertices.push_back(vec3(vp->x, vp->y, vp->z));
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				modelData.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				modelData.mTextureCoords.push_back(vec2(vt->x, vt->y));
			}
			if (mesh->HasTangentsAndBitangents()) {
				/* You can extract tangents and bitangents here              */
				/* Note that you might need to make Assimp generate this     */
				/* data for you. Take a look at the flags that aiImportFile  */
				/* can take.                                                 */
			}
		}
	}

	aiReleaseImport(scene);
	return modelData;
}

#pragma endregion MESH LOADING

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS
char* readShaderSource(const char* shaderFile) {
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		std::cerr << "Error creating shader..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	const char* pShaderSource = readShaderSource(pShaderText);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024] = { '\0' };
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cerr << "Error compiling "
			<< (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader program: " << InfoLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders(const char* vertexShader, const char* fragmentShader)
{
	GLuint shaderProgramID;
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		std::cerr << "Error creating shader program..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// NOTE: change from default shaders to input shaders
	AddShader(shaderProgramID, vertexShader, GL_VERTEX_SHADER);
	AddShader(shaderProgramID, fragmentShader, GL_FRAGMENT_SHADER);


	GLint Success = 0;
	GLchar ErrorLog[1024] = { '\0' };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	//glUseProgram(shaderProgramID);
	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS

#pragma region TEXTURE_FUNCTIONS
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void loadTexture(const char* texture, int i) {
	glGenTextures(1, &VTO[i]);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(texture, &width, &height, &nrComponents, 0);

	GLenum format;
	if (nrComponents == 1)
		format = GL_RED;
	else if (nrComponents == 3)
		format = GL_RGB;
	else if (nrComponents == 4)
		format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, VTO[i]);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
}
#pragma endregion TEXTURE_FUNCTIONS
// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS
void generateObjectBufferMesh(std::vector < ModelData > dataArray, std::vector <std::string> textureArray) {

	int width, height, nrChannels;
	unsigned char *data;
	int counter = 0;

	loc1 = glGetAttribLocation(textureShaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(textureShaderProgramID, "vertex_normal");
	loc3 = glGetAttribLocation(textureShaderProgramID, "vertex_texture");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (dataArray.size() == textureArray.size()) {
		for (int i = 0; i < dataArray.size(); i++) {
			glGenBuffers(1, &VBO[counter]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[counter]);
			glBufferData(GL_ARRAY_BUFFER, dataArray[i].mPointCount * sizeof(vec3), &dataArray[i].mVertices[0], GL_STATIC_DRAW);

			glGenBuffers(1, &VBO[counter+1]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[counter+1]);
			glBufferData(GL_ARRAY_BUFFER, dataArray[i].mPointCount * sizeof(vec3), &dataArray[i].mNormals[0], GL_STATIC_DRAW);

			glGenBuffers(1, &VBO[counter + 2]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[counter+2]);
			glBufferData(GL_ARRAY_BUFFER, dataArray[i].mPointCount * sizeof(vec2), &dataArray[i].mTextureCoords[0], GL_STATIC_DRAW);

			glGenVertexArrays(1, &VAO[i]);
			glBindVertexArray(VAO[i]);

			glEnableVertexAttribArray(loc1);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[counter]);
			glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			glEnableVertexAttribArray(loc2);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[counter + 1]);
			glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			glEnableVertexAttribArray(loc3);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[counter + 2]);
			glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

			counter += 3;

			loadTexture(textureArray[i].c_str(), i);
		}
	}
}

void generateModels() {

	std::vector < ModelData > dataArray;
	std::vector < std::string > textureArray;

	// ------------ BIN ------------
	bin_data = load_mesh(BIN_MESH);
	dataArray.push_back(bin_data);
	textureArray.push_back(bin);
	// ------------ FOOTPATH ------------
	footpath_data = load_mesh(FOOTPATH_MESH);
	dataArray.push_back(footpath_data);
	textureArray.push_back(footpath);
	// ------------ ROAD ------------
	road_data = load_mesh(ROAD_MESH);
	dataArray.push_back(road_data);
	textureArray.push_back(road);
	// ------------ BUILDING1 ------------
	building1_data = load_mesh(BUILDING1_MESH);
	dataArray.push_back(building1_data);
	textureArray.push_back(building1);
	// ------------ BUILDING2 ------------
	building2_data = load_mesh(BUILDING2_MESH);
	dataArray.push_back(building2_data);
	textureArray.push_back(building2);
	// ------------ BUILDING3 ------------
	building3_data = load_mesh(BUILDING3_MESH);
	dataArray.push_back(building3_data);
	textureArray.push_back(building3);
	// ------------ GRASS ------------
	grass_data = load_mesh(GRASS_MESH);
	dataArray.push_back(grass_data);
	textureArray.push_back(grass);
	// ------------ BOTTLE ------------
	bottle_data = load_mesh(BOTTLE_MESH);
	dataArray.push_back(bottle_data);
	textureArray.push_back(bottle);

	generateObjectBufferMesh(dataArray, textureArray);
}

void generateSkybox() {
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void generateObjects() {
	car_data = load_mesh(CAR_MESH);
	wheel_data = load_mesh(WHEEL_MESH);

	loc1 = glGetAttribLocation(objectShaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(objectShaderProgramID, "vertex_normal");

	// -------------------------------- CAR --------------------------------
	glGenVertexArrays(1, &carVAO);
	glGenBuffers(1, &carVBO1);

	glBindBuffer(GL_ARRAY_BUFFER, carVBO1);
	glBufferData(GL_ARRAY_BUFFER, car_data.mPointCount * sizeof(vec3), &car_data.mVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &carVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, carVBO2);
	glBufferData(GL_ARRAY_BUFFER, car_data.mPointCount * sizeof(vec3), &car_data.mNormals[0], GL_STATIC_DRAW);

	glBindVertexArray(carVAO);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, carVBO1);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, carVBO2);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// -------------------------------- WHEEL --------------------------------
	glGenVertexArrays(1, &wheelsVAO);
	glGenBuffers(1, &wheelsVBO1);

	glBindBuffer(GL_ARRAY_BUFFER, wheelsVBO1);
	glBufferData(GL_ARRAY_BUFFER, wheel_data.mPointCount * sizeof(vec3), &wheel_data.mVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &wheelsVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, wheelsVBO2);
	glBufferData(GL_ARRAY_BUFFER, wheel_data.mPointCount * sizeof(vec3), &wheel_data.mNormals[0], GL_STATIC_DRAW);

	glBindVertexArray(wheelsVAO);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, wheelsVBO1);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, wheelsVBO2);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

#pragma endregion VBO_FUNCTIONS

void display() {

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);

	glDepthFunc(GL_LESS); 
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	int matrix_location, view_mat_location, proj_mat_location;

	// ------------------------------------- PROJECTION ------------------------------------- (texture Shader)
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(textureShaderProgramID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

	// ------------------------------------- CAMERA ------------------------------------- (texture Shader)
	glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	glUniformMatrix4fv(glGetUniformLocation(textureShaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));

	// ------------------------------------- SKYBOX ------------------------------------- (skybox Shader)
	glDepthFunc(GL_LEQUAL);
	glUseProgram(skyboxShaderProgramID);

	view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

	glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgramID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	// ------------------------------------- CAR ------------------------------------- (object Shader)
	glUseProgram(objectShaderProgramID);
	glBindVertexArray(carVAO);

	glm::vec3 objectColor = glm::vec3(0.0f, 0.5f, 0.5f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(glGetUniformLocation(objectShaderProgramID, "objectColor"), 1, &objectColor[0]);
	glUniform3fv(glGetUniformLocation(objectShaderProgramID, "lightColor"), 1, &lightColor[0]);
	glUniform3fv(glGetUniformLocation(objectShaderProgramID, "lightPos"), 1, &lightPos[0]);
	glUniform3fv(glGetUniformLocation(objectShaderProgramID, "viewPos"), 1, &cameraPosition[0]);

	glUniformMatrix4fv(glGetUniformLocation(objectShaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(objectShaderProgramID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

	glm::mat4 carModel = glm::mat4(1.0f);
	carModel = glm::translate(carModel, glm::vec3(-90.0f, 9.0f, 40.0f));
	matrix_location = glGetUniformLocation(objectShaderProgramID, "model");
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(carModel));

	glDrawArrays(GL_TRIANGLES, 0, car_data.mPointCount);

	// ------------------------------------- WHEELS ------------------------------------- (object Shader)
	glBindVertexArray(wheelsVAO);

	objectColor = glm::vec3(0.2f, 0.2f, 0.2f);
	glUniform3fv(glGetUniformLocation(objectShaderProgramID, "objectColor"), 1, &objectColor[0]);

	// Front-right
	glm::mat4 wheelModel = glm::mat4(1.0f);
	wheelModel = glm::translate(wheelModel, glm::vec3(-80.0f, 5.0f, 25.0f));
	wheelModel = glm::rotate(wheelModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	wheelModel = glm::rotate(wheelModel, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(wheelModel));
	glDrawArrays(GL_TRIANGLES, 0, wheel_data.mPointCount);

	// Front-left
	wheelModel = glm::mat4(1.0f);
	wheelModel = glm::translate(wheelModel, glm::vec3(-99.0f, 5.0f, 25.0f));
	wheelModel = glm::rotate(wheelModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	wheelModel = glm::rotate(wheelModel, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(wheelModel));
	glDrawArrays(GL_TRIANGLES, 0, wheel_data.mPointCount);

	// Back-right 
	wheelModel = glm::mat4(1.0f);
	wheelModel = glm::translate(wheelModel, glm::vec3(-81.0f, 5.0f, 56.0f));
	wheelModel = glm::rotate(wheelModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	wheelModel = glm::rotate(wheelModel, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(wheelModel));
	glDrawArrays(GL_TRIANGLES, 0, wheel_data.mPointCount);

	// Back-left
	wheelModel = glm::mat4(1.0f);
	wheelModel = glm::translate(wheelModel, glm::vec3(-99.0f, 5.0f, 56.0f));
	wheelModel = glm::rotate(wheelModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	wheelModel = glm::rotate(wheelModel, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(wheelModel));
	glDrawArrays(GL_TRIANGLES, 0, wheel_data.mPointCount);

	//------------------------------------- BIN ------------------------------------- (texture Shader)
	glUseProgram(textureShaderProgramID);

	// Diffuse material
	glUniform1d(glGetUniformLocation(textureShaderProgramID, "material.diffuse"), 10);

	// UV Scalar
	uvScalar = 1;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);
	// Light & View Position
	glUniform3fv(glGetUniformLocation(textureShaderProgramID, "light.position"), 1, &lightPos[0]);
	glUniform3fv(glGetUniformLocation(textureShaderProgramID, "viewPos"), 1, &cameraPosition[0]);

	// Material and Light Attributes
	glm::vec3 mSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 lAmbient = glm::vec3(0.6f, 0.6f, 0.6f);
	glm::vec3 lDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 lSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(glGetUniformLocation(textureShaderProgramID, "light.ambient"), 1, &lAmbient[0]);
	glUniform3fv(glGetUniformLocation(textureShaderProgramID, "light.diffuse"), 1, &lDiffuse[0]);
	glUniform3fv(glGetUniformLocation(textureShaderProgramID, "light.specular"), 1, &lSpecular[0]);

	glUniform3fv(glGetUniformLocation(textureShaderProgramID, "material.specular"), 1, &mSpecular[0]);
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "material.shininess"), 32.0f);
	
	// View amd Projection Matrix
	glUniformMatrix4fv(glGetUniformLocation(textureShaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(textureShaderProgramID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

	// Texture
	glBindTexture(GL_TEXTURE_2D, VTO[0]);
	glBindVertexArray(VAO[0]);

	// Bin 1
	glm::mat4 binModel = glm::mat4(1.0f);
	binModel = glm::translate(binModel, glm::vec3(-20.0, 3.0, 135.0f));
	//model = glm::rotate(model, glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix_location = glGetUniformLocation(textureShaderProgramID, "model");
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(binModel));
	glDrawArrays(GL_TRIANGLES, 0, bin_data.mPointCount);

	// Bin 2
	binModel = glm::mat4(1.0f);
	binModel = glm::translate(binModel, glm::vec3(20.0, 3.0, 20.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(binModel));
	glDrawArrays(GL_TRIANGLES, 0, bin_data.mPointCount);

	// Bin 3
	binModel = glm::mat4(1.0f);
	binModel = glm::translate(binModel, glm::vec3(-20.0, 3.0, -100.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(binModel));
	glDrawArrays(GL_TRIANGLES, 0, bin_data.mPointCount);

	// Bin 4
	binModel = glm::mat4(1.0f);
	binModel = glm::translate(binModel, glm::vec3(20.0, 3.0, -220.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(binModel));
	glDrawArrays(GL_TRIANGLES, 0, bin_data.mPointCount);

	// ------------------------------------- FOOTPATHS ------------------------------------- (texture Shader)
	// uvScalar
	uvScalar = 15;	
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[1]);
	glBindVertexArray(VAO[1]);

	// Footpath 1
	glm::mat4 footpathModel = glm::mat4(1.0f);
	footpathModel = glm::translate(footpathModel, glm::vec3(0.0f, 2.0f, 0.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(footpathModel));
	glDrawArrays(GL_TRIANGLES, 0, footpath_data.mPointCount);

	// Footpath 2
	footpathModel = glm::mat4(1.0f);
	footpathModel = glm::translate(footpathModel, glm::vec3(-150.0f, 2.0f, 0.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(footpathModel));
	glDrawArrays(GL_TRIANGLES, 0, footpath_data.mPointCount); 

	// ------------------------------------- ROAD ------------------------------------- (texture Shader)
	// uvScalar
	uvScalar = 5;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[2]);
	glBindVertexArray(VAO[2]);

	// Road
	glm::mat4 roadModel = glm::mat4(1.0f);
	roadModel = glm::translate(roadModel, glm::vec3(-75.0f, 0.0f, -100.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(roadModel));
	glDrawArrays(GL_TRIANGLES, 0, road_data.mPointCount);

	// ------------------------------------- BUILDING1 ------------------------------------- (texture Shader)
	// uvScalar
	uvScalar = 10;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	//Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[3]);
	glBindVertexArray(VAO[3]);

	// Building 1 - 1
	glm::mat4 building1Model = glm::mat4(1.0f);
	building1Model = glm::translate(building1Model, glm::vec3(-185.0f, 0.0f, 20.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(building1Model));
	glDrawArrays(GL_TRIANGLES, 0, building1_data.mPointCount);

	// Building 1 - 2
	building1Model = glm::mat4(1.0f);
	building1Model = glm::translate(building1Model, glm::vec3(-185.0f, 0.0f, -98.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(building1Model));
	glDrawArrays(GL_TRIANGLES, 0, building1_data.mPointCount);

	// ------------------------------------- BUILDING2 ------------------------------------- (texture Shader)
	// uvScalar
	uvScalar = 5;	
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[4]);
	glBindVertexArray(VAO[4]);

	// Building 2 - 1
	glm::mat4 building2Model = glm::mat4(1.0f);
	building2Model = glm::translate(building2Model, glm::vec3(-185.0f, 0.0f, -39.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(building2Model));
	glDrawArrays(GL_TRIANGLES, 0, building2_data.mPointCount);

	// ------------------------------------- BUILDING3 ------------------------------------- (texture Shader)
	// uvScalar
	uvScalar = 10;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[5]);
	glBindVertexArray(VAO[5]);

	// Building 3 - 1
	glm::mat4 building3Model = glm::mat4(1.0f);
	building3Model = glm::translate(building3Model, glm::vec3(-185.0f, 0.0f, 100.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(building3Model));
	glDrawArrays(GL_TRIANGLES, 0, building3_data.mPointCount);
	
	// Building 3 - 2
	building3Model = glm::mat4(1.0f);
	building3Model = glm::translate(building3Model, glm::vec3(-185.0f, 0.0f, 218.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(building3Model));
	glDrawArrays(GL_TRIANGLES, 0, building3_data.mPointCount);

	// ------------------------------------- GRASS ------------------------------------- (texture Shader)
	// uvScalar
	uvScalar = 50;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[6]);
	glBindVertexArray(VAO[6]);

	// Grass
	glm::mat4 grassModel = glm::mat4(1.0f);
	grassModel = glm::translate(grassModel, glm::vec3(20.0f, -12.0f, -20.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(grassModel));
	glDrawArrays(GL_TRIANGLES, 0, grass_data.mPointCount);

	// ------------------------------------- BOTTLE ------------------------------------- (texture Shader)
	// uvScalar
	uvScalar = 20;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[7]);
	glBindVertexArray(VAO[7]);

	// Bottle 1
	glm::mat4 bottleModel = glm::mat4(1.0f);
	bottleModel = glm::translate(bottleModel, glm::vec3(-16.0f, 4.0f, 140.0f));
	bottleModel = glm::rotate(bottleModel, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
	glDrawArrays(GL_TRIANGLES, 0, bottle_data.mPointCount);

	// Bottle 2
	bottleModel = glm::mat4(1.0f);
	bottleModel = glm::translate(bottleModel, glm::vec3(-18.0f, 4.0f, 110.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
	glDrawArrays(GL_TRIANGLES, 0, bottle_data.mPointCount);

	// Bottle 3
	bottleModel = glm::mat4(1.0f);
	bottleModel = glm::translate(bottleModel, glm::vec3(14.0f, 4.0f, 120.0f));
	bottleModel = glm::rotate(bottleModel, glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
	glDrawArrays(GL_TRIANGLES, 0, bottle_data.mPointCount);


	// Bottle 4
	bottleModel = glm::mat4(1.0f);
	bottleModel = glm::translate(bottleModel, glm::vec3(-8.0f, 2.0f, 50.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
	glDrawArrays(GL_TRIANGLES, 0, bottle_data.mPointCount);


	// ---------------------------------------------------------------------------------

	glutSwapBuffers();

}

void updateScene() {

	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

	// Rotate the model slowly around the y axis at 20 degrees per second
	rotate_y += 20.0f * delta;
	rotate_y = fmodf(rotate_y, 360.0f);

	// Draw the next frame
	glutPostRedisplay();
}


void init()
{
	// Set up the shaders
	textureShaderProgramID = CompileShaders("./shaders/textureVertexShader.txt", "./shaders/textureFragmentShader.txt");
	skyboxShaderProgramID = CompileShaders("./shaders/skyboxVertexShader.txt", "./shaders/skyboxFragmentShader.txt");
	objectShaderProgramID = CompileShaders("./shaders/objectVertexShader.txt", "./shaders/objectFragmentShader.txt");
	// generate Skybox and load Skybox images
	generateSkybox();
	cubemapTexture = loadCubemap(faces);
	// Generate the rest of the Models (both with textures and without)
	generateModels();
	generateObjects();
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {
	float cameraSpeed = 300.0f * delta;
	switch (key) {
	// Move Camera Left
	case 'a':
		if (cameraPosition.x > -140) {
			cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		break;
	// Use camera to move around the screen
	case 'c':
		if (!keyC) {
			keyC = true;
		}
		else if (keyC) {
			keyC = false;
		}
		break;
	// Move Camera Right
	case 'd':
		if (cameraPosition.x < 20) {
			cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		break;
	// Move Camera Down
	case 'j':
		if (cameraPosition.y >= 10) {
			cameraPosition -= cameraSpeed * up;
		}
		break;
	// Move Camera Backwards
	case 's':
		if (cameraPosition.z < 220) {
			cameraPosition -= cameraSpeed * cameraFront;
		}
		break;
	// Move Camera Up
	case 'u':
		if (cameraPosition.y < 30) {
			cameraPosition += cameraSpeed * up;
		}
		break;
	// Move Camera Forwards
	case 'w':
		if (cameraPosition.z > -300) {
			cameraPosition += cameraSpeed * cameraFront;
		}
		break;
	// For button 'x' on the keyboard, the player will have the option to quit the game.
	case 'x':
		int messageBoxID = MessageBox(NULL, "Do you want to quit the game?", "Menu", MB_YESNO | MB_ICONQUESTION);

		switch (messageBoxID) {
		case IDYES:
			exit(0);
			break;
		case IDNO:
			break;
		}
		break;
	}

}

// -------------------------- MOUSE CAMERA MOVEMENTS --------------------------
void mouseCameraMovement(int x, int y) {
	if (keyC) {
		if (firstMouse) {
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		float xOffset = x - lastX;
		float yOffset = lastY - y;
		lastX = x;
		lastY = y;

		const float sensitivity = 0.05f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);

		cameraRight = glm::normalize(glm::cross(cameraFront, up));
		cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
	}
	
}

// -------------------------- ZOOM FUNCTIONALITY --------------------------

void zoomCameraMovement(int button, int direction, int x, int y) {
	float cameraSpeed = 10.0f;
	if (cameraPosition.z > -300) {
		if (cameraPosition.y > 5) {
			// If the wheel direction is positive, zoom in
			if (direction > 0) {
				cameraPosition += cameraSpeed * cameraFront;
			}
		}
	}
	if (cameraPosition.z < 220) {
		if (cameraPosition.y + 2> 5) {
			// If the wheel direction is negative, zoom out
			if (direction < 0) {
				cameraPosition -= cameraSpeed * cameraFront;
			}
		}
	}
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Eco Game");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	glutPassiveMotionFunc(mouseCameraMovement);
	glutMouseWheelFunc(zoomCameraMovement);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	//Set up your objects and shaders
	init();
	//Begin infinite event loop
	glutMainLoop();
	return 0;
}
