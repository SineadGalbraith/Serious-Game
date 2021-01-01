#pragma once
// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Model.h"
class Car : public Model {
public:
	Car();
	Car(const char* mesh, GLuint shaderProgram);
	Car::GLuint loc1, loc2; 
	Car::GLuint objectShaderProgramID;
	unsigned int carVAO, carVBO1, carVBO2;
	GLuint getCarVAO(); 
	std::vector < ModelData > getCarModel();
private:
	void generateBuffers();
};