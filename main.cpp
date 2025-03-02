#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "model.hpp"
#include "transparent.hpp"
#include "skybox.hpp"

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
	glDepthFunc(GL_LEQUAL);

	Model backpack("./Models/nanosuit_reflection/nanosuit.obj");
	std::vector<std::string> faces{
		"./Textures/skybox/right.jpg",
		"./Textures/skybox/left.jpg",
		"./Textures/skybox/top.jpg",
		"./Textures/skybox/bottom.jpg",
		"./Textures/skybox/front.jpg",
		"./Textures/skybox/back.jpg"
	};
	Skybox skybox(faces);
	Shader modelShader("./Shaders/model.vert", "./Shaders/model_environmentmapping.frag");
	Shader skyboxShader("./Shaders/skybox.vert", "./Shaders/skybox.frag");

	GLuint uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f);
		glm::mat4 view = camera.getViewMat();
		glm::mat4 projection = camera.getProjectionMat();

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		modelShader.use();
		model = glm::scale(model, glm::vec3(0.2f));
		modelShader.setMat4("model", model);
		modelShader.setVec3("cameraPos", camera.getPos());
		modelShader.setVec3("pLight[0].pos", glm::vec3(1.2f, 0.5f, 1.0f));
		modelShader.setVec3("pLight[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		modelShader.setVec3("pLight[0].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		modelShader.setVec3("pLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		modelShader.setFloat("pLight[0].constant", 1.0f);
		modelShader.setFloat("pLight[0].linear", 0.09f);
		modelShader.setFloat("pLight[0].quadratic", 0.031f);
		modelShader.setVec3("pLight[1].pos", camera.getPos());
		modelShader.setVec3("pLight[1].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		modelShader.setVec3("pLight[1].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		modelShader.setVec3("pLight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		modelShader.setFloat("pLight[1].constant", 1.0f);
		modelShader.setFloat("pLight[1].linear", 0.09f);
		modelShader.setFloat("pLight[1].quadratic", 0.031f);
		modelShader.setInt("skybox", 3);
		skybox.texture.use(GL_TEXTURE3);
		backpack.Draw(modelShader);

		skyboxShader.use();
		skybox.draw(skyboxShader);

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