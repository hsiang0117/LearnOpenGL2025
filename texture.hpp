#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glew.h>
#include <iostream>
#include <assimp/types.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture {
public:
	GLuint ID;
	std::string type;
	aiString path;
	Texture();
	Texture(const char* texturePath);
	Texture(const char* texturePath, const std::string type, const aiString path);
	void use(GLenum textureUnit);
};


Texture::Texture() {
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}


Texture::Texture(const char* texturePath) {
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	//为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1) {
			format = GL_RED;
		}
		else if (nrChannels == 3) {
			format = GL_RGB;
		}
		else if (nrChannels == 4) {
			format = GL_RGBA;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
			float anisoSetting = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
		}
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const char* texturePath, const std::string type, const aiString path) {
	this->type = type;
	this->path = path;
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	//为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1) {
			format = GL_RED;
		}
		else if (nrChannels == 3) {
			format = GL_RGB;
		}
		else if (nrChannels == 4) {
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
			float anisoSetting = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
		}
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::use(GLenum textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

class CubeMap {
public:
	GLuint ID;
	CubeMap() {};
	CubeMap(std::vector<std::string> faces);
	void use(GLenum textureUnit);
};

CubeMap::CubeMap(std::vector<std::string> faces) {
	stbi_set_flip_vertically_on_load(false);
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	int width, height, nrChannels;
	unsigned char* data;
	for (unsigned int i = 0; i < faces.size(); i++) {
		data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			GLenum format;
			if (nrChannels == 1) {
				format = GL_RED;
			}
			else if (nrChannels == 3) {
				format = GL_RGB;
			}
			else if (nrChannels == 4) {
				format = GL_RGBA;
			}
			glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::use(GLenum textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

#endif