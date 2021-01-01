#include "Car.h"

std::vector < ModelData > carModel;
Car::Car() {

}

Car::Car(const char* mesh, GLuint shaderProgram) : Model(mesh){
	carModel.push_back(modelMesh);
}

void Car::generateBuffers() {
	loc1 = glGetAttribLocation(objectShaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(objectShaderProgramID, "vertex_normal");

	glGenVertexArrays(1, &carVAO);
	glGenBuffers(1, &carVBO1);

	glBindBuffer(GL_ARRAY_BUFFER, carVBO1);
	glBufferData(GL_ARRAY_BUFFER, Model::modelMesh.mPointCount * sizeof(vec3), &Model::modelMesh.mVertices[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &carVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, carVBO2);
	glBufferData(GL_ARRAY_BUFFER, Model::modelMesh.mPointCount * sizeof(vec3), &Model::modelMesh.mNormals[0], GL_STATIC_DRAW);
	
	glBindVertexArray(carVAO);
	
	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, carVBO1);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, carVBO2);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

GLuint Car::getCarVAO() {
	return carVAO;
}

std::vector < ModelData > Car::getCarModel() {
	return carModel;
}