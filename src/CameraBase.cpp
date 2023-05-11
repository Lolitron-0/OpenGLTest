#include "CameraBase.hpp"

CameraBase::CameraBase(glm::vec3 position, glm::vec3 worldUp, float pitch, float yaw) :
	mPosition(position), mWorldUp(worldUp), mPitch(pitch), mYaw(yaw), mMouseSensitivity(0.1f)
{
	updateCamera();
}

glm::mat4 CameraBase::getViewMatrix()
{
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

glm::vec3 CameraBase::getPosition()
{
	return mPosition;
}

void CameraBase::updateCamera()
{
	glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);

	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));
}
