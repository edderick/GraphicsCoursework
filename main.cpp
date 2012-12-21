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

const GLuint WINDOW_WIDTH = 800;
const GLuint WINDOW_HEIGHT = 800;
const GLfloat PI = 3.14159; 

typedef enum Screen{
	WIRE_SPHERE,
	WIRE_CONE,
	HEDGEHOG,
	SHADED_SPHERE,
	ANIMATION, 
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
	} else if ((key == 'E' || key == 'e') && action == GLFW_PRESS){
		screen = ANIMATION;
	} 
}





void setUpFirstObjectMVP(float k, GLuint _programID){

	glUseProgram(_programID);

	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.f);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(glm::vec3(0,0,-20), glm::vec3(0,0,0), glm::vec3(0,1,0));

	glm::mat4 model = glm::mat4(1.f);
	model = glm::rotate(model, 180.f , glm::vec3(0.f, 1.f, 1.f));
	//model = glm::rotate(model, (GLfloat) glfwGetTime() * 60.f, glm::vec3(1.f, 1.f, 1.f));
	model = glm::translate(model, glm::vec3(3 * sin(k), 0.f, -1.f));

	glUniformMatrix4fv(glGetUniformLocation(_programID, "m"), 1, GL_FALSE, glm::value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(_programID, "v"), 1, GL_FALSE, glm::value_ptr(view));

	glUniformMatrix4fv(glGetUniformLocation(_programID, "p"), 1, GL_FALSE, glm::value_ptr(projection));


}

void setUpSecondObjectMVP(float k, GLuint _programID){

	
	glUseProgram(_programID);


	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.f);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(glm::vec3(0,0,-5), glm::vec3(0,0,0), glm::vec3(0,1,0));
	
	glm::mat4 model = glm::mat4(1.f);
	//model = glm::rotate(model, 90.f , glm::vec3(0.f, 1.f, 0.f));
	model = glm::translate(model, glm::vec3(0.f, 5 * cos(k), 0.f));
	model = glm::rotate(model, 90.f, glm::vec3(1.f, 0.f,0.f));
	model = glm::rotate(model, (GLfloat) glfwGetTime() * 90.f, glm::vec3(0.f, 0.f, 1.f));
	
	glUniformMatrix4fv(glGetUniformLocation(_programID, "m"), 1, GL_FALSE, glm::value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(_programID, "v"), 1, GL_FALSE, glm::value_ptr(view));

	glUniformMatrix4fv(glGetUniformLocation(_programID, "p"), 1, GL_FALSE, glm::value_ptr(projection));




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
	GLuint programID = setupShaders("vert.gls", "frag.gls");
	GLuint phongID = setupShaders("phongVert.gls", "phongFrag.gls");

	//TODO: Consider removing...
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	Cone cone(15, programID);
	Sphere wireSphere(20, programID);
	Sphere hedgehog(20, programID, GL_TRUE);
	Sphere shadedSphere(50, phongID, GL_FALSE, GL_TRUE, GL_FILL);

	Sphere firstObject(50, phongID, GL_FALSE, GL_TRUE, GL_FILL);
	Cone secondObject(15, programID);

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

			case ANIMATION:
		
				glfwSetWindowTitle("Animation");
					
				setUpFirstObjectMVP(glfwGetTime(), phongID);
				firstObject.draw();

				setUpSecondObjectMVP(glfwGetTime(), programID);
				secondObject.draw();
				
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

