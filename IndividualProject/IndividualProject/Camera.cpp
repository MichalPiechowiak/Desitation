#include "Camera.h"

Camera::Camera(Camera_Settings camera_settings)
{
	this->screenWidth = camera_settings.screenWidth;
	this->screenHeight = camera_settings.screenHeight;
	this->NearPlane = camera_settings.nearPlane;
	this->FarPlane = camera_settings.farPlane;

	this->Yaw = YAW;
	this->Pitch = PITCH;
	this->MovementSpeed = SPEED;
	this->MouseSensitivity = SENSITIVITY;
	this->Zoom = ZOOM;

	this->Position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->Front = glm::vec3(0.0f, 0.0f, -1.0f);

	updateCameraVectors();
}
void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, WorldUp);
}
void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	updateCameraVectors();
}
void Camera::processMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}
glm::vec3 Camera::getCameraPosition()
{
	return this->Position;
}