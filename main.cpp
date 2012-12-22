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
#include "cone.hpp"
#include "sphere.hpp"
#include "object.hpp"

const GLuint WINDOW_WIDTH = 800;
const GLuint WINDOW_HEIGHT = 800;
const GLfloat PI = 3.14159; 

typedef enum Screen{
	WIRE_SPHERE,
	WIRE_CONE,
	HEDGEHOG,
	SHADED_SPHERE,
	OBJECT,
} Screen;

Screen screen = WIRE_SPHERE;

void key_callback(int key, int action){
	if((key == 'A' || key == 'a')  && action == GLFW_PRESS){
		screen = WIRE_SPHERE;
	} else if ((key == 'B' || key == 'b') && action == GLFW_PRESS){
		screen = WIRE_CONE;
	} else if ((key == 'C' || key == 'c') && action == GLFW_PRESS){
		screen = HEDGEHOG;
	} else if ((key == 'D' || key == 'd') && action == GLFW_PRESS){
		screen = SHADED_SPHERE;
	} else if ((key == 'F' || key == 'f') && action == GLFW_PRESS){
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
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	Cone cone(15, programID);
	Sphere wireSphere(20, programID);
	Sphere hedgehog(20, programID, GL_TRUE);
	Sphere shadedSphere(50, phongID, GL_FALSE, GL_TRUE, GL_FILL);

	Sphere firstObject(50, phongID, GL_FALSE, GL_TRUE, GL_FILL);
	Cone secondObject(15, programID);

	Object object("models/cube.obj", programID);

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		switch (screen) {
			case WIRE_CONE:
				glfwSetWindowTitle("Cone");
				cone.setUpDefaultMVP();
				cone.draw();
				break;

			case WIRE_SPHERE:
				glfwSetWindowTitle("Wire Sphere");
				wireSphere.setUpDefaultMVP();
				wireSphere.draw();
				break;

			case HEDGEHOG:
				glfwSetWindowTitle("Hedgehog");
				hedgehog.setUpDefaultMVP();
				hedgehog.draw();
				break;

			case SHADED_SPHERE:
				glfwSetWindowTitle("Shaded Sphere");	
				shadedSphere.setUpDefaultMVP();
				shadedSphere.draw();
				break;
			
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

