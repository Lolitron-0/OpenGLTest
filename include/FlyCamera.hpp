#pragma once
#include <glm/glm.hpp>
#include "CameraBase.hpp"

class FlyCamera : public CameraBase
{
public:

	FlyCamera(glm::vec3 position, glm::vec3 worldUp);


	// Унаследовано через CameraBase
	virtual void processMouseMove(float xOffset, float yOffset, float timeScale) override;

	virtual void processKeyboard(CameraMovementDirection direction, float timeScale) override;

private:
	float mMoveSpeed;
};
