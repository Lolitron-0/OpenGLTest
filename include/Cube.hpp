#pragma once
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include "include/Shader.hpp"


class Cube
{
public:
	Cube(std::string pathToTexture);
	
	void translate(glm::vec3 position);
	void rotate(float degrees, glm::vec3 axis);
	void scale(glm::vec3 values);
	void scale(float value);

	void setShader(std::shared_ptr<Shader>& shader);

private:
	unsigned int mVBO;
	unsigned int mVAO;
	unsigned int mEBO;
	glm::mat4 mTransform;
	std::shared_ptr<Shader> mShader;
};

