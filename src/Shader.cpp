#include "Shader.hpp"
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ResourceManager.hpp>


Shader::Shader(const std::string& vertPath, const std::string& fragPath)
{
	mSuccess = true; //by default
	auto vertexSource = ResourceManager::loadShaderSourceFromFile(vertPath, mErrStream);
	auto fragSource = ResourceManager::loadShaderSourceFromFile(fragPath, mErrStream);
	if (mErrStream.rdbuf()->in_avail())
		mSuccess = false;

	auto vertexSourceData = vertexSource.c_str();
	auto fragmentSourceData = fragSource.c_str();

	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSourceData, NULL);
	glCompileShader(vertexShader);
	
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		mSuccess = false;
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		mErrStream << "Error compiling vertex shader!" << std::endl << 
			infoLog<<std::endl;

	}

	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSourceData, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		mSuccess = false;
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		mErrStream << "Error compiling fragment shader!" << std::endl << 
			infoLog<<std::endl;

	}

	mHandle = glCreateProgram();
	glAttachShader(mHandle, vertexShader);
	glAttachShader(mHandle, fragmentShader);
	glLinkProgram(mHandle);

	glGetProgramiv(mHandle, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mHandle, 512, NULL, infoLog);
		mErrStream << "Error linking shader program!" << std::endl <<
			infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::setInt(const std::string name, int value)
{
	glUniform1i(glGetUniformLocation(mHandle, name.c_str()), value);
}

void Shader::setBool(const std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(mHandle, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string name, float value)
{
	glUniform1f(glGetUniformLocation(mHandle, name.c_str()), value);
}

void Shader::setMat4(const std::string name, const float* value)
{
	glUniformMatrix4fv(glGetUniformLocation(mHandle, name.c_str()),1, GL_FALSE, value);
}

void Shader::setMat4(const std::string name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(mHandle, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat3(const std::string name, const float* value)
{
	glUniformMatrix3fv(glGetUniformLocation(mHandle, name.c_str()),1, GL_FALSE, value);
}

void Shader::setMat3(const std::string name, const glm::mat3& mat)
{
	glUniformMatrix3fv(glGetUniformLocation(mHandle, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string name, const glm::vec3& vec)
{
	glUniform3fv(glGetUniformLocation(mHandle, name.c_str()), 1, &vec[0]);
}

void Shader::setVec3(const std::string name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(mHandle, name.c_str()), x, y, z);
}

void Shader::use() {
	glUseProgram(mHandle);
}
 

std::string Shader::getErrorLog() const {
	return mErrStream.str();
}

bool Shader::isSuccessful() const {
	return mSuccess;
}

Shader::operator bool()
{
	return mSuccess;
}

std::string Shader::_loadSourceFromFile(const std::string& path)
{
	std::ifstream stream(path);

	if (!stream) {
		mErrStream << "Error opening "<<path<<" file!" << std::endl;
		mSuccess = false;
	}

	std::string line;
	std::string source;

	while (getline(stream, line))
		source += line + '\n';

	stream.close();
	return source;
}

