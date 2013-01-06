#include "gl_common.hpp"

#include <sstream>

//local includes
#include "utils.hpp"

#include "skybox.hpp"
#include "object.hpp"
#include "viewer.hpp" 

#include "heightmaploader.hpp"
#include "textgenerator.hpp"

#include "tour.hpp"

const GLfloat PI = 3.14159; 

const char* TITLE = "Mars in Fiction";

bool show_help = 0;
bool tour = 0;

//Viewer is global as it's state is modified by key_callback
Viewer* viewer = new Viewer(); 
Tour t(viewer, &tour);

int loading_count = 0;

void loading(TextGenerator *tg){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	char  string[100];
	strcpy(string, "Loading");
	for (int i = 0; i < loading_count; i++){
		strcat(string, ".");
	}

	
	tg->printText((char*)"Please wait", 10, WINDOW_HEIGHT - 90, 35);
	tg->printText(string, 10, WINDOW_HEIGHT - 50, 50);
	glEnable(GL_DEPTH_TEST);
	glfwSwapBuffers();

	loading_count++;
}

//Handle controls
void key_callback(int key, int action){
	if(!tour) {
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
			viewer->changeVelocity(0,0,0.2);
		} else if ((key == GLFW_KEY_DOWN) && action == GLFW_PRESS){
			viewer->changeVelocity(0,0,-0.2);
		} else if ((key == GLFW_KEY_PAGEUP) && action == GLFW_PRESS){
			viewer->setElevationVelocity(1);
		}  else if ((key == GLFW_KEY_PAGEUP) && action == GLFW_RELEASE){
			viewer->setElevationVelocity(0);
		} else if ((key == GLFW_KEY_PAGEDOWN) && action == GLFW_PRESS){
			viewer->setElevationVelocity(-1);
		} else if ((key == GLFW_KEY_PAGEDOWN) && action == GLFW_RELEASE){
			viewer->setElevationVelocity(0);
		} else if ((key == GLFW_KEY_LEFT) && action == GLFW_PRESS){
			viewer->setCameraRotationVelocity(2);
		} else if ((key == GLFW_KEY_RIGHT) && action == GLFW_PRESS){
			viewer->setCameraRotationVelocity(-2);
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
			viewer->resetElevation();
			viewer->gotoLocation(glm::vec3(13.8057, 0.3, 0.657926), glm::vec3(-6.42059,0.25837,9.794014) - glm::vec3(13.8057, 0.3, 0.657926) );
		} else if ((key == 'T' || key == 't') && action == GLFW_PRESS){
			tour = 1;

			//Stop any motion
			viewer->setCameraRotationVelocity(0);
			viewer->setVelocity(0,0,0);

			t.restart();
		}

	} else {
		if((key == 'E' || key == 'e') && action == GLFW_PRESS){
			tour = 0;
		}
	}
	if ((key == 'H' || key == 'h') && action == GLFW_PRESS){
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
	glfwSetWindowTitle(TITLE);	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	
	//Create text generator
	GLuint textShaderID = setupShaders("shaders/text/vert.gls", "shaders/text/frag.gls");
	TextGenerator tg((char*)"textures/font.png", textShaderID, ' ', '~', 16, 8, WINDOW_WIDTH, WINDOW_HEIGHT);
	loading(&tg);

	//Load in and set program (Shaders)
	// GLuint simpleShaderID = setupShaders("shaders/simple/vert.gls", "shaders/simple/frag.gls");
	// GLuint phongShaderID = setupShaders("shaders/phong/vert.gls", "shaders/phong/frag.gls");
	GLuint perFragmentShaderID = setupShaders("shaders/perFragment/vert.gls", "shaders/perFragment/frag.gls");

	//Load in terrain and sky box
	HeightMapLoader groundObj = HeightMapLoader("textures", "img2.png");
	ObjLoader skyboxObj = ObjLoader("models","skybox.obj");

	Object ground(&groundObj, perFragmentShaderID, viewer, GL_FILL);
	Skybox skybox(&skyboxObj, perFragmentShaderID, viewer, GL_FILL);
	loading(&tg);

	//Load in thunder birds
	ObjLoader thunderBird1Obj = ObjLoader("models", "thunderbird1.obj");
	ObjLoader t2ContainerObj = ObjLoader("models","t2container.obj");
	ObjLoader thunderBird2Obj = ObjLoader("models","thunderbird2.obj");
	ObjLoader thunderBird3Obj = ObjLoader("models","thunderbird3.obj");
	ObjLoader eggObj = ObjLoader("models", "egg.obj");

	Object thunderBird1(&thunderBird1Obj, perFragmentShaderID, viewer, GL_FILL);
	Object thunderBird1B(&thunderBird1Obj, perFragmentShaderID, viewer, GL_FILL);
	Object thunderBird1C(&thunderBird1Obj, perFragmentShaderID, viewer, GL_FILL);
	
	Object t2container(&t2ContainerObj, perFragmentShaderID, viewer, GL_FILL);
	Object thunderBird2(&thunderBird2Obj, perFragmentShaderID, viewer, GL_FILL);
	Object t2containerB(&t2ContainerObj, perFragmentShaderID, viewer, GL_FILL);
	Object thunderBird2B(&thunderBird2Obj, perFragmentShaderID, viewer, GL_FILL);
	
	Object thunderBird3(&thunderBird3Obj, perFragmentShaderID, viewer, GL_FILL);
	Object thunderBird3B(&thunderBird3Obj, perFragmentShaderID, viewer, GL_FILL);
	Object thunderBird3C(&thunderBird3Obj, perFragmentShaderID, viewer, GL_FILL);
	loading(&tg);

	std::vector<Object*> eggs;
	int number_of_eggs = 4 * 10;
	for (int i = 0; i < number_of_eggs; i++){
		eggs.push_back(new Object(&eggObj, perFragmentShaderID, viewer, GL_FILL));
	}

	//Set up collisions
	viewer->addTerrain(&ground);
	for (unsigned int i = 0; i < eggs.size(); i++){
		viewer->addCollidesWith(eggs[i]);
	}	
	
	glm::vec3 eggsGroup1Position(1.2,0.225,6.5);

	eggs[0]->setPosition(eggsGroup1Position + glm::vec3(0,0.05,0));
	eggs[0]->setRadius(0.2);
	eggs[0]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[0]->setRotation(50, glm::vec3(0,1,1));

	eggs[1]->setPosition(eggsGroup1Position + glm::vec3(0.2,0.05,0));
	eggs[1]->setRadius(0.2);
	eggs[1]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[1]->setRotation(0, glm::vec3(0,1,1));

	eggs[2]->setPosition(eggsGroup1Position + glm::vec3(0,0.05,0.2));
	eggs[2]->setRadius(0.2);
	eggs[2]->setScale(glm::vec3(0.08,0.08,0.08));
	eggs[2]->setRotation(0, glm::vec3(1,1,0));

	eggs[3]->setPosition(eggsGroup1Position + glm::vec3(0.1,0.05,-0.2));
	eggs[3]->setRadius(0.2);
	eggs[3]->setScale(glm::vec3(0.115,0.115,0.115));
	eggs[3]->setRotation(-25, glm::vec3(1,0,0));

	
	glm::vec3 eggsGroup2Position(-3,0.2,8);
	
	eggs[4]->setPosition(eggsGroup2Position + glm::vec3(0,0.05,0));
	eggs[4]->setRadius(0.2);
	eggs[4]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[4]->setRotation(50, glm::vec3(0,1,1));

	eggs[5]->setPosition(eggsGroup2Position + glm::vec3(0.2,0.05,0));
	eggs[5]->setRadius(0.2);
	eggs[5]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[5]->setRotation(0, glm::vec3(0,1,1));

	eggs[6]->setPosition(eggsGroup2Position + glm::vec3(0,0.05,0.2));
	eggs[6]->setRadius(0.2);
	eggs[6]->setScale(glm::vec3(0.08,0.08,0.08));
	eggs[6]->setRotation(0, glm::vec3(1,1,0));

	eggs[7]->setPosition(eggsGroup2Position + glm::vec3(0.1,0.05,-0.2));
	eggs[7]->setRadius(0.2);
	eggs[7]->setScale(glm::vec3(0.115,0.115,0.115));
	eggs[7]->setRotation(-25, glm::vec3(1,0,0));

	glm::vec3 eggsGroup3Position(-6.90139, 0.2, -14.7571);
	
	eggs[8]->setPosition(eggsGroup3Position + glm::vec3(0,0.05,0));
	eggs[8]->setRadius(0.2);
	eggs[8]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[8]->setRotation(50, glm::vec3(0,1,1));

	eggs[9]->setPosition(eggsGroup3Position + glm::vec3(0.2,0.05,0));
	eggs[9]->setRadius(0.2);
	eggs[9]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[9]->setRotation(0, glm::vec3(0,1,1));

	eggs[10]->setPosition(eggsGroup3Position + glm::vec3(0,0.05,0.2));
	eggs[10]->setRadius(0.2);
	eggs[10]->setScale(glm::vec3(0.08,0.08,0.08));
	eggs[10]->setRotation(0, glm::vec3(1,1,0));

	eggs[11]->setPosition(eggsGroup3Position + glm::vec3(0.1,0.05,-0.2));
	eggs[11]->setRadius(0.2);
	eggs[11]->setScale(glm::vec3(0.115,0.115,0.115));
	eggs[11]->setRotation(-25, glm::vec3(1,0,0));

	glm::vec3 eggsGroup4Position(-21.112, 0.54, -0.519565);
	
	eggs[12]->setPosition(eggsGroup4Position + glm::vec3(0,0.05,0));
	eggs[12]->setRadius(0.2);
	eggs[12]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[12]->setRotation(50, glm::vec3(0,1,1));

	eggs[13]->setPosition(eggsGroup4Position + glm::vec3(0.2,0.01,0));
	eggs[13]->setRadius(0.2);
	eggs[13]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[13]->setRotation(0, glm::vec3(0,1,1));

	eggs[14]->setPosition(eggsGroup4Position + glm::vec3(0,0.05,0.2));
	eggs[14]->setRadius(0.2);
	eggs[14]->setScale(glm::vec3(0.08,0.08,0.08));
	eggs[14]->setRotation(0, glm::vec3(1,1,0));

	eggs[15]->setPosition(eggsGroup4Position + glm::vec3(0.1,0.05,-0.2));
	eggs[15]->setRadius(0.2);
	eggs[15]->setScale(glm::vec3(0.115,0.115,0.115));
	eggs[15]->setRotation(-25, glm::vec3(1,0,0));

	glm::vec3 eggsGroup5Position(-11.3236, 0.233333, 20.3212);
	
	eggs[16]->setPosition(eggsGroup5Position + glm::vec3(0,0.05,0));
	eggs[16]->setRadius(0.2);
	eggs[16]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[16]->setRotation(50, glm::vec3(0,1,1));

	eggs[17]->setPosition(eggsGroup5Position + glm::vec3(0.2,0.01,0));
	eggs[17]->setRadius(0.2);
	eggs[17]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[17]->setRotation(0, glm::vec3(0,1,1));

	eggs[18]->setPosition(eggsGroup5Position + glm::vec3(0,0.05,0.2));
	eggs[18]->setRadius(0.2);
	eggs[18]->setScale(glm::vec3(0.08,0.08,0.08));
	eggs[18]->setRotation(0, glm::vec3(1,1,0));

	eggs[19]->setPosition(eggsGroup5Position + glm::vec3(0.1,0.05,-0.2));
	eggs[19]->setRadius(0.2);
	eggs[19]->setScale(glm::vec3(0.115,0.115,0.115));
	eggs[19]->setRotation(-25, glm::vec3(1,0,0));

	glm::vec3 eggsGroup6Position(15.4433, 0.225, 30.8257);
	
	eggs[20]->setPosition(eggsGroup6Position + glm::vec3(0,0.05,0));
	eggs[20]->setRadius(0.2);
	eggs[20]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[20]->setRotation(50, glm::vec3(0,1,1));

	eggs[21]->setPosition(eggsGroup6Position + glm::vec3(0.2,0.01,0));
	eggs[21]->setRadius(0.2);
	eggs[21]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[21]->setRotation(0, glm::vec3(0,1,1));

	eggs[22]->setPosition(eggsGroup6Position + glm::vec3(0,0.05,0.2));
	eggs[22]->setRadius(0.2);
	eggs[22]->setScale(glm::vec3(0.08,0.08,0.08));
	eggs[22]->setRotation(0, glm::vec3(1,1,0));

	eggs[23]->setPosition(eggsGroup6Position + glm::vec3(0.1,0.05,-0.2));
	eggs[23]->setRadius(0.2);
	eggs[23]->setScale(glm::vec3(0.115,0.115,0.115));
	eggs[23]->setRotation(-25, glm::vec3(1,0,0));

	glm::vec3 eggsGroup7Position(24.1003, 0.191667, 2.19043);
	
	eggs[24]->setPosition(eggsGroup7Position + glm::vec3(0,0.05,0));
	eggs[24]->setRadius(0.2);
	eggs[24]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[24]->setRotation(50, glm::vec3(0,1,1));

	eggs[25]->setPosition(eggsGroup7Position + glm::vec3(0.2,0.01,0));
	eggs[25]->setRadius(0.2);
	eggs[25]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[25]->setRotation(0, glm::vec3(0,1,1));

	eggs[26]->setPosition(eggsGroup7Position + glm::vec3(0,0.05,0.2));
	eggs[26]->setRadius(0.2);
	eggs[26]->setScale(glm::vec3(0.08,0.08,0.08));
	eggs[26]->setRotation(0, glm::vec3(1,1,0));

	eggs[27]->setPosition(eggsGroup7Position + glm::vec3(0.1,0.05,-0.2));
	eggs[27]->setRadius(0.2);
	eggs[27]->setScale(glm::vec3(0.115,0.115,0.115));
	eggs[27]->setRotation(-25, glm::vec3(1,0,0));

	glm::vec3 eggsGroup8Position(36.8265, 0.25, -30.415);
	
	eggs[28]->setPosition(eggsGroup8Position + glm::vec3(0,0.05,0));
	eggs[28]->setRadius(0.2);
	eggs[28]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[28]->setRotation(50, glm::vec3(0,1,1));

	eggs[29]->setPosition(eggsGroup8Position + glm::vec3(0.2,0.01,0));
	eggs[29]->setRadius(0.2);
	eggs[29]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[29]->setRotation(0, glm::vec3(0,1,1));

	eggs[30]->setPosition(eggsGroup8Position + glm::vec3(0,0.05,0.2));
	eggs[30]->setRadius(0.2);
	eggs[30]->setScale(glm::vec3(0.08,0.08,0.08));
	eggs[30]->setRotation(0, glm::vec3(1,1,0));

	eggs[31]->setPosition(eggsGroup8Position + glm::vec3(0.1,0.05,-0.2));
	eggs[31]->setRadius(0.2);
	eggs[31]->setScale(glm::vec3(0.115,0.115,0.115));
	eggs[31]->setRotation(-25, glm::vec3(1,0,0));

	glm::vec3 eggsGroup9Position(-7.79755, 0.1, -30.9879);
	
	eggs[32]->setPosition(eggsGroup9Position + glm::vec3(0,0.05,0));
	eggs[32]->setRadius(0.2);
	eggs[32]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[32]->setRotation(50, glm::vec3(0,1,1));

	eggs[33]->setPosition(eggsGroup9Position + glm::vec3(0.2,0.01,0));
	eggs[33]->setRadius(0.2);
	eggs[33]->setScale(glm::vec3(0.1,0.1,0.1));
	eggs[33]->setRotation(0, glm::vec3(0,1,1));

	eggs[34]->setPosition(eggsGroup9Position + glm::vec3(0,0.05,0.2));
	eggs[34]->setRadius(0.2);
	eggs[34]->setScale(glm::vec3(0.08,0.08,0.08));
	eggs[34]->setRotation(0, glm::vec3(1,1,0));

	eggs[35]->setPosition(eggsGroup9Position + glm::vec3(0.1,0.05,-0.2));
	eggs[35]->setRadius(0.2);
	eggs[35]->setScale(glm::vec3(0.115,0.115,0.115));
	eggs[35]->setRotation(-25, glm::vec3(1,0,0));

	glm::vec3 eggsGroupAPosition(-35.7026, 0.1, -5.2669);
	
	eggs[36]->setPosition(eggsGroupAPosition + glm::vec3(0,0.05,0));
	eggs[36]->setRadius(0.3);
	eggs[36]->setScale(glm::vec3(0.35,0.35,0.35));
	eggs[36]->setRotation(50, glm::vec3(0,1,1));

	eggs[37]->setPosition(eggsGroupAPosition + glm::vec3(0.45,0.01,0));
	eggs[37]->setRadius(0.4);
	eggs[37]->setScale(glm::vec3(0.35,0.35,0.35));
	eggs[37]->setRotation(0, glm::vec3(0,1,1));

	eggs[38]->setPosition(eggsGroupAPosition + glm::vec3(0,0.05,0.45));
	eggs[38]->setRadius(0.4);
	eggs[38]->setScale(glm::vec3(0.3,0.3,0.3));
	eggs[38]->setRotation(3, glm::vec3(1,1,0));

	eggs[39]->setPosition(eggsGroupAPosition + glm::vec3(0.25,0.05,-0.45));
	eggs[39]->setRadius(0.4);
	eggs[39]->setScale(glm::vec3(0.4,0.4,0.4));
	eggs[39]->setRotation(-25, glm::vec3(1,0,0));

	loading(&tg);

	thunderBird1.setPosition(glm::vec3(-2,0.1,0));

	t2container.setPosition(glm::vec3(-7.5,0.15,5));
	t2container.setScale(glm::vec3(0.2,0.2,0.2));
	t2container.setRotation(90, glm::vec3(0,1,0));

	thunderBird2.setPosition(glm::vec3(-7.5,0.15,5));
	thunderBird2.setScale(glm::vec3(0.2,0.2,0.2));
	thunderBird2.setRotation(90, glm::vec3(0,1,0));

	t2containerB.setPosition(glm::vec3(-13.7941, 0.0, -11.716));
	t2containerB.setScale(glm::vec3(0.2,0.2,0.2));
	t2containerB.setRotation(120, glm::vec3(0,1,0));

	thunderBird2B.setPosition(glm::vec3(-13.7941, 0.0, -11.716));
	thunderBird2B.setScale(glm::vec3(0.2,0.2,0.2));
	thunderBird2B.setRotation(120, glm::vec3(0,1,0));
	
	
	thunderBird3.setPosition(glm::vec3(2,2,12.5));
	thunderBird3.setScale(glm::vec3(0.4,0.4,0.4));
	thunderBird3.setRotation(-90, glm::vec3(1,0,0));

	thunderBird3B.setPosition(glm::vec3(22.3989, 0.541667, -5.00961));
	thunderBird3B.setScale(glm::vec3(0.4,0.4,0.4));
	thunderBird3B.setRotation(-90, glm::vec3(1,0,0));
	
	thunderBird3B.setPosition(glm::vec3(2,2,12.5));
	thunderBird3B.setScale(glm::vec3(0.4,0.4,0.4));
	thunderBird3B.setRotation(-90, glm::vec3(1,0,0));


	ground.setScale(glm::vec3(0.35,0.025,0.35));
	ground.setPosition(glm::vec3(-groundObj.width/2 * 0.35,0,-groundObj.height/2 * 0.35));

	std::vector<glm::vec3> positions;

	positions.push_back(glm::vec3(-6.42059,0.25837,4.494014));
	positions.push_back(glm::vec3(-3.85007,0.1,7.12377));
	positions.push_back(glm::vec3(-0.601204,0.1,3.592329));
	positions.push_back(glm::vec3(8.2538,0.1,2.5428));
	positions.push_back(glm::vec3(10.6809,0.38337,-3.84069));
	positions.push_back(glm::vec3(-5.13897,0.1,-11.4451));
	positions.push_back(glm::vec3(-14.3374,0.1,-8.11471));
	positions.push_back(glm::vec3(-20.2777, 0.1, 8.26625));
	positions.push_back(glm::vec3(-22.3929, 1.35833,19.6362));
	positions.push_back(glm::vec3(1.64004, 0.1, 33.6688));
	positions.push_back(glm::vec3(30.9513, 0.683333, 18.214));
	positions.push_back(glm::vec3(32.9145, 1.56667, -11.0445));
	positions.push_back(glm::vec3(31.17666, 0.808333, -22.8479));
	positions.push_back(glm::vec3(-29.7966, 0.35, -27.9126));
	positions.push_back(glm::vec3(-33.6018, 0.433333, -25.1177));
	positions.push_back(glm::vec3(-29.1175, 2.39167, -17.2737));
	positions.push_back(glm::vec3(-27.6434, 2.1833, -14.5711));
	positions.push_back(glm::vec3(-33.0654, 3.5, 2.62864));
	positions.push_back(glm::vec3(-20.6909, 4, 3.49496));
	positions.push_back(glm::vec3(-3.85007, 3.5 , 7.12377));
	positions.push_back(glm::vec3(1.42417, 2.116667, 0.276567));
	


	WayPoint w1 = WayPoint(positions[0], positions[1] - positions[0]);
	WayPoint w2 = WayPoint(positions[1], positions[2] - positions[1]);
	WayPoint w3 = WayPoint(positions[2], positions[3] - positions[2]);
	WayPoint w4 = WayPoint(positions[3], positions[4] - positions[3]);
	WayPoint w5 = WayPoint(positions[4], positions[5] - positions[4]);
	WayPoint w6 = WayPoint(positions[5], positions[6] - positions[5]);
	WayPoint w7 = WayPoint(positions[6], positions[7] - positions[6]);
	WayPoint w8 = WayPoint(positions[7], positions[8] - positions[7]);
	WayPoint w9 = WayPoint(positions[8], positions[9] - positions[8]);
	WayPoint w10 = WayPoint(positions[9], positions[10] - positions[9]);
	WayPoint w11 = WayPoint(positions[10], positions[11] - positions[10]);
	WayPoint w12 = WayPoint(positions[11], positions[12] - positions[11]);
	WayPoint w13 = WayPoint(positions[12], positions[13] - positions[12]);
	WayPoint w14 = WayPoint(positions[13], positions[14] - positions[13]);
	WayPoint w15 = WayPoint(positions[14], positions[15] - positions[14]);
	WayPoint w16 = WayPoint(positions[15], positions[16] - positions[15]);
	WayPoint w17 = WayPoint(positions[16], positions[17] - positions[16]);
	WayPoint w18 = WayPoint(positions[17], positions[18] - positions[17]);
	WayPoint w19 = WayPoint(positions[18], positions[19] - positions[18]);
	WayPoint w20 = WayPoint(positions[19], positions[20] - positions[19]);
	WayPoint w21 = WayPoint(positions[20], glm::vec3(0,0,1));
	
	t.addWayPoint(0.0, &w1, 1);
	t.addWayPoint(2.0, &w2, 1);
	t.addWayPoint(4.0, &w3, 1);
	t.addWayPoint(6.0, &w4, 1);
	t.addWayPoint(8.0, &w5, 1);
	t.addWayPoint(10.0, &w6, 1);
	t.addWayPoint(12.0, &w7, 1);
	t.addWayPoint(14.0, &w8, 1);
	t.addWayPoint(16.0, &w9, 1);
	t.addWayPoint(18.0, &w10, 1);
	t.addWayPoint(20.0, &w11, 1);
	t.addWayPoint(22.0, &w12, 1);
	t.addWayPoint(24.0, &w13, 1);
	t.addWayPoint(26.0, &w14, 1);
	t.addWayPoint(28.0, &w15, 1);
	t.addWayPoint(30.0, &w16, 1);
	t.addWayPoint(32.0, &w17, 1);
	t.addWayPoint(34.0, &w18, 1);
	t.addWayPoint(36.0, &w19, 1);
	t.addWayPoint(38.0, &w20, 1);
	t.addWayPoint(40.0, &w21, 1);

	Animutator* tb1Anim = new Animutator();

	KeyFrame tb1KF1 = KeyFrame(glm::vec3(0,1,1), 90, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1KF2 = KeyFrame(glm::vec3(10,1,1), 90, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1KF3 = KeyFrame(glm::vec3(10,1,1), -90, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5)); 
	KeyFrame tb1KF4 = KeyFrame(glm::vec3(0,1,1), -90, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));

	tb1Anim->addKeyFrame(0.0, &tb1KF1);
	tb1Anim->addKeyFrame(10.0, &tb1KF2);
	tb1Anim->addKeyFrame(12.5, &tb1KF3);
	tb1Anim->addKeyFrame(22.5, &tb1KF4);
	tb1Anim->addKeyFrame(25.0, &tb1KF1);

	thunderBird1.addAnimutator(tb1Anim);


	Animutator* tb1BAnim = new Animutator();

	KeyFrame tb1BKF1 = KeyFrame(glm::vec3(-23.027, 1.35, 16.35409), -110.328, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1BKF2 = KeyFrame(glm::vec3(-24.5907, 2.29167, 15.7748), -110.328, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1BKF3 = KeyFrame(glm::vec3(-24.5907, 2.29167, 15.7748), -38.604, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1BKF4 = KeyFrame(glm::vec3(-32.497, 1, 25.6773), -38.604, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5)); 
	KeyFrame tb1BKF5 = KeyFrame(glm::vec3(-32.497, 1, 25.6773), 5.98309, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5)); 
	KeyFrame tb1BKF6 = KeyFrame(glm::vec3(-31.8602, 1.55833, 31.7533), 5.98309, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1BKF7 = KeyFrame(glm::vec3(-31.8602, 1.55833, 31.7533), 38.384, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1BKF8 = KeyFrame(glm::vec3(-28.5349, 1, 35.9512), 38.384, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1BKF9 = KeyFrame(glm::vec3(-28.5349, 1, 35.9512), 99.0798, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1BKF10 = KeyFrame(glm::vec3(-13.2714, 1, 33.5119), 99.0798, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1BKF11 = KeyFrame(glm::vec3(-13.2714, 1, 33.5119), -150.378, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1BKF12 = KeyFrame(glm::vec3(-23.027, 1.35, 16.35409), -150.378, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	
	tb1BAnim->addKeyFrame(0.0, &tb1BKF1);
	tb1BAnim->addKeyFrame(5.0, &tb1BKF2);
	tb1BAnim->addKeyFrame(5.5, &tb1BKF3);
	tb1BAnim->addKeyFrame(10.5, &tb1BKF4);
	tb1BAnim->addKeyFrame(11.0, &tb1BKF5);
	tb1BAnim->addKeyFrame(16.0, &tb1BKF6);
	tb1BAnim->addKeyFrame(16.5, &tb1BKF7);
	tb1BAnim->addKeyFrame(21.5, &tb1BKF8);
	tb1BAnim->addKeyFrame(22.0, &tb1BKF9);
	tb1BAnim->addKeyFrame(27.0, &tb1BKF10);
	tb1BAnim->addKeyFrame(27.5, &tb1BKF11);
	tb1BAnim->addKeyFrame(32.5, &tb1BKF12);
	tb1BAnim->addKeyFrame(33.0, &tb1BKF1);
	
	thunderBird1B.addAnimutator(tb1BAnim);

	Animutator* tb1CAnim = new Animutator();

	KeyFrame tb1CKF1 = KeyFrame(glm::vec3(25.7527, 1.5, -29.4807), -77.7031, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1CKF2 = KeyFrame(glm::vec3(-24.549, 2.76667, -18.516), -77.7031, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));
	KeyFrame tb1CKF3 = KeyFrame(glm::vec3(-24.549, 2.76667, -18.516), 102.297, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5)); 
	KeyFrame tb1CKF4 = KeyFrame(glm::vec3(25.7527, 1.5, -29.4807), 102.297, glm::vec3(0,1,0), glm::vec3(0.5,0.5,0.5));

	tb1CAnim->addKeyFrame(0.0, &tb1CKF1);
	tb1CAnim->addKeyFrame(7.5, &tb1CKF2);
	tb1CAnim->addKeyFrame(9, &tb1CKF3);
	tb1CAnim->addKeyFrame(16.5, &tb1CKF4);
	tb1CAnim->addKeyFrame(18, &tb1CKF1);

	thunderBird1C.addAnimutator(tb1CAnim);


	Animutator* tb2cAnim = new Animutator();

	KeyFrame tbc2KF1 = KeyFrame(glm::vec3(-7.5, 0.15, 5), 90, glm::vec3(0,1,0), glm::vec3(0.2,0.2,0.2));
	KeyFrame tbc2KF2 = KeyFrame(glm::vec3(-7.5, -0.3, 5), 90, glm::vec3(0,1,0), glm::vec3(0.2,0.2,0.2));

	tb2cAnim->addKeyFrame(0.0, &tbc2KF1);
	tb2cAnim->addKeyFrame(5.0, &tbc2KF2);
	tb2cAnim->addKeyFrame(5.5, &tbc2KF2);
	tb2cAnim->addKeyFrame(10.0, &tbc2KF1);
	tb2cAnim->addKeyFrame(10.5, &tbc2KF1);

	t2container.addAnimutator(tb2cAnim);

	Animutator* tb2cBAnim = new Animutator();

	KeyFrame tbc2BKF1 = KeyFrame(glm::vec3(-13.7941, 0.0, -11.716), 120, glm::vec3(0,1,0), glm::vec3(0.2,0.2,0.2));
	KeyFrame tbc2BKF2 = KeyFrame(glm::vec3(-13.7941, -0.4, -11.716), 120, glm::vec3(0,1,0), glm::vec3(0.2,0.2,0.2));

	tb2cBAnim->addKeyFrame(0.0, &tbc2BKF1);
	tb2cBAnim->addKeyFrame(5.0, &tbc2BKF2);
	tb2cBAnim->addKeyFrame(5.5, &tbc2BKF2);
	tb2cBAnim->addKeyFrame(10.0, &tbc2BKF1);
	tb2cBAnim->addKeyFrame(10.5, &tbc2BKF1);

	t2containerB.addAnimutator(tb2cBAnim);


	Animutator* tb3cAnim = new Animutator();
	KeyFrame tb3KF1 = KeyFrame(glm::vec3(2, 1.90, 12.5), -90, glm::vec3(1,0,0), glm::vec3(0.4,0.4,0.4));
	KeyFrame tb3KF2 = KeyFrame(glm::vec3(2, 4, 12.5), -90, glm::vec3(1,0,0), glm::vec3(0.4,0.4,0.4));

	tb3cAnim->addKeyFrame(0.0, &tb3KF1);
	tb3cAnim->addKeyFrame(5.0, &tb3KF2);
	tb3cAnim->addKeyFrame(10.0, &tb3KF1);

	thunderBird3.addAnimutator(tb3cAnim);

	Animutator* tb3BcAnim = new Animutator();
	KeyFrame tb3BKF1 = KeyFrame(glm::vec3(22.3989, 2, -5.00961), -90, glm::vec3(1,0,0), glm::vec3(0.4,0.4,0.4));
	KeyFrame tb3BKF2 = KeyFrame(glm::vec3(22.3989, 4.5, -5.00961), -90, glm::vec3(1,0,0), glm::vec3(0.4,0.4,0.4));

	tb3BcAnim->addKeyFrame(0.0, &tb3BKF1);
	tb3BcAnim->addKeyFrame(5.0, &tb3BKF2);
	tb3BcAnim->addKeyFrame(10.0, &tb3BKF1);

	thunderBird3B.addAnimutator(tb3BcAnim);

	Animutator* tb3CcAnim = new Animutator();
	KeyFrame tb3CKF1 = KeyFrame(glm::vec3(-30.3314, 1.9, 2.7756), -90, glm::vec3(1,0,0), glm::vec3(0.4,0.4,0.4));
	KeyFrame tb3CKF2 = KeyFrame(glm::vec3(-30.3314, 4.6, 2.7756), -90, glm::vec3(1,0,0), glm::vec3(0.4,0.4,0.4));

	tb3CcAnim->addKeyFrame(0.0, &tb3CKF1);
	tb3CcAnim->addKeyFrame(5.0, &tb3CKF2);
	tb3CcAnim->addKeyFrame(10.0, &tb3CKF1);

	thunderBird3C.addAnimutator(tb3CcAnim);

	int count = 0;
	GLfloat lastTime = glfwGetTime();
	std::string s;

	loading(&tg);

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
		if(tour){
			t.update();
		} else {
			viewer->update();
		}
		
		thunderBird1.draw();
		thunderBird1B.draw();
		thunderBird1C.draw();
		
		thunderBird2.draw();
		t2container.draw();
		thunderBird2B.draw();
		t2containerB.draw();
		
		thunderBird3.draw();
		thunderBird3B.draw();
		thunderBird3C.draw();
		
	
		for (unsigned int i = 0; i < eggs.size(); i++){
			eggs[i]->draw();
		}	

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
			tg.printText( (char*)"ejfs1g10 - CW3", 10, 10, 50);
			
			tg.printText( (char*)"Esc, Q:", 10, 70, 23);
			tg.printText( (char*)"Exit", 280, 75, 23);

			tg.printText( (char*)"P:", 10, 100, 23);
			tg.printText( (char*)"Screenshot View Point", 280, 105, 23);
		
			tg.printText( (char*)"T:", 10, 130, 23);
			tg.printText( (char*)"Start Tour", 280, 135, 23);

			tg.printText( (char*)"E:", 10, 160, 23);
			tg.printText( (char*)"End Tour", 280, 165, 23);

			tg.printText( (char*)"Left/Right:", 10, 190, 23);
			tg.printText( (char*)"Turn Camera", 280, 195, 23);
		
			tg.printText( (char*)"Pg Up/Down:", 10, 220, 23);
			tg.printText( (char*)"Change Elevation", 280, 225, 23);
	
			tg.printText( (char*)"Up:", 10, 250, 23);
			tg.printText( (char*)"Accelerate", 280, 255, 23);
	
			tg.printText( (char*)"Down:", 10, 280, 23);
			tg.printText( (char*)"Decelerate", 280, 285, 23);
		
			tg.printText( (char*)"Space:", 10, 310, 23);
			tg.printText( (char*)"Stop Moving", 280, 315, 23);

			tg.printText( (char*)s.c_str() , 10, 710, 50);
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
