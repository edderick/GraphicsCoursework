//Standard includes
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

//GL stuff
#include <GL/glew.h>
#include <GL/glfw.h>

//GLM stuff
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//local includes
#include "utils.hpp"
#include "object.hpp"

const GLuint WINDOW_WIDTH = 800;
const GLuint WINDOW_HEIGHT = 800;
const GLfloat PI = 3.14159; 

typedef enum Screen{
	OBJECT,
} Screen;

Screen screen = OBJECT;

void key_callback(int key, int action){
	if ((key == 'A' || key == 'a') && action == GLFW_PRESS){
		screen = OBJECT;
	}
}


int main(int argc, char *argv[]){

	//Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "There was a problem initializing glfw");
		exit(EXIT_FAILURE);
	}

	//glfw hints
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Open a window
	if (!glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
		fprintf(stderr, "There was a problem opening a window");
		exit(EXIT_FAILURE);
	}

	//Turn on experimental features to prevent seg fault
	glewExperimental = GL_TRUE;

	//Intitialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "There was a problem initialising GLEW");
		exit(EXIT_FAILURE);
	}

	//various glfw settings
	glfwEnable(GLFW_STICKY_KEYS);
	glfwSetKeyCallback(&key_callback);

	glEnable(GL_DEPTH_TEST);

	//Load in and set program (Shaders)
	GLuint programID = setupShaders("shaders/vert.gls", "shaders/frag.gls");
	GLuint phongID = setupShaders("shaders/phongVert.gls", "shaders/phongFrag.gls");

	//TODO: Consider removing...
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Object object("models/normals.obj", phongID, GL_FILL);

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		switch (screen) {
		
			case OBJECT:
				glfwSetWindowTitle("Object");	
				object.setUpDefaultMVP();
				object.draw();
				break;
			
		}

		glfwSwapBuffers();
	} while ( (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS) && 
			(glfwGetKey('q') != GLFW_PRESS) &&
			(glfwGetKey('Q') != GLFW_PRESS) &&
			(glfwGetWindowParam( GLFW_OPENED )) );

	glfwTerminate();

	//Everything was okay, return zero
	exit(EXIT_SUCCESS);
}

