#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "model.hpp"
#include "grass.hpp"

static float scrWidth = 800.0f;
static float scrHeight = 600.0f;
#include "camera.hpp"

void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

Camera camera;

int main() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(scrWidth, scrHeight, "program1", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		exit(-1);
	}

	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	Grass grass;
	Model backpack("./Models/backpack/backpack.obj");
	Shader backpackShader("./Shaders/backpack.vert", "./Shaders/backpack.frag");
	Shader grassShader("./Shaders/grass.vert", "./Shaders/grass.frag");

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		backpackShader.use();
		
		glm::mat4 model(1.0f);
		backpackShader.setMat4("model", model);

		glm::mat4 view = camera.getViewMat();
		backpackShader.setMat4("view", view);

		glm::mat4 projection = camera.getProjectionMat();
		backpackShader.setMat4("projection", projection);

		backpackShader.setVec3("cameraPos", camera.getPos());
		backpackShader.setVec3("pLight[0].pos", glm::vec3(1.2f, 0.5f, 1.0f));
		backpackShader.setVec3("pLight[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		backpackShader.setVec3("pLight[0].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		backpackShader.setVec3("pLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		backpackShader.setFloat("pLight[0].constant", 1.0f);
		backpackShader.setFloat("pLight[0].linear", 0.09f);
		backpackShader.setFloat("pLight[0].quadratic", 0.031f);
		backpackShader.setVec3("pLight[1].pos", camera.getPos());
		backpackShader.setVec3("pLight[1].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		backpackShader.setVec3("pLight[1].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		backpackShader.setVec3("pLight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		backpackShader.setFloat("pLight[1].constant", 1.0f);
		backpackShader.setFloat("pLight[1].linear", 0.09f);
		backpackShader.setFloat("pLight[1].quadratic", 0.031f);

		backpack.Draw(backpackShader);

		grassShader.use();
		grassShader.setMat4("model", model);
		grassShader.setMat4("view", view);
		grassShader.setMat4("projection", projection);
		grass.draw(grassShader);

		camera.updateDeltaTime();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FRONT);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACK);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processKeyboard(UP);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.processKeyboard(DOWN);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	camera.processMouseMovement(xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) 
{
	camera.processMouseScroll(yoffset);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	scrWidth = width;
	scrHeight = height;
}