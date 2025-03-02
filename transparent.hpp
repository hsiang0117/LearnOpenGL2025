#ifndef GRASS_HPP
#define GRASS_HPP

#include <glew.h>
#include <glm/glm.hpp>
#include <assimp/types.h>
#include "texture.hpp"
#include "shader.hpp"

class Transparent 
{
public:
	Transparent(const char* texturePath);
	void draw(Shader& shader);
private:
	Texture texture;
	GLuint VAO, VBO;
	float vertices[20] = {
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f
	};
};

Transparent::Transparent(const char* texturePath)
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	texture = Texture(texturePath);
}

void Transparent::draw(Shader& shader)
{
	shader.setInt("transparent_texture", 0);
	texture.use(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}
#endif // !GRASS_HPP
