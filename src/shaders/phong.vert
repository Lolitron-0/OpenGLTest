#version 330 core
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat3 normalModel;
uniform mat4 view;
uniform mat4 projection;


void main() {
	gl_Position = projection * view * model * vec4(aPosition, 1.0);
	FragPos = vec3(model * vec4(aPosition, 1.0));
	Normal = normalModel * aNormal;

	TexCoord = aTexCoord;
}