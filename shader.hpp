#ifndef SHADER_HPP
#define SHADER_HPP

#include <glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    GLuint ID;
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
    void use();
	void setVec3(const char* name, glm::vec3 vec);
	void setMat4(const char* name, glm::mat4 mat);
	void setFloat(const char* name, float value);
	void setInt(const char* name, int value);
};

std::string readShaderFile(const char* shaderFilePath) {
	std::ifstream shaderFile(shaderFilePath);
	std::stringstream fileStream;
	fileStream << shaderFile.rdbuf();
	shaderFile.close();
	return fileStream.str();
}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vShaderContent = readShaderFile(vertexShaderPath);
	std::string fShaderContent = readShaderFile(fragmentShaderPath);

	const char* vshaderCode = vShaderContent.c_str();
	const char* fshaderCode = fShaderContent.c_str();

	glShaderSource(vShader, 1, &vshaderCode, NULL);
	glShaderSource(fShader, 1, &fshaderCode, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);

	ID = glCreateProgram();
	glAttachShader(ID, vShader);
	glAttachShader(ID, fShader);
	glLinkProgram(ID);
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setVec3(const char* name, glm::vec3 vec) {
	int location = glGetUniformLocation(ID, name);
	glUniform3fv(location, 1, glm::value_ptr(vec));
}

void Shader::setMat4(const char* name, glm::mat4 mat) {
	int location = glGetUniformLocation(ID, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setFloat(const char* name, float value) {
	int location = glGetUniformLocation(ID, name);
	glUniform1f(location, value);
}

void Shader::setInt(const char* name, int value) {
	int location = glGetUniformLocation(ID, name);
	glUniform1i(location, value);
}
#endif