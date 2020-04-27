#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <glad/glad.h>
#include <iostream>

class TextureLoader
{
public:
	static GLuint loadTexture(const std::string &);
};
#endif // !TEXTURELOADER_H