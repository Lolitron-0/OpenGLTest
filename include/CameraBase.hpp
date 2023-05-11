#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovementDirection {
	Forward,
	Backward,
	Right,
	Left,
	Up,
	Down,
};

class CameraBase
{
public:

	CameraBase(glm::vec3 position, glm::vec3 worldUp, float pitch, float yaw);
	glm::mat4 getViewMatrix();

	glm::vec3 getPosition();

	virtual void processMouseMove(float xOffset, float yOffset, float timeScale) =0;
	virtual void processKeyboard(CameraMovementDirection direction, float timeScale) =0;

protected:

	void updateCamera();

	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;
	float mYaw;
	float mPitch;
	float mMouseSensitivity;
};

