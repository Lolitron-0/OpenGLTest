#version 330 core
struct Material {
	sampler2D diffuse;
    sampler2D specular;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Spotlight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);

out vec4 fragmentColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform Material material;
uniform DirLight light;

uniform vec3 viewPos;

uniform sampler2D textureObj;

void main() {
	vec3 viewDir = normalize(FragPos - viewPos);

	vec3 result = calcDirLight(light, Normal, viewDir);
	fragmentColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {	
	vec3 lightDir = normalize(light.direction);
	vec3 norm = normalize(normal);

	// diffuse
	float diff = max(dot(norm, -lightDir), 0.0); // to match normal dir

	//specular
	vec3 reflectDir = reflect(lightDir, norm); //actual reflect
	float spec = pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = (diff * vec3(texture(material.diffuse, TexCoord))) * light.diffuse;
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));

	vec3 result = (ambient + diffuse + specular);
	return (ambient + diffuse + specular);
}