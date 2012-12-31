#include "gl_common.hpp"

#include <sstream>

//local includes
#include "utils.hpp"

#include "skybox.hpp"
#include "object.hpp"
#include "viewer.hpp" 

#include "heightmaploader.hpp"
#include "textgenerator.hpp"

const GLuint WINDOW_WIDTH = 800;
const GLuint WINDOW_HEIGHT = 800;
const GLfloat PI = 3.14159; 

bool show_help = 0;

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
	} else if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS){
		viewer->setCameraRotationVelocity(0);
		viewer->setVelocity(0,0,0);
	} else if ((key == 'P' || key == 'p') && action == GLFW_PRESS){
		viewer->setCameraRotationVelocity(0);
		viewer->setVelocity(0,0,0);
		viewer->gotoLocation(glm::vec3(0,0,0), glm::vec3(-1,0,-1));
	} else if ((key == 'H' || key == 'h') && action == GLFW_PRESS){
		show_help = !show_help;
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
	GLuint textShaderID = setupShaders("shaders/text/vert.gls", "shaders/text/frag.gls");
	
	//TODO sort out SkyBox so that this can be removed
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	//Load in obj files & height maps
	ObjLoader prettyMonkeyObj = ObjLoader("models","prettyMonkey.obj");
	HeightMapLoader groundObj = HeightMapLoader("textures", "img2.jpg");
	ObjLoader skyboxObj = ObjLoader("models","skybox.obj");

	//Create objects
	Object object(&prettyMonkeyObj, perFragmentShaderID, viewer, GL_FILL);
	Object ground(&groundObj, perFragmentShaderID, viewer, GL_FILL);
	Skybox skybox(&skyboxObj, perFragmentShaderID, viewer, GL_FILL);

	TextGenerator tg((char*)"textures/font.jpg", textShaderID, ' ', '~', 16, 8);

	viewer->addTerrain(&ground);
	viewer->addCollidesWith(&object);

	ground.setScale(glm::vec3(0.1,0.01,0.1));

	glfwSetWindowTitle("Mars In Fiction");	

	int count = 0;
	GLfloat lastTime = glfwGetTime();

std::string s;

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
		viewer->update();

		object.draw();
		ground.draw();
		skybox.draw();

		if(glfwGetTime() - lastTime < 1) {
			count++;
		} else {
			std::stringstream sstream;
			sstream << "FPS: " << count;
			s = sstream.str();
			count = 0;
			lastTime = glfwGetTime();
		}
		
		if(show_help){
			glDisable(GL_DEPTH_TEST);
			tg.printText( (char*)s.c_str() , 10, 10, 50);
			tg.printText( (char*)"EJFS1G10 - CW3", 10, 550, 55);
			glEnable(GL_DEPTH_TEST);
		}

		glfwSwapBuffers();
	} while ( (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS) && 
			(glfwGetKey('q') != GLFW_PRESS) &&
			(glfwGetKey('Q') != GLFW_PRESS) &&
			(glfwGetWindowParam( GLFW_OPENED )) );

	glfwTerminate();

	//Everything was okay, return success
	exit(EXIT_SUCCESS);
}
