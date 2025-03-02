#version 430 core

in vec3 normal;
in vec2 texCoords;
in vec3 fragPos;

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

uniform sampler2D textureDiffuse1;
uniform sampler2D textureDiffuse2;
uniform sampler2D textureDiffuse3;
uniform sampler2D textureSpecular1;
uniform sampler2D textureSpecular2;
uniform float shininess;

uniform vec3 cameraPos;
#define NR_POINTLIGHTS 2
uniform pointLight pLight[NR_POINTLIGHTS];

out vec4 fragColor;

vec3 calculatePointLight(pointLight light, vec3 fragPos, vec3 normal, vec3 cameraDir)
{
	vec3 ambient = light.ambient*vec3(texture(textureDiffuse1,texCoords));
	vec3 lightDir = normalize(light.pos - fragPos);
	float diff = max(dot(normal,lightDir),0);
	vec3 diffuse = light.diffuse*diff*vec3(texture(textureDiffuse1,texCoords));
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(cameraDir,reflectDir),0),shininess);
	vec3 specular = light.specular*spec*vec3(texture(textureSpecular1,texCoords));
	float distance = length(light.pos - fragPos);
	float attenuation = 1.0/(light.constant + light.linear*distance + light.quadratic*(distance*distance));
	vec3 result = (ambient+diffuse+specular)*attenuation;
	return result;
};

void main()
{   
	vec3 normal = normalize(normal);
	vec3 cameraDir = normalize(cameraPos - fragPos);
	vec3 result;
	for(int i = 0; i < NR_POINTLIGHTS; i++)
	{
		result += calculatePointLight(pLight[i], fragPos, normal, cameraDir);
	}
	fragColor = vec4(result,1.0);
}