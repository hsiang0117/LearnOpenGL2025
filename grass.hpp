#ifndef GRASS_HPP
#define GRASS_HPP

#include <glew.h>
#include <glm/glm.hpp>
#include <assimp/types.h>
#include "texture.hpp"
#include "shader.hpp"

float vertices[] = {
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f
};

class Grass 
{
public:
	Grass();
	void draw(Shader& shader);
private:
	Texture texture = Texture("./Textures/grass.png");
	GLuint VAO, VBO;
};

Grass::Grass()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Grass::draw(Shader& shader)
{
	shader.setInt("grass_texture", 0);
	texture.use(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}
#endif // !GRASS_HPP
