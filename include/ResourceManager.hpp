#pragma once

#include <iostream>

static class ResourceManager
{
public:
	static unsigned int loadTextureFromFile(const std::string& path);
	static std::string loadShaderSourceFromFile(const std::string& path, std::stringstream& errors);
};

