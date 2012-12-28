#include "gl_common.hpp"

//local includes
#include "utils.hpp"

#include "skybox.hpp"
#include "object.hpp"
#include "viewer.hpp" 

const GLuint WINDOW_WIDTH = 800;
const GLuint WINDOW_HEIGHT = 800;
const GLfloat PI = 3.14159; 

//Viewer is global as it's state is modified by key_callback
Viewer* viewer = new Viewer(); 

//Handle controls
void key_callback(int key, int action){
	if ((key == 'W' || key == 'w') && action == GLFW_PRESS){
		viewer->setForwardVelocity(1);
	} else if ((key == 'W' || key == 'w') && action == GLFW_RELEASE){
		viewer->setForwardVelocity(0);
	} else if ((key == 'S' || key == 's') && action == GLFW_PRESS){
		viewer->setForwardVelocity(-1);
	} else if ((key == 'S' || key == 's') && action == GLFW_RELEASE){
		viewer->setForwardVelocity(0);
	} else if ((key == 'A' || key == 'a') && action == GLFW_PRESS){
		viewer->setStrafeVelocity(-1);
	} else if ((key == 'A' || key == 'a') && action == GLFW_RELEASE){
		viewer->setStrafeVelocity(0);
	} else if ((key == 'D' || key == 'd') && action == GLFW_PRESS){
		viewer->setStrafeVelocity(1);
	} else if ((key == 'D' || key == 'd') && action == GLFW_RELEASE){
		viewer->setStrafeVelocity(0);
	} else if ((key == GLFW_KEY_UP) && action == GLFW_PRESS){
		viewer->changeVelocity(0,0,0.1);
	} else if ((key == GLFW_KEY_DOWN) && action == GLFW_PRESS){
		viewer->changeVelocity(0,0,-0.1);
	} else if ((key == GLFW_KEY_PAGEUP) && action == GLFW_PRESS){
		viewer->setUpVelocity(1);
	}  else if ((key == GLFW_KEY_PAGEUP) && action == GLFW_RELEASE){
		viewer->setUpVelocity(0);
	} else if ((key == GLFW_KEY_PAGEDOWN) && action == GLFW_PRESS){
		viewer->setUpVelocity(-1);
	} else if ((key == GLFW_KEY_PAGEDOWN) && action == GLFW_RELEASE){
		viewer->setUpVelocity(0);
	} else if ((key == GLFW_KEY_LEFT) && action == GLFW_PRESS){
		viewer->setCameraRotationVelocity(1);
	} else if ((key == GLFW_KEY_RIGHT) && action == GLFW_PRESS){
		viewer->setCameraRotationVelocity(-1);
	}  else if ((key == GLFW_KEY_LEFT) && action == GLFW_RELEASE){
		viewer->setCameraRotationVelocity(0);
	} else if ((key == GLFW_KEY_RIGHT) && action == GLFW_RELEASE){
		viewer->setCameraRotationVelocity(0);
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
	
	//Load in and set program (Shaders)
	GLuint simpleShaderID = setupShaders("shaders/simple/vert.gls", "shaders/simple/frag.gls");
	GLuint phongShaderID = setupShaders("shaders/phong/vert.gls", "shaders/phong/frag.gls");
	GLuint perFragmentShaderID = setupShaders("shaders/perFragment/vert.gls", "shaders/perFragment/frag.gls");
	
	//TODO sort out SkyBox so that this can be removed
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	ObjLoader prettyMonkey = ObjLoader("models","prettyMonkey.obj");
	ObjLoader normals = ObjLoader("models","normals.obj");

	Object object(&prettyMonkey, perFragmentShaderID, viewer, GL_FILL);
	Skybox skybox(&normals, perFragmentShaderID, viewer, GL_FILL);
	
	glfwSetWindowTitle("Mars In Fiction");	

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		object.draw();
		skybox.draw();

		glfwSwapBuffers();
	} while ( (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS) && 
			(glfwGetKey('q') != GLFW_PRESS) &&
			(glfwGetKey('Q') != GLFW_PRESS) &&
			(glfwGetWindowParam( GLFW_OPENED )) );

	glfwTerminate();

	//Everything was okay, return success
	exit(EXIT_SUCCESS);
}
