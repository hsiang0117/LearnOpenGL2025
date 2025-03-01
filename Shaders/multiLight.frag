#version 430 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 fragColor;

uniform vec3 cameraPos;

struct directionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct pointLight
{
	vec3 pos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

struct spotLight
{
	vec3 pos;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float cutOff;
	float outerCutOff;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform directionalLight dLight;
#define NR_POINT_LIGHTS 1
uniform pointLight pLight[NR_POINT_LIGHTS];
uniform spotLight sLight;
uniform Material material;

vec3 calculateDirectionalLight(directionalLight light, vec3 normal, vec3 cameraDir)
{
	vec3 ambient = light.ambient*vec3(texture(material.diffuse,texCoord));
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal,lightDir),0);
	vec3 diffuse = light.diffuse*diff*vec3(texture(material.diffuse,texCoord));
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(cameraDir,reflectDir),0),material.shininess);
	vec3 specular = light.specular*spec*vec3(texture(material.specular,texCoord));
	vec3 result = ambient+diffuse+specular;
	return result;
};

vec3 calculatePointLight(pointLight light, vec3 fragPos, vec3 normal, vec3 cameraDir)
{
	vec3 ambient = light.ambient*vec3(texture(material.diffuse,texCoord));
	vec3 lightDir = normalize(light.pos - fragPos);
	float diff = max(dot(normal,lightDir),0);
	vec3 diffuse = light.diffuse*diff*vec3(texture(material.diffuse,texCoord));
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(cameraDir,reflectDir),0),material.shininess);
	vec3 specular = light.specular*spec*vec3(texture(material.specular,texCoord));
	float distance = length(light.pos - fragPos);
	float attenuation = 1.0/(light.constant + light.linear*distance + light.quadratic*(distance*distance));
	vec3 result = (ambient+diffuse+specular)*attenuation;
	return result;
};

vec3 calculateSpotLight(spotLight light, vec3 fragPos, vec3 normal, vec3 cameraDir)
{
	vec3 lightDir = normalize(light.pos - fragPos);
	float theta = dot(lightDir,normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta-light.outerCutOff)/epsilon,0.0,1.0);
	vec3 ambient = light.ambient*vec3(texture(material.diffuse,texCoord));
	float diff = max(dot(normal,lightDir),0);
	vec3 diffuse = light.diffuse*diff*vec3(texture(material.diffuse,texCoord));
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(cameraDir,reflectDir),0),material.shininess);
	vec3 specular = light.specular*spec*vec3(texture(material.specular,texCoord));
	vec3 result = ambient+(diffuse+specular)*intensity;
	return result;
};

void main()
{
	vec3 normal = normalize(normal);
	vec3 cameraDir = normalize(cameraPos - fragPos);
	vec3 result = calculateDirectionalLight(dLight,normal,cameraDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += calculatePointLight(pLight[i],fragPos,normal,cameraDir);
	result += calculateSpotLight(sLight,fragPos,normal,cameraDir);
	fragColor = vec4(result,1.0);
}