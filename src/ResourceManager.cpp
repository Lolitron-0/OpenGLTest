#include "ResourceManager.hpp"
#include <stb_image.h>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

unsigned int ResourceManager::loadTextureFromFile(const std::string& path)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int tWidth, tHeight, tNumChannels;
	unsigned char* tData = stbi_load(path.c_str(), &tWidth, &tHeight, &tNumChannels, 0);

	if (tData) {
		glTexImage2D(GL_TEXTURE_2D, 0, tNumChannels == 3 ? GL_RGB : GL_RGBA, tWidth, tHeight, 0, tNumChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, tData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cerr << "Error loading image!" << std::endl;
	stbi_image_free(tData);

	return texture;
}

std::string ResourceManager::loadShaderSourceFromFile(const std::string& path, std::stringstream& errors)
{
	std::ifstream stream(path);

	if (!stream) {
		errors << "Error opening " << path << " file!" << std::endl;
	}

	std::string line;
	std::string source;

	while (getline(stream, line))
		source += line + '\n';

	stream.close();
	return source;
}
