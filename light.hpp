#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>

class Light {
private:
	glm::vec3 color;
};

class GlobalAmbient : public Light {
public:
	GlobalAmbient(glm::vec3 color);
};

GlobalAmbient::GlobalAmbient(glm::vec3 color) {
	color = color;
}

class DirectionLight : public Light {
public:
	DirectionLight(glm::vec3 direction, glm::vec3 color);
private:
	glm::vec3 direction;
};

DirectionLight::DirectionLight(glm::vec3 direction, glm::vec3 color) {
	direction = direction;
	color = color;
}

class PositionLight : public Light {
public:
	PositionLight(glm::vec3 pos, glm::vec3 color, float constant, float linear, float quadratic);
private:
	glm::vec3 pos;
	float constant;
	float linear;
	float quadratic;
};

PositionLight::PositionLight(glm::vec3 pos, glm::vec3 color, float constant, float linear, float quadratic) {
	pos = pos;
	color = color;
	constant = constant;
	linear = linear;
	quadratic = quadratic;
}

class SpotLight :public Light {
public:
	SpotLight(glm::vec3 pos, glm::vec3 color, glm::vec3 direction, float phi);
private:
	glm::vec3 pos;
	glm::vec3 direction;
	float phi;
};

SpotLight::SpotLight(glm::vec3 pos, glm::vec3 color, glm::vec3 direction, float phi) {
	pos = pos;
	color = color;
	direction = direction;
	phi = phi;
}

#endif