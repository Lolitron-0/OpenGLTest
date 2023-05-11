#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <iostream>
#include <vector>
#include "Shader.hpp"
#include <include/FlyCamera.hpp>
#include <include/ResourceManager.hpp>


void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);

void GLAPIENTRY errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "Notification >> ";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "Low severity >> ";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "Medium severity >> ";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "High severity >> ";
		break;
	default:
		std::cout << "Unknown severity " << severity << " >> ";
		break;
	}
	std::cout << message << std::endl;
	if (severity == GL_DEBUG_SEVERITY_HIGH)
		__debugbreak();
}


FlyCamera camera(glm::vec3(0.f, 1.f, 5.f), glm::vec3(0.f, 1.f, 0.f));

glm::vec3 lightPos(1.2f, 0.0f, 2.0f);

float lastFrame = 0.f, currentFrame;
float deltaTime;
float timeScale;

float lastMouseX = -1, lastMouseY = -1;


int main(void)
{
	srand(time(0));
    if (!glfwInit())
        return -1;

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);

    if (glewInit() != GLEW_OK)
        std::cerr << "Failed to load glew!" << std::endl;       
    std::cout << glGetString(GL_VERSION) << std::endl;


	glDebugMessageCallback(&errorCallback, nullptr);


    float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

	auto diffuseMap = ResourceManager::loadTextureFromFile("resources\\container2.png");
	auto specularMap = ResourceManager::loadTextureFromFile("resources\\container2_specular.png");

    unsigned int vbo; //vertex buffer
    glGenBuffers(1, &vbo);

	unsigned int cubeVao, lightSourceVao; //attrib buffer
	glGenVertexArrays(1,&cubeVao);

	glBindVertexArray(cubeVao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3,
			GL_FLOAT,
			GL_FALSE, //should normalise
			8 * sizeof(float), //stride
			(void*)0); //offset (but pointer?)
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3,
			GL_FLOAT,
			GL_FALSE, //should normalise
			8 * sizeof(float), //stride
			(void*)(3*sizeof(float))); //offset 
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2,
			GL_FLOAT,
			GL_FALSE, //should normalise
			8 * sizeof(float), //stride
			(void*)(6*sizeof(float))); //offset 
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &lightSourceVao);
	glBindVertexArray(lightSourceVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3,
		GL_FLOAT,
		GL_FALSE, //should normalise
		8 * sizeof(float), //stride
		(void*)0); //offset
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), 800.f / 600, 0.1f, 100.f);

	glm::mat4 model(1.f);
	glm::mat4 lightModel(1.f);

	Shader phong("src\\shaders\\phong.vert", "src\\shaders\\phong.frag");
	if (!phong.isSuccessful())
		std::cerr << phong.getErrorLog();

	phong.use();
	phong.setMat4("projection", projection);
	phong.setInt("material.diffuse", 0);
	phong.setInt("material.specular", 1);
	phong.setFloat("material.shininess", 32.0f);
	phong.setVec3("light.ambient", .2f, .2f, .2f);
	phong.setVec3("light.diffuse", .5f, .5f, .5f);
	phong.setVec3("light.specular", 1.f, 1.f, 1.f);

	Shader lightSourceShader("src\\shaders\\lightSource.vert", "src\\shaders\\lightSource.frag");
	if (!lightSourceShader.isSuccessful())
		std::cerr << phong.getErrorLog();

	lightSourceShader.use();
	lightSourceShader.setMat4("projection", projection);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		timeScale = (1.0 - pow(0.95, deltaTime)) * 20.f; // 20.f is "everythings' speed multiplier" constant
		lastFrame = currentFrame;
		
		
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		processInput(window);
		auto view = camera.getViewMatrix();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		phong.use();
		glBindVertexArray(cubeVao);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		//model = glm::rotate(model, glm::radians(50.f * timeScale), glm::vec3(1.f, 1.f, 1.f));

		lightPos.x = 1.5f * sin(glfwGetTime()*2);
		lightPos.y = 1.5f * cos(glfwGetTime()*2);

		glm::vec3 lightColor(1.f);

		glm::vec3 lightDiffuse = lightColor * 0.5f;
		glm::vec3 lightAmbient = lightColor * 0.2f;

		phong.setVec3("light.ambient", lightAmbient);
		phong.setVec3("light.diffuse", lightDiffuse);

		phong.setVec3("viewPos", camera.getPosition());
		phong.setVec3("light.direction", glm::vec3(-1.f));
		phong.setMat4("model", model);
		phong.setMat4("view", view);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightSourceShader.use();
		glBindVertexArray(lightSourceVao);


		lightModel = glm::mat4(1.f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));

		lightSourceShader.setVec3("lightColor", lightColor);
		lightSourceShader.setMat4("model", lightModel);
		lightSourceShader.setMat4("view", view);

		glDrawArrays(GL_TRIANGLES, 0, 36);


		glBindVertexArray(0);
		glUseProgram(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W))
		camera.processKeyboard(CameraMovementDirection::Forward, timeScale);
	if (glfwGetKey(window, GLFW_KEY_S))
		camera.processKeyboard(CameraMovementDirection::Backward, timeScale);
	if (glfwGetKey(window, GLFW_KEY_D))
		camera.processKeyboard(CameraMovementDirection::Right, timeScale);
	if (glfwGetKey(window, GLFW_KEY_A))
		camera.processKeyboard(CameraMovementDirection::Left, timeScale);
	if (glfwGetKey(window, GLFW_KEY_SPACE))
		camera.processKeyboard(CameraMovementDirection::Up, timeScale);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
		camera.processKeyboard(CameraMovementDirection::Down, timeScale);
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	float xOffset = xPos - lastMouseX;
	float yOffset = lastMouseY - yPos;
	if (lastMouseX == -1) {
		xOffset = 0;
		yOffset = 0;
	}

	camera.processMouseMove(xOffset, yOffset, timeScale);

	lastMouseX = xPos;
	lastMouseY = yPos;
}
