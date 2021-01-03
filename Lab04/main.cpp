// Includes
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
//#include <stdio.h>

// Include GLUT
#include <GL/glew.h>
#include <GL/freeglut.h>

// Include GLM 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Loading photos
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// GLFW includes
#include <GLFW\glfw3.h>

// Include Model Class
#include "Model.h"
Model model;

/*----------------------------------------------------------------------------
MESHES
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
#define CHEST_MESH "./models/chest.obj"
#define HEAD_MESH "./models/head.obj"
#define HAIR_MESH "./models/hair.obj"
#define LEFT_LEG_MESH "./models/leftLeg.dae"
#define RIGHT_LEG_MESH "./models/rightLeg.dae"
#define LEFT_FOOT_MESH "./models/leftFoot.dae"
#define RIGHT_FOOT_MESH "./models/rightFoot.dae"
#define BOTTLE_MESH "./models/bottle.dae"
#define BAG_MESH "./models/rubbishBag.dae"

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
const char *chest = "./textures/chest_texture.jpg";
const char *skin = "./textures/skin_texture.jpg";
const char *jean = "./textures/legs_texture.jpg";
const char *shoe = "./textures/shoe_texture.jpg";
const char *hair = "./textures/hair_texture.jpg";
const char *rubbishBag = "./textures/rubbishBag_texture.jpg";

using namespace std;
int width = 800;
int height = 600;

// ------------ CAMERA ------------
glm::vec3 cameraPosition = glm::vec3(0.0f, 17.0f, 220.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 startingCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::vec3(0.0f);

// ------------ CAMERA SCROLLING ------------
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = (float)width / 2.0;
float lastY = (float)height / 2.0;

// ------------ SPEED ------------
float delta;
static DWORD last_time = 0;

// ------------ ROTATION ------------
GLfloat rotate_y = 0.0f;

// ------------ KEY PRESS TOGGLE ------------
bool keyC = false;
bool keyM = false;
bool keyO = false;
bool keyP = false;

// ------------ SHADERS ------------
GLuint textureShaderProgramID, skyboxShaderProgramID, objectShaderProgramID;
float uvScalar = 0;

// ------------ VBO/VAO SETUP ------------
const int i = 16;
const int j = 3;
GLuint VAO[i], VBO[i * 3], VTO[i];
GLuint objectsVAO[j], objectsVBO[j * 3];
GLuint loc1, loc2, loc3;

// ------------ MODEL RELATED ARRAYS ------------
std::vector < ModelData > meshData;
std::vector < std::string > textures;
std::vector < ModelData > nonTextureModels;

// ------------ CAR ------------
glm::vec3 carPos1 = glm::vec3(-90.0f, 9.0f, 5000.0f);
glm::vec3 carPos2 = glm::vec3(-60.0f, 9.0f, -10000.0f);
bool drawCar1 = true;
bool drawCar2 = true;

// ------------ LIGHTING ------------
glm::vec3 lightPos(-20.0f, 100.0f, 100.0f);

// ------------ OVERHEAD VIEW ------------
glm::vec3 overheadPos = glm::vec3(-50.0f, 350.0f, 200.0f);
glm::vec3 overheadCameraFront = glm::vec3(0.5f, -1.0f, -0.85f);
glm::vec3 overheadUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lastCameraPos;
glm::vec3 lastCameraFront;
glm::vec3 lastCameraUp;

// ------------ FRED (I.E PLAYER CHARACTER) ------------
glm::vec3 fredPos = glm::vec3(0.0f, 17.0f, 222.0f);

// ------------ SCORE ------------
int playerScore = 0;

// ------------ PICK UP AND DEPOSIT OBJECTS ------------
bool pressP = false;
bool pressM = false;

// ------------ INVENTORY ------------
int bottleInventory = 0;
int bagInventory = 0;

// ------------ OBJECTS ------------
int xDistance = 10;
int zDistance = 50;

// ------------ BOTTLES ------------
bool bottle1 = true;
bool bottle2 = true;
bool bottle3 = true;
bool bottle4 = true;
bool bottle5 = true;
bool bottle6 = true;
bool bottle7 = true;
bool bottle8 = true;
bool bottle9 = true;
bool bottle10 = true;
glm::vec3 bottle1Pos = glm::vec3(-16.0f, 3.8f, 180.0f);
glm::vec3 bottle2Pos = glm::vec3(-18.0f, 3.8f, 130.0f);
glm::vec3 bottle3Pos = glm::vec3(14.0f, 3.8f, 95.0f);
glm::vec3 bottle4Pos = glm::vec3(-12.0f, 3.8f, 50.0f);
glm::vec3 bottle5Pos = glm::vec3(18.0f, 3.8f, 0.0f);
glm::vec3 bottle6Pos = glm::vec3(5.0f, 3.8f, -40.0f);
glm::vec3 bottle7Pos = glm::vec3(-18.0f, 3.8f, -70.0f);
glm::vec3 bottle8Pos = glm::vec3(17.0f, 3.8f, -125.0f);
glm::vec3 bottle9Pos = glm::vec3(-10.0f, 3.8f, -163.0f);
glm::vec3 bottle10Pos = glm::vec3(7.0f, 3.8f, -200.0f);

// ------------ BAGS ------------
bool bag1 = true;
bool bag2 = true;
bool bag3 = true;
bool bag4 = true;
bool bag5 = true;
glm::vec3 bag1Pos = glm::vec3(-22.0f, 4.0f, 138.0f);
glm::vec3 bag2Pos = glm::vec3(-20.0f, 5.0f, 65.0f);
glm::vec3 bag3Pos = glm::vec3(14.0f, 4.0f, -20.0f);
glm::vec3 bag4Pos = glm::vec3(-15.5f, 4.0f, -100.0f);
glm::vec3 bag5Pos = glm::vec3(20.0f, 5.0f, -180.0f);

// ------------ BINS ------------
glm::vec3 bin1Pos = glm::vec3(-20.0, 3.0, 135.0f);
glm::vec3 bin2Pos = glm::vec3(20.0, 3.0, 20.0f);
glm::vec3 bin3Pos = glm::vec3(-20.0, 3.0, -100.0f);
glm::vec3 bin4Pos = glm::vec3(20.0, 3.0, -220.0f);

// ------------ SKYBOX ------------
unsigned int skyboxVAO, skyboxVBO;
unsigned int cubemapTexture;
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

// ------------ SHADER FUNCTIONS (UNCHANGED) ------------
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

// ------------ TEXTURE FUNCTIONS FOR MODELS AND SKYBOX------------
#pragma region TEXTURE_FUNCTIONS
// Function to load the faces (outlined above) and assemble the skybox.
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int skyboxTextureID;
	glGenTextures(1, &skyboxTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return skyboxTextureID;
}

// Function to load the textures of each individual object. The beginning of this function is very similar to the previous function.
// The main difference in this function is that it checks the number of channels in the image to determine the type of image (e.g
// JPG or PNG).
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

// ------------ VBO FUNCTIONS ------------
#pragma region VBO_FUNCTIONS
// This function is an updated version of the function originally provided with the same code. This function
// caters for models that have an associated texture specifically. The dataArray and textureArray that have 
// been returned from the model class are passed in and VAOs, VBOs and VTOs are created respectively for each 
// object in the arrays.
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

			glGenBuffers(1, &VBO[counter + 1]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[counter + 1]);
			glBufferData(GL_ARRAY_BUFFER, dataArray[i].mPointCount * sizeof(vec3), &dataArray[i].mNormals[0], GL_STATIC_DRAW);

			glGenBuffers(1, &VBO[counter + 2]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[counter + 2]);
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

// This function is similar to the one above only it caters for the models without an associated texture. The process
// is pretty much the same but there are no texture coordinates.
void generateNonTextureObjects(std::vector <ModelData> nonTextureObjects) {
	loc1 = glGetAttribLocation(objectShaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(objectShaderProgramID, "vertex_normal");
	
	int width, height, nrChannels;
	unsigned char *data;
	int counter = 0;

	for (int i = 0; i < nonTextureObjects.size(); i++) {
		glGenBuffers(1, &objectsVBO[counter]);
		glBindBuffer(GL_ARRAY_BUFFER, objectsVBO[counter]);
		glBufferData(GL_ARRAY_BUFFER, nonTextureObjects[i].mPointCount * sizeof(vec3), &nonTextureObjects[i].mVertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &objectsVBO[counter + 1]);
		glBindBuffer(GL_ARRAY_BUFFER, objectsVBO[counter + 1]);
		glBufferData(GL_ARRAY_BUFFER, nonTextureObjects[i].mPointCount * sizeof(vec3), &nonTextureObjects[i].mNormals[0], GL_STATIC_DRAW);

		glGenVertexArrays(1, &objectsVAO[i]);
		glBindVertexArray(objectsVAO[i]);

		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, objectsVBO[counter]);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, objectsVBO[counter + 1]);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		counter += 2;
	}
}
#pragma endregion VBO_FUNCTIONS

// ------------ GENERATE MODEL FUNCTION ------------
#pragma region MODELS_FUNCTION
// This function takes each of the meshes that have been defined at the beginning of the program
// and loads them in. This functionality was split into a separate class as it was not needed within
// the main body of the program. An array of the meshes and also of textures are then returned and
// passed to the VBO Functions.
void generateModels() {
	Model bin_data(BIN_MESH, bin);
	Model footpath_data(FOOTPATH_MESH, footpath);
	Model road_data(ROAD_MESH, road);
	Model building1_data(BUILDING1_MESH, building1);
	Model building2_data(BUILDING2_MESH, building2);
	Model building3_data(BUILDING3_MESH, building3);
	Model grass_data(GRASS_MESH, grass);
	Model chest_data(CHEST_MESH, chest);
	Model head_data(HEAD_MESH, skin);
	Model hair_data(HAIR_MESH, hair);
	Model leftLeg_data(LEFT_LEG_MESH, jean);
	Model rightLeg_data(RIGHT_LEG_MESH, jean);
	Model leftFoot_data(LEFT_FOOT_MESH, shoe);
	Model rightFoot_data(RIGHT_FOOT_MESH, shoe);
	Model bottle_data(BOTTLE_MESH, bottle);
	Model bag_data(BAG_MESH, rubbishBag);
	Model car_data(CAR_MESH);
	Model wheel_data(WHEEL_MESH);

	meshData = model.getDataArray();
	textures = model.getTextureArray();
	nonTextureModels = model.getNonTextureObjects();

	generateObjectBufferMesh(meshData, textures);
	generateNonTextureObjects(nonTextureModels);
}
#pragma endregion MODELS_FUNCTION

// ------------ GENERATE SKYBOX FUNCTION ------------
#pragma region SKYBOX_FUNCTION
// This function creates the VAO and VBO needed for generating the skybox.
void generateSkybox() {
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}
#pragma endregion SKYBOX_FUNCTION

// ------------ DISPLAY ------------
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
	lastCameraPos = cameraPosition;
	lastCameraFront = cameraFront;
	lastCameraUp = cameraUp;
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(lastCameraPos, lastCameraPos + lastCameraFront, lastCameraUp);
	glUniformMatrix4fv(glGetUniformLocation(textureShaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));

	// ------------------------------------- SKYBOX ------------------------------------- (skybox Shader)
	glDepthFunc(GL_LEQUAL);
	glUseProgram(skyboxShaderProgramID);
	
	if (!keyO) {
		view = glm::lookAt(lastCameraPos, lastCameraPos + lastCameraFront, lastCameraUp);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	}
	if (keyO) {
		view = glm::lookAt(overheadPos, overheadPos + overheadCameraFront, overheadUp);
		view = glm::rotate(view, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	}
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
	glBindVertexArray(objectsVAO[0]);

	float carSpeed = 100.0f * delta;
	glm::vec3 objectColor = glm::vec3(0.0f, 0.5f, 0.5f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(glGetUniformLocation(objectShaderProgramID, "objectColor"), 1, &objectColor[0]);
	glUniform3fv(glGetUniformLocation(objectShaderProgramID, "lightColor"), 1, &lightColor[0]);
	glUniform3fv(glGetUniformLocation(objectShaderProgramID, "lightPos"), 1, &lightPos[0]);
	glUniform3fv(glGetUniformLocation(objectShaderProgramID, "viewPos"), 1, &cameraPosition[0]);

	glUniformMatrix4fv(glGetUniformLocation(objectShaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(objectShaderProgramID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

	// Car 1
	glm::mat4 car1Model = glm::mat4(1.0f);
	car1Model = glm::translate(car1Model, carPos1);
	carPos1 += carSpeed * startingCameraFront;
	matrix_location = glGetUniformLocation(objectShaderProgramID, "model");
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(car1Model));

	if (carPos1.z > -370) {
		glDrawArrays(GL_TRIANGLES, 0, nonTextureModels[0].mPointCount);
	}
	else {
		drawCar1 = false;
	}

	// Car 2
	objectColor = glm::vec3(0.5f, 0.0f, 0.1f);
	glUniform3fv(glGetUniformLocation(objectShaderProgramID, "objectColor"), 1, &objectColor[0]);
	glm::mat4 car2Model = glm::mat4(1.0f);
	car2Model = glm::translate(car2Model, carPos2);
	car2Model = glm::rotate(car2Model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	carPos2 -= carSpeed * cameraFront;
	matrix_location = glGetUniformLocation(objectShaderProgramID, "model");
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(car2Model));

	if (carPos2.z < 250) {
		glDrawArrays(GL_TRIANGLES, 0, nonTextureModels[0].mPointCount);
	}
	else {
		drawCar2 = false;
	}

	// ------------------------------------- WHEELS ------------------------------------- (object Shader)
	glBindVertexArray(objectsVAO[1]);

	// Car 1
	if (drawCar1) {
		objectColor = glm::vec3(0.2f, 0.2f, 0.2f);
		glUniform3fv(glGetUniformLocation(objectShaderProgramID, "objectColor"), 1, &objectColor[0]);

		// Front-right
		glm::mat4 wheelModel = glm::mat4(1.0f);
		wheelModel = glm::translate(wheelModel, glm::vec3(10.0f, -4.0f, -16.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
		wheelModel = car1Model * wheelModel;
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(wheelModel));
		glDrawArrays(GL_TRIANGLES, 0, nonTextureModels[1].mPointCount);

		// Front-left
		wheelModel = glm::mat4(1.0f);
		wheelModel = glm::translate(wheelModel, glm::vec3(-10.0f, -4.0f, -16.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
		wheelModel = car1Model * wheelModel;
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(wheelModel));
		glDrawArrays(GL_TRIANGLES, 0, nonTextureModels[1].mPointCount);

		// Back-right 
		wheelModel = glm::mat4(1.0f);
		wheelModel = glm::translate(wheelModel, glm::vec3(10.0f, -4.0f, 16.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
		wheelModel = car1Model * wheelModel;
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(wheelModel));
		glDrawArrays(GL_TRIANGLES, 0, nonTextureModels[1].mPointCount);

		// Back-left
		wheelModel = glm::mat4(1.0f);
		wheelModel = glm::translate(wheelModel, glm::vec3(-10.0f, -4.0f, 16.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
		wheelModel = car1Model * wheelModel;
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(wheelModel));
		glDrawArrays(GL_TRIANGLES, 0, nonTextureModels[1].mPointCount);
	}

	// Car 2
	if (drawCar2) {
		objectColor = glm::vec3(0.2f, 0.2f, 0.2f);
		glUniform3fv(glGetUniformLocation(objectShaderProgramID, "objectColor"), 1, &objectColor[0]);

		// Front-right
		glm::mat4 wheelModel = glm::mat4(1.0f);
		wheelModel = glm::translate(wheelModel, glm::vec3(10.0f, -4.0f, -16.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
		wheelModel = car2Model * wheelModel;
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(wheelModel));
		glDrawArrays(GL_TRIANGLES, 0, nonTextureModels[1].mPointCount);

		// Front-left
		wheelModel = glm::mat4(1.0f);
		wheelModel = glm::translate(wheelModel, glm::vec3(-10.0f, -4.0f, -16.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
		wheelModel = car2Model * wheelModel;
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(wheelModel));
		glDrawArrays(GL_TRIANGLES, 0, nonTextureModels[1].mPointCount);

		// Back-right 
		wheelModel = glm::mat4(1.0f);
		wheelModel = glm::translate(wheelModel, glm::vec3(10.0f, -4.0f, 16.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
		wheelModel = car2Model * wheelModel;
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(wheelModel));
		glDrawArrays(GL_TRIANGLES, 0, nonTextureModels[1].mPointCount);

		// Back-left
		wheelModel = glm::mat4(1.0f);
		wheelModel = glm::translate(wheelModel, glm::vec3(-10.0f, -4.0f, 16.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wheelModel = glm::rotate(wheelModel, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
		wheelModel = car2Model * wheelModel;
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(wheelModel));
		glDrawArrays(GL_TRIANGLES, 0, nonTextureModels[1].mPointCount);
	}
	
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

	// Deposit Objects Into Bin
	if (glm::distance(bin1Pos.x, fredPos.x) < xDistance & glm::distance(bin1Pos.z, fredPos.z) < zDistance || 
		glm::distance(bin2Pos.x, fredPos.x) < xDistance & glm::distance(bin2Pos.z, fredPos.z) < zDistance || 
		glm::distance(bin3Pos.x, fredPos.x) < xDistance & glm::distance(bin3Pos.z, fredPos.z) < zDistance || 
		glm::distance(bin4Pos.x, fredPos.x) < xDistance & glm::distance(bin4Pos.z, fredPos.z) < zDistance) {
		pressM = true;
	}
	else {
		pressM = false;
	}

	// Bin 1
	glm::mat4 binModel = glm::mat4(1.0f);
	binModel = glm::translate(binModel, bin1Pos);
	matrix_location = glGetUniformLocation(textureShaderProgramID, "model");
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(binModel));
	glDrawArrays(GL_TRIANGLES, 0, meshData[0].mPointCount);

	// Bin 2
	binModel = glm::mat4(1.0f);
	binModel = glm::translate(binModel, bin2Pos);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(binModel));
	glDrawArrays(GL_TRIANGLES, 0, meshData[0].mPointCount);

	// Bin 3
	binModel = glm::mat4(1.0f);
	binModel = glm::translate(binModel, bin3Pos);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(binModel));
	glDrawArrays(GL_TRIANGLES, 0, meshData[0].mPointCount);

	// Bin 4
	binModel = glm::mat4(1.0f);
	binModel = glm::translate(binModel, bin4Pos);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(binModel));
	glDrawArrays(GL_TRIANGLES, 0, meshData[0].mPointCount);

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
	glDrawArrays(GL_TRIANGLES, 0, meshData[1].mPointCount);

	// Footpath 2
	footpathModel = glm::mat4(1.0f);
	footpathModel = glm::translate(footpathModel, glm::vec3(-150.0f, 2.0f, 0.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(footpathModel));
	glDrawArrays(GL_TRIANGLES, 0, meshData[1].mPointCount);

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
	glDrawArrays(GL_TRIANGLES, 0, meshData[2].mPointCount);

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
	glDrawArrays(GL_TRIANGLES, 0, meshData[3].mPointCount);

	// Building 1 - 2
	building1Model = glm::mat4(1.0f);
	building1Model = glm::translate(building1Model, glm::vec3(-185.0f, 0.0f, -98.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(building1Model));
	glDrawArrays(GL_TRIANGLES, 0, meshData[3].mPointCount);

	// Building 1 - 3
	building1Model = glm::mat4(1.0f);
	building1Model = glm::translate(building1Model, glm::vec3(-185.0f, 0.0f, -152.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(building1Model));
	glDrawArrays(GL_TRIANGLES, 0, meshData[3].mPointCount);

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
	glDrawArrays(GL_TRIANGLES, 0, meshData[4].mPointCount);

	// Building 2 - 2
	building2Model = glm::mat4(1.0f);
	building2Model = glm::translate(building2Model, glm::vec3(-185.0f, 0.0f, -211.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(building2Model));
	glDrawArrays(GL_TRIANGLES, 0, meshData[4].mPointCount);

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
	glDrawArrays(GL_TRIANGLES, 0, meshData[5].mPointCount);

	// Building 3 - 2
	building3Model = glm::mat4(1.0f);
	building3Model = glm::translate(building3Model, glm::vec3(-185.0f, 0.0f, 218.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(building3Model));
	glDrawArrays(GL_TRIANGLES, 0, meshData[5].mPointCount);

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
	glDrawArrays(GL_TRIANGLES, 0, meshData[6].mPointCount);

	// ------------------------------------- CHEST ------------------------------------- (texture Shader)
	//uvScalar
	uvScalar = 10;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[7]);
	glBindVertexArray(VAO[7]);

	// Chest
	glm::mat4 chestModel = glm::mat4(1.0f);
	chestModel = glm::translate(chestModel, fredPos);
	chestModel = glm::rotate(chestModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(chestModel));
	glDrawArrays(GL_TRIANGLES, 0, meshData[7].mPointCount);

	// ------------------------------------- HEAD ------------------------------------- (texture Shader)
	// uvScalar
	uvScalar = 10;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[8]);
	glBindVertexArray(VAO[8]);

	// Head
	glm::mat4 headModel = glm::mat4(1.0f);
	headModel = glm::translate(headModel, glm::vec3(0.0f, 0.0f, 0.0f));
	headModel = glm::rotate(headModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	headModel = chestModel * headModel;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(headModel));
	glDrawArrays(GL_TRIANGLES, 0, meshData[8].mPointCount);

	// ------------------------------------- HAIR ------------------------------------- (texture Shader)
	// uvScalar
	uvScalar = 10;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[9]);
	glBindVertexArray(VAO[9]);

	// Hair
	glm::mat4 hairModel = glm::mat4(1.0f);
	hairModel = glm::translate(hairModel, glm::vec3(0.0f, 0.0f, 0.0f));
	hairModel = glm::rotate(hairModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	hairModel = headModel * hairModel;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(hairModel));
	glDrawArrays(GL_TRIANGLES, 0, meshData[9].mPointCount);

	// ------------------------------------- LEFT LEG ------------------------------------- (texture Shader)
	//uvScalar
	uvScalar = 10;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[10]);
	glBindVertexArray(VAO[10]);

	// Left Leg
	glm::mat4 leftLegModel = glm::mat4(1.0f);
	leftLegModel = glm::translate(leftLegModel, glm::vec3(0.0f, -6.0f, 0.0f));
	leftLegModel = glm::rotate(leftLegModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	leftLegModel = chestModel * leftLegModel;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(leftLegModel));
	glDrawArrays(GL_TRIANGLES, 0, meshData[10].mPointCount);

	// ------------------------------------- RIGHT LEG ------------------------------------- (texture Shader)
	//uvScalar
	uvScalar = 10;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[11]);
	glBindVertexArray(VAO[11]);

	// Right Leg
	glm::mat4 rightLegModel = glm::mat4(1.0f);
	rightLegModel = glm::translate(rightLegModel, glm::vec3(0.0f, -6.0f, 0.0f));
	rightLegModel = chestModel * rightLegModel;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(rightLegModel));
	glDrawArrays(GL_TRIANGLES, 0, meshData[11].mPointCount);

	// ------------------------------------- LEFT FOOT ------------------------------------- (texture Shader)
	//uvScalar
	uvScalar = 10;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[12]);
	glBindVertexArray(VAO[12]);

	// Left Foot
	glm::mat4 leftFootModel = glm::mat4(1.0f);
	leftFootModel = glm::translate(leftFootModel, glm::vec3(-0.2f, -0.2f, 0.0f));
	leftFootModel = glm::rotate(leftFootModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	leftFootModel = leftLegModel * leftFootModel;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(leftFootModel));
	glDrawArrays(GL_TRIANGLES, 0, meshData[12].mPointCount);

	// ------------------------------------- RIGHT FOOT ------------------------------------- (texture Shader)
	//uvScalar
	uvScalar = 10;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[13]);
	glBindVertexArray(VAO[13]);

	// Right Foot
	glm::mat4 rightFootModel = glm::mat4(1.0f);
	rightFootModel = glm::translate(rightFootModel, glm::vec3(-1.25f, -0.2f, 0.0f));
	rightFootModel = rightLegModel * rightFootModel;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(rightFootModel));
	glDrawArrays(GL_TRIANGLES, 0, meshData[13].mPointCount);

	// ------------------------------------- BOTTLES ------------------------------------- (texture Shader)
	// uvScalar
	uvScalar = 20;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[14]);
	glBindVertexArray(VAO[14]);

	// Bottles
	glm::mat4 bottleModel = glm::mat4(1.0f);
	// Bottle 1
	if (bottle1) {
		bottleModel = glm::translate(bottleModel, bottle1Pos);
		bottleModel = glm::rotate(bottleModel, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[14].mPointCount);
	}

	// Bottle2
	if (bottle2) {
		bottleModel = glm::mat4(1.0f);
		bottleModel = glm::translate(bottleModel, bottle2Pos);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[14].mPointCount);
	}

	// Bottle3
	if (bottle3) {
		bottleModel = glm::mat4(1.0f);
		bottleModel = glm::translate(bottleModel, bottle3Pos);
		bottleModel = glm::rotate(bottleModel, glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[14].mPointCount);
	}

	// Bottle4
	if (bottle4) {
		bottleModel = glm::mat4(1.0f);
		bottleModel = glm::translate(bottleModel, bottle4Pos);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[14].mPointCount);
	}

	// Bottle5
	if (bottle5) {
		bottleModel = glm::mat4(1.0f);
		bottleModel = glm::translate(bottleModel, bottle5Pos);
		bottleModel = glm::rotate(bottleModel, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[14].mPointCount);
	}

	// Bottle6
	if (bottle6) {
		bottleModel = glm::mat4(1.0f);
		bottleModel = glm::translate(bottleModel, bottle6Pos);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[14].mPointCount);
	}

	// Bottle7
	if (bottle7) {
		bottleModel = glm::mat4(1.0f);
		bottleModel = glm::translate(bottleModel, bottle7Pos);
		bottleModel = glm::rotate(bottleModel, glm::radians(-76.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[14].mPointCount);
	}

	// Bottle8
	if (bottle8) {
		bottleModel = glm::mat4(1.0f);
		bottleModel = glm::translate(bottleModel, bottle8Pos);
		bottleModel = glm::rotate(bottleModel, glm::radians(32.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[14].mPointCount);
	}

	// Bottle9
	if (bottle9) {
		bottleModel = glm::mat4(1.0f);
		bottleModel = glm::translate(bottleModel, bottle9Pos);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[14].mPointCount);
	}

	// Bottle10
	if (bottle10) {
		bottleModel = glm::mat4(1.0f);
		bottleModel = glm::translate(bottleModel, bottle10Pos);
		bottleModel = glm::rotate(bottleModel, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bottleModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[14].mPointCount);
	}

	// Pick up Bottles
	// Bottle 1
	if (bottle1 && glm::distance(bottle1Pos.x, fredPos.x) < xDistance & glm::distance(bottle1Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bottleInventory++;
			bottle1 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bottle 2
	if (bottle2 && glm::distance(bottle2Pos.x, fredPos.x) < xDistance & glm::distance(bottle2Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bottleInventory++;
			bottle2 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bottle 3
	if (bottle3 && glm::distance(bottle3Pos.x, fredPos.x) < xDistance & glm::distance(bottle3Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bottleInventory++;
			bottle3 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bottle 4
	if (bottle4 && glm::distance(bottle4Pos.x, fredPos.x) < xDistance & glm::distance(bottle4Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bottleInventory++;
			bottle4 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bottle 5
	if (bottle5 && glm::distance(bottle5Pos.x, fredPos.x) < xDistance & glm::distance(bottle5Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bottleInventory++;
			bottle5 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bottle 6
	if (bottle6 && glm::distance(bottle6Pos.x, fredPos.x) < xDistance & glm::distance(bottle6Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bottleInventory++;
			bottle6 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bottle 7
	if (bottle7 && glm::distance(bottle7Pos.x, fredPos.x) < xDistance & glm::distance(bottle7Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bottleInventory++;
			bottle7 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bottle 8
	if (bottle8 && glm::distance(bottle8Pos.x, fredPos.x) < xDistance & glm::distance(bottle8Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bottleInventory++;
			bottle8 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bottle 9
	if (bottle9 && glm::distance(bottle9Pos.x, fredPos.x) < xDistance & glm::distance(bottle9Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bottleInventory++;
			bottle9 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bottle 10
	if (bottle10 && glm::distance(bottle10Pos.x, fredPos.x) < xDistance & glm::distance(bottle10Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bottleInventory++;
			bottle10 = false;
			pressP = false;
			keyP = false;
		}
	}

	// ------------------------------------- RUBBISH BAG ------------------------------------- (texture Shader)
	// uvScalar
	uvScalar = 1;
	glUniform1f(glGetUniformLocation(textureShaderProgramID, "uvScalar"), uvScalar);

	// Texture & VAO
	glBindTexture(GL_TEXTURE_2D, VTO[15]);
	glBindVertexArray(VAO[15]);

	// Bags
	glm::mat4 bagModel = glm::mat4(1.0f);
	// Bag 1
	if (bag1) {
		bagModel = glm::translate(bagModel, bag1Pos);
		bagModel = glm::rotate(bagModel, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix_location = glGetUniformLocation(objectShaderProgramID, "model");
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bagModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[15].mPointCount);
	}

	// Bag 2
	if (bag2) {
		bagModel = glm::mat4(1.0f);
		bagModel = glm::translate(bagModel, bag2Pos);
		bagModel = glm::rotate(bagModel, glm::radians(-90.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		matrix_location = glGetUniformLocation(objectShaderProgramID, "model");
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bagModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[15].mPointCount);
	}

	// Bag 3
	if (bag3) {
		bagModel = glm::mat4(1.0f);
		bagModel = glm::translate(bagModel, bag3Pos);
		bagModel = glm::rotate(bagModel, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix_location = glGetUniformLocation(objectShaderProgramID, "model");
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bagModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[15].mPointCount);
	}

	// Bag 4
	if (bag4) {
		bagModel = glm::mat4(1.0f);
		bagModel = glm::translate(bagModel, bag4Pos);
		bagModel = glm::rotate(bagModel, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix_location = glGetUniformLocation(objectShaderProgramID, "model");
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bagModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[15].mPointCount);
	}

	// Bag 5
	if (bag5) {
		bagModel = glm::mat4(1.0f);
		bagModel = glm::translate(bagModel, bag5Pos);
		bagModel = glm::rotate(bagModel, glm::radians(-180.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		matrix_location = glGetUniformLocation(objectShaderProgramID, "model");
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(bagModel));
		glDrawArrays(GL_TRIANGLES, 0, meshData[15].mPointCount);
	}

	// Pick up Bags
	// Bag 1
	if (bag1 && glm::distance(bag1Pos.x, fredPos.x) < xDistance & glm::distance(bag1Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bagInventory++;
			bag1 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bag 2
	if (bag2 && glm::distance(bag2Pos.x, fredPos.x) < xDistance & glm::distance(bag2Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bagInventory++;
			bag2 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bag 3
	if (bag3 && glm::distance(bag3Pos.x, fredPos.x) < xDistance & glm::distance(bag3Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bagInventory++;
			bag3 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bag 4
	if (bag4 && glm::distance(bag4Pos.x, fredPos.x) < xDistance & glm::distance(bag4Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bagInventory++;
			bag4 = false;
			pressP = false;
			keyP = false;
		}
	}
	// Bag 5
	if (bag5 && glm::distance(bag5Pos.x, fredPos.x) < xDistance & glm::distance(bag5Pos.z, fredPos.z) < zDistance) {
		pressP = true;
		if (keyP) {
			bagInventory++;
			bag5 = false;
			pressP = false;
			keyP = false;
		}
	}

	// ------------------------------------- End Game ------------------------------------- 
	if (fredPos.z <= -210) {
		std::string score = "Final Score:\n " + std::to_string(playerScore);
		LPCSTR sw = score.c_str();
		int messageBoxID = MessageBox(NULL, sw, "Game Over", MB_OK);
		switch (messageBoxID) {
		case IDOK:
			exit(0);
		}
	}

	// ---------------------------------------------------------------------------------
	glutSwapBuffers();

}

// ------------ UPDATE SCENE (UNCHANGED) ------------
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

// ------------ INITIATE SCENE AND SET UP OBJECTS ------------
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
}

// ------------ KEYBOARD AND MOUSE FUNCTIONALITY ------------
#pragma region KEYBOARD_FUNCTIONS
// This function manages all of the keyboard input. If a key with a function is pressed,
// it will be handled here.
void keypress(unsigned char key, int x, int y) {
	float cameraSpeed = 300.0f * delta;
	int messageBoxID;
	std::string s = "Bottles: " + std::to_string(bottleInventory) + "\nBags: " + std::to_string(bagInventory);
	LPCSTR sw = s.c_str();
	std::string score = "Score: " + std::to_string(playerScore);
	LPCSTR scores = score.c_str();
	switch (key) {
	// Move Camera Left
	case 'a':
		if (cameraPosition.x > -140 && !keyO) {
			cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			fredPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		break;
	// Show Score
	case 'b':
		messageBoxID = MessageBox(NULL, scores, "Score", MB_OK);
		switch (messageBoxID) {
		case IDOK:
			break;
		}
		break;
	// Use camera to move around the screen
	case 'c':
		if (!keyC && !keyO) {
			keyC = true;
		}
		else if (keyC) {
			keyC = false;
		}
		break;
	// Move Camera Right
	case 'd':
		if (cameraPosition.x < 20 && !keyO) {
			cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			fredPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		break;
	// Inventory
	case 'i':
		messageBoxID = MessageBox(NULL, sw, "Inventory", MB_OK);
		switch (messageBoxID) {
		case IDOK:
			break;
		}
		break;
	// Move Camera Down
	case 'j':
		if (cameraPosition.y >= 10 && !keyO) {
			cameraPosition -= cameraSpeed * up;
		}
		break;
	// Deposit Inventory Items into Bin
	case 'm':
		if (pressM && (bottleInventory != 0) & (bagInventory != 0)) {
			keyM = true;
			playerScore += bottleInventory;
			playerScore += bagInventory * 3;
			bottleInventory = 0;
			bagInventory = 0;
		}
		else if (!pressM) {
			keyM = false;
		}
		break;
	// Overhead View
	case 'o':
		if (!keyO) {
			keyO = true;
		}
		else if (keyO) {
			keyO = false;
		}
		break;
	// Pick up item
	case 'p':
		if (pressP) {
			keyP = true;
		}
		else if (!pressP) {
			keyP = false;
		}
		break;
	// Reset camera to Fred's position
	case 'r':
		glm::vec3 newFredPos = glm::vec3(fredPos.x, fredPos.y, fredPos.z - 2);
		cameraPosition = newFredPos;
		break;
	// Move Camera Backwards
	case 's':
		if (cameraPosition.z < 220 && !keyO) {
			cameraPosition -= cameraSpeed * cameraFront;
			fredPos -= cameraSpeed * cameraFront;
		}
		break;
	// Move Camera Up
	case 'u':
		if (cameraPosition.y < 30 && !keyO) {
			cameraPosition += cameraSpeed * up;
		}
		break;
	// Move Camera Forwards
	case 'w':
		if (cameraPosition.z > -300 && !keyO) {
			cameraPosition += cameraSpeed * cameraFront;
			fredPos += cameraSpeed * cameraFront;
		}
		break;
	// For button 'x' on the keyboard, the player will have the option to quit the game.
	case 'x':
		messageBoxID = MessageBox(NULL, "Do you want to quit the game?", "Menu", MB_YESNO | MB_ICONQUESTION);

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
// When the pan camera view is on (key C is pressed), this function allows the camera to move around the screen.
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
// This function handles the scrollwheel on the mouse, if the player scrolls foward, the camera
// will 'zoom in' and if they scroll backwards, the camera will 'zoom out'.
void zoomCameraMovement(int button, int direction, int x, int y) {
	float cameraSpeed = 10.0f;
	if (cameraPosition.z > -300 && !keyO) {
		if (cameraPosition.y > 5) {
			// If the wheel direction is positive, zoom in
			if (direction > 0) {
				cameraPosition += cameraSpeed * cameraFront;
			}
		}
	}
	if (cameraPosition.z < 220 && !keyO) {
		if (cameraPosition.y + 2> 5) {
			// If the wheel direction is negative, zoom out
			if (direction < 0) {
				cameraPosition -= cameraSpeed * cameraFront;
			}
		}
	}
}
#pragma endregion KEYBOARD_FUNCTIONS

// ------------ MAIN FUNCTION/SETUP ------------
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("EcoGame");
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
