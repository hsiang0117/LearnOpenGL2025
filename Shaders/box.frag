#version 430 core

in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

float ambient = 0.2f;
float specularStrength = 0.5;

void main()
{
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm,lightDir),0);
	vec3 diffuse = diff*lightColor;
	vec3 cameraDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(cameraDir,reflectDir),0),32);
	vec3 specular = specularStrength*spec*lightColor;
	fragColor = vec4((vec3(ambient)+diffuse+specular)*objectColor,1.0);
}