#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "TextureLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

class Model
{
public:
	Model(const std::string &path);
	void Draw(ShaderLoader shader);
	void attachTexture(GLuint textureID, std::string type = "texture_diffuse");
private:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	void loadModel(const std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
#endif