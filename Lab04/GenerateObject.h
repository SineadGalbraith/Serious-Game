// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>
class GenerateObject
{
private:
	// ------------ VBO/VAO SETUP ------------
	const int i = 16;
	GLuint* VAO = new GLuint[i];
	GLuint* VBO = new GLuint[i * 3];
	GLuint* VTO = new GLuint[i];
	GLuint loc1, loc2, loc3;
	int counter = 0;
	int vbo_counter = 0;
public:
	typedef struct
	{
		size_t mPointCount = 0;
		std::vector<vec3> mVertices;
		std::vector<vec3> mNormals;
		std::vector<vec2> mTextureCoords;
	} ModelData;
	void generateObjectBufferMesh(GLuint shaderProgramID, const char *image_texture);
	GLuint* getVAO();
	GLuint* getVBO();
	GLuint* getVTO();

	ModelData objectData;
	ModelData load_mesh(const char* file_name);
};
