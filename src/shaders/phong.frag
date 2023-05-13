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

struct SpotLight {
	vec3 position;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

out vec4 fragmentColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform Material material;
uniform PointLight light;

uniform vec3 viewPos;

uniform sampler2D textureObj;

void main() {
	vec3 viewDir = normalize(FragPos - viewPos);

	//vec3 result = calcDirLight(light, Normal, viewDir);
	vec3 result = calcPointLight(light, Normal, FragPos, viewDir);
	//vec3 result = calcSpotLight(light, Normal, FragPos, viewDir);
	fragmentColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {	
	vec3 lightDir = normalize(light.direction);
	vec3 norm = normalize(normal);

	// diffuse
	float diff = max(dot(norm, -lightDir), 0.0); // negate to match normal dir

	//specular
	vec3 reflectDir = reflect(lightDir, norm); //actual reflect
	float spec = pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));

	return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(fragPos - light.position);
	vec3 norm = normalize(normal);

	// diffuse
	float diff = max(dot(norm, -lightDir), 0.0); // negate to match normal dir

	//specular
	vec3 reflectDir = reflect(lightDir, norm); //actual reflect
	float spec = pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess);

	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.quadratic*(distance*distance) +
							   light.linear*distance + 
							   light.constant); 


	vec3 ambient =  attenuation * light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse =  attenuation * light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));
	vec3 specular = attenuation * light.specular * (spec * vec3(texture(material.specular, TexCoord)));

	return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(fragPos - light.position);
	vec3 norm = normalize(normal);
	float theta = dot(lightDir, normalize(light.direction)); // cos between spotlight center vec and fragment dir

	// diffuse
	float diff = max(dot(norm, -lightDir), 0.0); // negate to match normal dir

	//specular
	vec3 reflectDir = reflect(lightDir, norm); //actual reflect
	float spec = pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess);

	vec3 ambient =  light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse =  light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));

	float cutoffLerp = clamp((theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff), 0.0, 1.0);
	diffuse *= cutoffLerp;
	specular *= cutoffLerp;

	return  (ambient + diffuse + specular);
}