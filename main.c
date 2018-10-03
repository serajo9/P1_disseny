/*  by Alun Evans 2016 LaSalle (aevanss@salleurl.edu) */

//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

//include OpenGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//include some custom code files
#include "glfunctions.h" //include all OpenGL stuff
#include "Shader.h" // class to compile shaders

using namespace std;
using namespace glm;

//global variables to help us do things
int g_ViewportWidth = 2*512; int g_ViewportHeight = 2*512; // Default window size, in pixels
double mouse_x, mouse_y; //variables storing mouse position
const vec3 g_backgroundColor(0.2f, 0.2f, 0.2f); // background colour - a GLM 3-component vector

GLuint g_simpleShader = 0; //shader identifier
GLuint g_Vao = 0; //vao
GLuint g_NumTriangles = 0; //  Numbre of triangles we are painting.

// ------------------------------------------------------------------------------------------
// This function manually creates a square geometry (defined in the array vertices[])
// ------------------------------------------------------------------------------------------
void load()
{
	//**********************
	// CODE TO SET GEOMETRY
	//**********************

	//the positions of the corner of the square, each a 3-component vector, x, y, z
	const GLfloat vertices[] = { 0.1f, 0.4f, 0.0f,
		0.1f, -0.7f, 0.0f,
		-0.9f, -0.7f, 0.0f,
		-0.9f, 0.4f, 0.0f,
		-0.2f, -0.3, 0.0f,
		-0.2f, -0.7f, 0.0f,
		-0.6f, -0.7f, 0.0f,
		-0.6f, -0.3f, 0.0f,
		0.0f, 0.3f, 0.0f,
		0.0f, 0.1f, 0.0f,
		-0.2f, 0.1f, 0.0f,
		-0.2f, 0.3f, 0.0f,
		-0.6f, 0.3f, 0.0f,
		-0.6f, 0.1f, 0.0f,
		-0.8f, 0.1f, 0.0f,
		-0.8f, 0.3f, 0.0f,
		0.1f, 0.4f, 0.0f,
		-0.4f, 0.9f, 0.0f,
		-0.9f, 0.4f, 0.0f,
		1.0f, -0.7f, 0.0f,//19
		1.0f, -1.0f, 0.0f,//20
		-1.0f, -1.0f, 0.0f,//21
		-1.0f, -0.7f, 0.0f,//22
		-1.0f, 1.0f, 0.0f, //--23-- punt superior esquerra fons blau
		1.0f, 1.0f, 0.0f,	//--24 - punt superior dret fons blau
		1.0f, -0.7f, 0.0f, // 25 igual que el 19 fons blau
		-1.0f, -0.7f, 0.0f, //26 igual que el 22 fons blau
		0.5f, -0.7f, 0.0f, //27 tronc inferior esquerra
		0.6,-0.7f, 0.0f, //28 tronc inferior dret
		0.6,-0.4f, 0.0f ,//29 tronc superior dret
		0.5,-0.4f, 0.0f, //30 tronc superior esquerra
		0.55f,0.4f, 0.0f, //31 arbre part fulla superior
		0.3f , -0.4f, 0.0f, // 32 arbre part fulla esquerra
		0.8f, -0.4f, 0.0f // 33 arbre part fulla dreta
	};
	
	//we assign a colour to each corner (each colour is RGB)
	const GLfloat colors[] = { 0.95f, 0.64f, 0.37f,
		0.95f, 0.64f, 0.37f,
		0.95f, 0.64f, 0.37f,
		0.95f, 0.64f, 0.37f,
		0.54f, 0.27f, 0.07f,//4
		0.54f, 0.27f, 0.07f,//5
		0.54f, 0.27f, 0.07f,//6
		0.54f, 0.27f, 0.07f,//7
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.54f, 0.27f, 0.07f, //16 --marró taulada   
		0.54f, 0.27f, 0.07f,//17 --marró taulada	 
		0.54f, 0.27f, 0.07f, //18 --marró taulada
		0.133f, 0.54f, 0.133f, //19 --verd
		0.133f, 0.54f, 0.133f, //20 --verd
		0.133f, 0.54f, 0.133f, //21 --verd
		0.133f, 0.54f, 0.133f, //22 --verd
		0.0f, 0.0f, 1.0f, //23
		0.0f, 0.0f, 1.0f, //24
		0.0f, 0.0f, 1.0f, //25
		0.0f, 0.0f, 1.0f, //26
		0.54f, 0.27f, 0.07f,
		0.54f, 0.27f, 0.07f,
		0.54f, 0.27f, 0.07f,
		0.54f, 0.27f, 0.07f, //30
		0.133f, 0.54f, 0.133f, //31 --verd
		0.133f, 0.54f, 0.133f, //32 --verd
		0.133f, 0.54f, 0.133f //33 ---verd
	};

	// The index buffer references the vertices we paint, in order
	//here we have two triangles
	const GLuint indices[] = {26,25,23,23,24,25, 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 19, 20, 21, 19, 21, 22, 27, 28, 29, 29, 30,27, 31,32,33 };



	//**********************
	// CODE TO LOAD EVERYTHING INTO MEMORY
	//**********************

	//load the shader
	Shader simpleShader("src/shader.vert", "src/shader.frag");
	g_simpleShader = simpleShader.program;

	// Create the VAO where we store all geometry (stored in g_Vao)
	g_Vao = gl_createAndBindVAO();

	//create vertex buffer for positions, colors, and indices, and bind them to shader
	gl_createAndBindAttribute(vertices, sizeof(vertices), g_simpleShader, "a_vertex", 3);
	gl_createAndBindAttribute(colors, sizeof(colors), g_simpleShader, "a_color", 3);
	gl_createIndexBuffer(indices, sizeof(indices));

	//unbind everything
	gl_unbindVAO();

	//store number of triangles (use in draw())
	g_NumTriangles = sizeof(indices) / (sizeof(GLuint) * 3);
}

// ------------------------------------------------------------------------------------------
// This function actually draws to screen and called non-stop, in a loop
// ------------------------------------------------------------------------------------------
void draw()
{
	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// activate shader
	glUseProgram(g_simpleShader);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);

}

// ------------------------------------------------------------------------------------------
// This function is called every time you press a screen
// ------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//quit
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
	//reload
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		load();
}

// ------------------------------------------------------------------------------------------
// This function is called every time you click the mouse
// ------------------------------------------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		cout << "Left mouse down at" << mouse_x << ", " << mouse_y << endl;
	}
}

int main(void)
{
	//setup window and boring stuff, defined in glfunctions.cpp
	GLFWwindow* window;
	if (!glfwInit())return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(g_ViewportWidth, g_ViewportHeight, "Hello OpenGL!", NULL, NULL);
	if (!window) { glfwTerminate();	return -1; }
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	//input callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	//load all the resources
	load();

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		draw();

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();

		//mouse position must be tracked constantly (callbacks do not give accurate delta)
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
	}

	//terminate glfw and exit
	glfwTerminate();
	return 0;
}



