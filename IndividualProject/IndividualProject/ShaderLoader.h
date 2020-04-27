#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderLoader
{
public:
	unsigned int ID;
	ShaderLoader(const char* vertexPath, const char* fragmentPath);
	void Use();
	void setBool(const std::string &name, bool value);
	void setInt(const std::string &name, int value);
	void setFloat(const std::string &name, float value);
	void setVec2(const std::string &name, const glm::vec2 &value);
	void setVec2(const std::string &name, float x, float y);
	void setVec3(const std::string &name, const glm::vec3 &value);
	void setVec3(const std::string &name, float x, float y, float z);
	void setVec4(const std::string &name, const glm::vec4 &value);
	void setVec4(const std::string &name, float x, float y, float z, float w);
	void setMat2(const std::string &name, const glm::mat2 &mat);
	void setMat3(const std::string &name, const glm::mat3 &mat);
	void setMat4(const std::string &name, const glm::mat4 &mat);
};
#endif // !SHADER_LOADER_H