#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct Camera_Settings
{
	unsigned int screenWidth;
	unsigned int screenHeight;
	double nearPlane;
	double farPlane;
};

//Defoult values
const double YAW = -90.0f;
const double PITCH = 0.0f;
const double SPEED = 2.5f;
const double SENSITIVITY = 0.1f;
const double ZOOM = 45.0f;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
private:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	double Yaw;
	double Pitch;

	double MovementSpeed;
	double MouseSensitivity;

	double NearPlane;
	double FarPlane;

	unsigned int screenWidth;
	unsigned int screenHeight;

	void updateCameraVectors();
public:
	double Zoom;

	Camera(Camera_Settings);
	glm::mat4 GetViewMatrix();
	void processMouseMovement(float, float, bool constrainPitch = true);
	void processMouseScroll(float);
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	glm::vec3 getCameraPosition();
};
#endif // !CAMERA_H