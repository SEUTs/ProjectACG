// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"

//variables
GLFWwindow* window;
const int width = 1024, height = 1024;


float i = 0.0f;
float stepSize = 1.0f;
double xPosition, yPosition;
int nr = 1;

//Handling cursor position
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	xPosition = xpos;
	yPosition = ypos;
	std::cout << "The mouse cursor is: " << xpos << " " << ypos << std::endl;
}


//create matrices for transforms
glm::mat4 transforms[20];
glm::mat4 trans(1.0f);

//Ex 3: Add callback for mouse button
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		trans = glm::translate(glm::mat4(1.0f),
			glm::vec3(xPosition / (width / 2) - 1,
				1 - yPosition / (height / 2),
				0));

		transforms[nr] = trans;
		nr++;
	}
}

//Ex 4: Complete callback for adjusting the viewport when resizing the window
void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	//what should we do here?
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Lab 4", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	float HPos = 0.0f, VPos = 0.0f;
	GLfloat vertices[] = {
		0.025f,  0.05f, 0.0f,  // top right
		0.025f, -0.05f, 0.0f,  // bottom right
		-0.025f, -0.05f, 0.0f,  // bottom left
		-0.025f,  0.05f, 0.0f,   // top left 

		-0.425f, 0.21f, 0.0f,
		-0.425f, 0.11f, 0.0f,
		-0.375f, 0.11f, 0.0f,
		-0.375f, 0.21f, 0.0f,

		0.425f, 0.21f, 0.0f,
		0.425f, 0.11f, 0.0f,
		0.375f, 0.11f, 0.0f,
		0.375f, 0.21f, 0.0f,
	};

	GLuint indices[] = {  // note that we start from 0!
		0, 3, 1,  // first Triangle
		1, 3, 2,   // second Triangle

		4, 7, 5,
		5, 7, 6,

		8, 11, 9,
		9, 11, 10,
	};

	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object. 
	GLuint vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	//bind VAO
	glBindVertexArray(vao);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//maybe we can play with different positions
	glm::vec3 positions[] = {
		glm::vec3(0.0f,  0.0f,  0),
		glm::vec3(0.2f,  0.5f, 0),
		glm::vec3(-0.15f, -0.22f, 0),
		glm::vec3(-0.38f, -0.2f, 0),
		glm::vec3(0.24f, -0.4f, 0),
		glm::vec3(-0.17f,  0.3f, 0),
		glm::vec3(0.23f, -0.2f, 0),
		glm::vec3(0.15f,  0.2f, 0),
		glm::vec3(0.15f,  0.7f, 0),
		glm::vec3(-0.13f,  0.1f, 0)
	};

	transforms[0] = trans;

	// Set a callback for handling mouse cursor position
	// Decomment for a callback example
	//glfwSetCursorPosCallback(window, cursor_position_callback);

	//Ex4 - Set callback for window resizing
	glfwSetFramebufferSizeCallback(window, window_callback);


	float startTime = 0;
	bool mouseNotPressed = true, leftNotPressed = true, rightNotPressed = true;
	bool canMoveUpDown = true, upNotPressed = true, downNotPressed = true;
	float movementAmplification = 5;
	// Check if the window was closed
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		//bind VAO
		glBindVertexArray(vao);

		trans = glm::translate(trans, positions[0]);

		if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ||
			glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			&& mouseNotPressed == true) {
			startTime = (float)glfwGetTime();
			mouseNotPressed = false;
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			leftNotPressed = false;
		}
		if (glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS) {
			leftNotPressed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			rightNotPressed = false;
		}
		if (glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS) {
			rightNotPressed = true;
		}

		if (leftNotPressed == false || rightNotPressed == false) {
			int direction = 0;
			if (leftNotPressed == false)
				direction -= 1;
			if (rightNotPressed == false)
				direction += 1;

			float newHPos = HPos + direction * 0.0002f * movementAmplification;
			if (newHPos > -0.8f && newHPos < 0.8f) {
				HPos = newHPos;
				vertices[0] = (float)-0.025f + HPos;
				vertices[3] = (float)-0.025f + HPos;
				vertices[6] = (float)0.025f + HPos;
				vertices[9] = (float)0.025f + HPos;
			}

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}

		if (canMoveUpDown) {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				upNotPressed = false;
			}
			if (glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS) {
				upNotPressed = true;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				downNotPressed = false;
			}
			if (glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS) {
				downNotPressed = true;
			}

			if (upNotPressed == false || downNotPressed == false) {
				int direction = 0;
				if (downNotPressed == false)
					direction -= 1;
				if (upNotPressed == false)
					direction += 1;

				float newVPos = VPos + direction * 0.00015f * movementAmplification;
				if (newVPos > -0.4f && newVPos < 0.2f) {
					VPos = newVPos;
					vertices[1] = (float)0.05f + VPos;
					vertices[4] = (float)-0.05f + VPos;
					vertices[7] = (float)-0.05f + VPos;
					vertices[10] = (float)0.05f + VPos;
				}

				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			}
		}

		if (!canMoveUpDown) {
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS &&
				glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {
				mouseNotPressed = true;
			}

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
				trans = glm::rotate(trans, ((float)glfwGetTime() - startTime) * 0.3f, glm::vec3(0.0, 0.0, 1.0));
			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
				trans = glm::rotate(trans, ((float)glfwGetTime() - startTime) * 0.3f, glm::vec3(0.0, 0.0, -1.0));
			}
		}

		// send variables to shader

		//transform matrix
		unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		//color
		unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
		glm::vec4 color = glm::vec4(0.5f, 0, 0.5f, 1.0);
		glUniform4fv(transformLoc2, 1, glm::value_ptr(color));

		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

	}

	// Cleanup
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
