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

#include "GenerateObject.h"

/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define BIN_MESH "./models/rubbishBinCircular.dae"
#define FOOTPATH_MESH "./models/footpath.dae"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

const char *bin = "./textures/bin_texture.jpg";
const char *footpath = "./textures/footpath_texture.jpg";

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
glm::vec3 cameraPosition = glm::vec3(0.0f, 20.0f, 100.0f);
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
GLuint objectShaderProgramID, skyboxShaderProgramID;

// ------------ MESH SETUP ------------
ModelData bin_data, footpath_data;

GLuint loc1, loc2, loc3;
GLfloat rotate_y = 0.0f;

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
	-200.0f,  200.0f, -200.0f,
	-200.0f, -200.0f, -200.0f,
	 200.0f, -200.0f, -200.0f,
	 200.0f, -200.0f, -200.0f,
	 200.0f,  200.0f, -200.0f,
	-200.0f,  200.0f, -200.0f,

	-200.0f, -200.0f,  200.0f,
	-200.0f, -200.0f, -200.0f,
	-200.0f,  200.0f, -200.0f,
	-200.0f,  200.0f, -200.0f,
	-200.0f,  200.0f,  200.0f,
	-200.0f, -200.0f,  200.0f,

	 200.0f, -200.0f, -200.0f,
	 200.0f, -200.0f,  200.0f,
	 200.0f,  200.0f,  200.0f,
	 200.0f,  200.0f,  200.0f,
	 200.0f,  200.0f, -200.0f,
	 200.0f, -200.0f, -200.0f,

	-200.0f, -200.0f,  200.0f,
	-200.0f,  200.0f,  200.0f,
	 200.0f,  200.0f,  200.0f,
	 200.0f,  200.0f,  200.0f,
	 200.0f, -200.0f,  200.0f,
	-200.0f, -200.0f,  200.0f,

	-200.0f,  200.0f, -200.0f,
	 200.0f,  200.0f, -200.0f,
	 200.0f,  200.0f,  200.0f,
	 200.0f,  200.0f,  200.0f,
	-200.0f,  200.0f,  200.0f,
	-200.0f,  200.0f, -200.0f,

	-200.0f, -200.0f, -200.0f,
	-200.0f, -200.0f,  200.0f,
	 200.0f, -200.0f, -200.0f,
	 200.0f, -200.0f, -200.0f,
	-200.0f, -200.0f,  200.0f,
	 200.0f, -200.0f,  200.0f
};

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


void generateBufferObjects() {
	GenerateObject generateObject;
	bin_data = load_mesh(BIN_MESH);
	generateObject.generateObjectBufferMesh(objectShaderProgramID, bin_data, bin);
	footpath_data = load_mesh(FOOTPATH_MESH);
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

void display() {

	//glEnable(GL_DEPTH_TEST); // enable depth-testing
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glActiveTexture(GL_TEXTURE0);

	//// tell GL to only draw onto a pixel if the shape is closer to the viewer
	//glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//glm::mat4 model = glm::mat4(1.0f);

	//// ------------------------------------- PROJECTION -------------------------------------
	//glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
	//int proj_mat_location = glGetUniformLocation(objectShaderProgramID, "proj");
	//glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, glm::value_ptr(proj));

	//// ------------------------------------- CAMERA -------------------------------------

	//glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

	////std::cout << glm::to_string(cameraUp) << std::endl;

	//int view_mat_location = glGetUniformLocation(objectShaderProgramID, "view");
	//glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, glm::value_ptr(view));

	//// ------------------------------------- SKYBOX -------------------------------------

	//glDepthFunc(GL_LEQUAL);
	//glUseProgram(skyboxShaderProgramID);
	//view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

	//view_mat_location = glGetUniformLocation(skyboxShaderProgramID, "view");
	//glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, glm::value_ptr(view));

	//proj_mat_location = glGetUniformLocation(skyboxShaderProgramID, "proj");
	//glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, glm::value_ptr(proj));

	//glBindVertexArray(skyboxVAO);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDepthFunc(GL_LESS);
	//glDepthMask(GL_TRUE);

	//// ------------------------------------- BIN MODEL USING GLM -------------------------------------

	//glUseProgram(objectShaderProgramID);
	//glBindTexture(GL_TEXTURE_2D, VTO[0]);
	//glBindVertexArray(VAO[0]);
	//model = glm::mat4(1.0f);

	//model = glm::translate(model, glm::vec3(10.0, 50.0, 0.0f));
	////model = glm::rotate(model, glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));

	//int matrix_location = glGetUniformLocation(objectShaderProgramID, "model");
	//glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(model));

	//glDrawArrays(GL_TRIANGLES, 0, bin_data.mPointCount);

	//// ------------------------------------- FOOTPATH MODEL USING GLM -------------------------------------

	//glBindTexture(GL_TEXTURE_2D, VTO[1]);
	//glBindVertexArray(VAO[1]);
	//model = glm::mat4(1.0f);

	//model = glm::translate(model, glm::vec3(0.0, 20.0, 70.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//matrix_location = glGetUniformLocation(objectShaderProgramID, "model");
	//glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(model));

	//glDrawArrays(GL_TRIANGLES, 0, footpath_data.mPointCount);


	//// ------------------------------------- NON-GLM MODEL -------------------------------------
	////int matrix_location = glGetUniformLocation(binShaderProgramID, "model");

	////mat4 model = identity_mat4();
	////model = rotate_y_deg(model, rotate_y);

	////int matrix_location = glGetUniformLocation(binShaderProgramID, "model");
	////glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);

	//////glBindVertexArray(VAO[0]);
	////glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);
	//
	//glutSwapBuffers();

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
	objectShaderProgramID = CompileShaders("./shaders/objectVertexShader.txt", "./shaders/objectFragmentShader.txt");
	skyboxShaderProgramID = CompileShaders("./shaders/skyboxVertexShader.txt", "./shaders/skyboxFragmentShader.txt");
	// generate Skybox VBO and VAO
	generateSkybox();
	cubemapTexture = loadCubemap(faces);
	// load mesh into a vertex buffer array
	generateBufferObjects();
	//generateObjectBufferMesh();
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {
	float cameraSpeed = 100.0f * delta;
	switch (key) {
		// Move Camera Left
	case 'a':
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
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
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
		// Move Camera Down
	case 'j':
		cameraPosition -= cameraSpeed * up;
		break;
		// Move Camera Backwards
	case 's':
		cameraPosition -= cameraSpeed * cameraFront;
		break;
		// Move Camera Up
	case 'u':
		cameraPosition += cameraSpeed * up;
		break;
		// Move Camera Forwards
	case 'w':
		cameraPosition += cameraSpeed * cameraFront;
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
	if (cameraPosition.z > -100) {
		// If the wheel direction is positive, zoom in
		if (direction > 0) {
			cameraPosition += cameraSpeed * cameraFront;
		}
	}
	// Can't zoom too far
	if (cameraPosition.z < 200) {
		// If the wheel direction is negative, zoom out
		if (direction < 0) {
			cameraPosition -= cameraSpeed * cameraFront;
		}
	}
	
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Eco Game");

	// Generate Objects Class
	GenerateObject generateObject;

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
