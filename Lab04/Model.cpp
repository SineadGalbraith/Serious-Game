#include "Model.h"

std::vector < ModelData > dataArray;
std::vector < std::string > textureArray;
std::vector < ModelData > nonTextureObjects;

// Default Constructor
Model::Model() {}

// Constructor for models with textures
Model::Model(const char* mesh, const char* texture) {
	modelMesh = load_mesh(mesh);
	dataArray.push_back(modelMesh);
	textureArray.push_back(texture);
}

// Constructor for models without textures
Model::Model(const char* mesh) {
	modelMesh = load_mesh(mesh);
	nonTextureObjects.push_back(modelMesh);
}

// Return the data array (i.e. models that have textures)
std::vector < ModelData > Model::getDataArray() {
	return dataArray;
}

// Return the texture array
std::vector < std::string > Model::getTextureArray() {
	return textureArray;
}

// Return the non-texture objects (i.e. models that do not have textures)
std::vector < ModelData > Model::getNonTextureObjects() {
	return nonTextureObjects;
}

// ------------ MESH LOADING FUNCTION (UNCHANGED) ------------
#pragma region MESH_LOADING
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/
ModelData Model::load_mesh(const char* file_name) {
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
#pragma endregion MESH_LOADING