#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader(const std::string& vertPath, const std::string& fragPath);

	void use();

	void setInt(const std::string name, int value);
	void setBool(const std::string name, bool value);
	void setFloat(const std::string name, float value);
	void setMat4(const std::string name, const float* value);
	void setMat4(const std::string name, const glm::mat4& mat);
	void setMat3(const std::string name, const float* value);
	void setMat3(const std::string name, const glm::mat3& mat);
	void setVec3(const std::string name, const glm::vec3& vec);
	void setVec3(const std::string name, float x, float y, float z);

	std::string getErrorLog() const;
	bool isSuccessful() const;

	operator bool();

private:
	std::string _loadSourceFromFile(const std::string& path);

	std::stringstream mErrStream;
	unsigned int mHandle;

	bool mSuccess;
};

