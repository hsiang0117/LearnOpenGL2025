#version 430 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 fragColor;

uniform vec3 cameraPos;
uniform struct Light
{	
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
}light;

uniform struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
}material;

void main()
{
	vec3 ambient = light.ambient*vec3(texture(material.diffuse,texCoord));
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm,lightDir),0);
	vec3 diffuse = light.diffuse*diff*vec3(texture(material.diffuse,texCoord));
	vec3 cameraDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(cameraDir,reflectDir),0),material.shininess);
	vec3 specular = light.specular*spec*vec3(texture(material.specular,texCoord));
	fragColor = vec4((ambient+diffuse+specular),1.0);
}