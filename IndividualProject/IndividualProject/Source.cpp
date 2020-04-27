#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Camera.h"
#include "Model.h"
#include "ShaderLoader.h"
#include "SkinnedMesh.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int witdth, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 800;

Camera_Settings camera_settings{ SCREEN_WIDTH, SCREEN_HEIGHT, 0.1, 100.0 };

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float passTime = 0.0f;

double lastX = SCREEN_WIDTH / 2;
double lastY = SCREEN_HEIGHT / 2;
bool firstMouse = true;

Camera camera(camera_settings);

int main()
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Facial Animation", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialized GLAD" << std::endl;
		return -1;
	}

	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	ShaderLoader Shader("../Resources/Shaders/Shader.vs", "../Resources/Shaders/Shader.fs");
	SkinnedMesh firstTest;
	firstTest.LoadMesh("../Resources/Models/Face.FBX");
	const int numberOfBones = firstTest.getNumberOfBones();
	std::vector<glm::mat4> Transforms;

	glm::vec4 light_position1 = { 0.0f, 0.0f, -17.0f, 1.0f };
	glm::vec4 light_position2 = { -5.0f, 0.0f, -12.0f, 1.0f };
	glm::vec4 light_position3 = { 0.0f, 0.0f, -7.0f, 1.0f };
	glm::vec4 light_position4 = { 5.0f, 0.0f, -12.0f, 1.0f };

	glm::vec4 light_ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	glm::vec4 light_diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec3 attenuation = { 1.0f, 0.10f, 0.08f };

	glm::vec4 mat_ambbient = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 mat_diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 mat_specular_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_specular_exponent = 32.0f;

	while (!glfwWindowShouldClose(window))
	{
		float curretnFrame = glfwGetTime();
		deltaTime = curretnFrame - lastFrame;
		lastFrame = curretnFrame;
		processInput(window);

		//Clear the screen
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader.Use();

		glm::mat4 projection = glm::perspective((float)glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 model2 = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -12.0f));
		model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
		model2 = glm::translate(model2, glm::vec3(4.0f, 0.0f, -15.0f));
		model2 = glm::scale(model2, glm::vec3(0.2f, 0.2f, 0.2f));

		Shader.setVec4("lightAmbient", light_ambient);
		Shader.setVec4("lightDiffuse", light_diffuse);

		Shader.setVec4("lightPosition1", light_position1);
		Shader.setVec4("lightPosition2", light_position2);
		Shader.setVec4("lightPosition3", light_position3);
		Shader.setVec4("lightPosition4", light_position4);

		Shader.setVec3("lightAttenuation", attenuation);
		Shader.setVec4("matAmbient", mat_ambbient);
		Shader.setVec4("matDiffuse", mat_diffuse);
		Shader.setVec4("matSpecularColour", mat_specular_colour);
		Shader.setFloat("matSpecularExponent", mat_specular_exponent);
		Shader.setVec3("eyePos", camera.getCameraPosition());
		Shader.setMat4("projection", projection);
		Shader.setMat4("view", view);
		Shader.setMat4("model", model);

		firstTest.BoneTransform(1, Transforms, 0);

		for (int i = 0; i < numberOfBones; i++)
		{
			std::stringstream name;
			name << "gBones[" << i << "]";
			glUniformMatrix4fv(glGetUniformLocation(Shader.ID, name.str().c_str()), 1, GL_FALSE, glm::value_ptr(Transforms[i]));
		}
		firstTest.Render(Shader);
		passTime += deltaTime;
		std::cout << "Animation time: " << passTime << std::endl;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}	
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}