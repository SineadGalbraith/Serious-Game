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

GLuint* GenerateObject::getVAO() {
	return VAO;
}

GLuint* GenerateObject::getVBO() {
	return VBO;
}

GLuint* GenerateObject::getVTO() {
	return VTO;
}

GenerateObject::ModelData load_mesh(const char* file_name) {
	GenerateObject::ModelData modelData;

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

void GenerateObject::generateObjectBufferMesh(GLuint shaderProgramID, const char *image_texture) {
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.

	loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data;

	glGenTextures(1, &VTO[counter]);
	glBindTexture(GL_TEXTURE_2D, VTO[counter]);

	data = stbi_load(image_texture, &width, &height, &nrChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	glGenBuffers(1, &VBO[vbo_counter]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[vbo_counter]);
	glBufferData(GL_ARRAY_BUFFER, objectData.mPointCount * sizeof(vec3), &objectData.mVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[vbo_counter+1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[vbo_counter+1]);
	glBufferData(GL_ARRAY_BUFFER, objectData.mPointCount * sizeof(vec3), &objectData.mNormals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[vbo_counter+2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[vbo_counter+2]);
	glBufferData(GL_ARRAY_BUFFER, objectData.mPointCount * sizeof(vec2), &objectData.mTextureCoords[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO[counter]);
	glBindVertexArray(VAO[counter]);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[vbo_counter]);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[vbo_counter+1]);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(loc3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[vbo_counter+2]);
	glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	counter++;
	vbo_counter = vbo_counter + 3;

	std::cout << counter << std::endl;
	std::cout << vbo_counter << std::endl;
}