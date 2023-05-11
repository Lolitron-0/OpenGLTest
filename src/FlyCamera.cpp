#include "FlyCamera.hpp"

FlyCamera::FlyCamera(glm::vec3 position, glm::vec3 worldUp) :
	CameraBase(position, worldUp, 0, -90), mMoveSpeed(4.5f)
{
}

void FlyCamera::processMouseMove(float xOffset, float yOffset, float timeScale)
{
	xOffset *= mMouseSensitivity;
	yOffset *= mMouseSensitivity;

	mYaw += xOffset;
	mPitch += yOffset;

	if (mPitch > 89.f)
		mPitch = 89.f;
	if (mPitch < -89.f)
		mPitch = -89.f;

	updateCamera();
}

void FlyCamera::processKeyboard(CameraMovementDirection direction, float timeScale)
{
	float velocity = timeScale * mMoveSpeed;
	switch (direction)
	{
	case CameraMovementDirection::Forward:
		mPosition += mFront * velocity;
		break;
	case CameraMovementDirection::Backward:
		mPosition -= mFront * velocity;
		break;
	case CameraMovementDirection::Right:
		mPosition += mRight * velocity;
		break;
	case CameraMovementDirection::Left:
		mPosition -= mRight * velocity;
		break;
	case CameraMovementDirection::Up:
		mPosition += mUp * velocity;
		break;
	case CameraMovementDirection::Down:
		mPosition -= mUp * velocity;
		break;
	default:
		break;
	}
}
