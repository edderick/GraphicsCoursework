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
Tour t(viewer);

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
			viewer->changeVelocity(0,0,0.1);
		} else if ((key == GLFW_KEY_DOWN) && action == GLFW_PRESS){
			viewer->changeVelocity(0,0,-0.1);
		} else if ((key == GLFW_KEY_PAGEUP) && action == GLFW_PRESS){
			viewer->setElevationVelocity(1);
		}  else if ((key == GLFW_KEY_PAGEUP) && action == GLFW_RELEASE){
			viewer->setElevationVelocity(0);
		} else if ((key == GLFW_KEY_PAGEDOWN) && action == GLFW_PRESS){
			viewer->setElevationVelocity(-1);
		} else if ((key == GLFW_KEY_PAGEDOWN) && action == GLFW_RELEASE){
			viewer->setElevationVelocity(0);
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
			viewer->gotoLocation(glm::vec3(13.8057, 0.3, 0.657926), glm::vec3(-1,0,-1));
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
	
	//Load in and set program (Shaders)
	// GLuint simpleShaderID = setupShaders("shaders/simple/vert.gls", "shaders/simple/frag.gls");
	// GLuint phongShaderID = setupShaders("shaders/phong/vert.gls", "shaders/phong/frag.gls");
	GLuint perFragmentShaderID = setupShaders("shaders/perFragment/vert.gls", "shaders/perFragment/frag.gls");
	GLuint textShaderID = setupShaders("shaders/text/vert.gls", "shaders/text/frag.gls");
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);


	//Load in terrain and sky box
	HeightMapLoader groundObj = HeightMapLoader("textures", "img2.png");
	ObjLoader skyboxObj = ObjLoader("models","skybox.obj");

	Object ground(&groundObj, perFragmentShaderID, viewer, GL_FILL);
	Skybox skybox(&skyboxObj, perFragmentShaderID, viewer, GL_FILL);
	

	//Load in thunder birds
	ObjLoader thunderBird1Obj = ObjLoader("models", "thunderbird1.obj");
	ObjLoader t2ContainerObj = ObjLoader("models","t2container.obj");
	ObjLoader thunderBird2Obj = ObjLoader("models","thunderbird2.obj");
	ObjLoader thunderBird3Obj = ObjLoader("models","thunderbird3.obj");
	ObjLoader eggObj = ObjLoader("models", "egg.obj");

	Object thunderBird1(&thunderBird1Obj, perFragmentShaderID, viewer, GL_FILL);
	Object t2container(&t2ContainerObj, perFragmentShaderID, viewer, GL_FILL);
	Object thunderBird2(&thunderBird2Obj, perFragmentShaderID, viewer, GL_FILL);
	Object thunderBird3(&thunderBird3Obj, perFragmentShaderID, viewer, GL_FILL);
	
	std::vector<Object*> eggs;
	int number_of_eggs = 4 * 4;
	for (int i = 0; i < number_of_eggs; i++){
		eggs.push_back(new Object(&eggObj, perFragmentShaderID, viewer, GL_FILL));
	}

	//Create text generator
	TextGenerator tg((char*)"textures/font.jpg", textShaderID, ' ', '~', 16, 8, WINDOW_WIDTH, WINDOW_HEIGHT);

	//Set up collisions
	viewer->addTerrain(&ground);
	for (int i = 0; i < eggs.size(); i++){
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


	thunderBird1.setPosition(glm::vec3(-2,0.1,0));

	t2container.setPosition(glm::vec3(-7.5,0.15,5));
	t2container.setScale(glm::vec3(0.2,0.2,0.2));
	t2container.setRotation(90, glm::vec3(0,1,0));

	thunderBird2.setPosition(glm::vec3(-7.5,0.15,5));
	thunderBird2.setScale(glm::vec3(0.2,0.2,0.2));
	thunderBird2.setRotation(90, glm::vec3(0,1,0));
	
	thunderBird3.setPosition(glm::vec3(2,2,12.5));
	thunderBird3.setScale(glm::vec3(0.4,0.4,0.4));
	thunderBird3.setRotation(-90, glm::vec3(1,0,0));


	ground.setScale(glm::vec3(0.35,0.025,0.35));
	ground.setPosition(glm::vec3(-groundObj.width/2 * 0.35,0,-groundObj.height/2 * 0.35));


	WayPoint w1 = WayPoint(glm::vec3(-6.42059,0.25837,4.494014), glm::vec3(1,0,0));
	WayPoint w2 = WayPoint(glm::vec3(-3.85007,0.1,7.12377), glm::vec3(0,0,1));
	WayPoint w3 = WayPoint(glm::vec3(-0.601204,0.1,3.592329), glm::vec3(1,0,1));
	WayPoint w4 = WayPoint(glm::vec3(8.2538,0.1,2.5428), glm::vec3(1,0,1));
	WayPoint w5 = WayPoint(glm::vec3(10.6809,0.38337,-3.84069), glm::vec3(1,0,1));
	WayPoint w6 = WayPoint(glm::vec3(-5.13897,0.1,-11.4451), glm::vec3(1,0,1));
	WayPoint w7 = WayPoint(glm::vec3(-14.3374,0.1,-8.11471), glm::vec3(1,0,1));

	t.addWayPoint(0.0, &w1, 0);
	t.addWayPoint(2.0, &w2, 0);
	t.addWayPoint(4.0, &w3, 0);
	t.addWayPoint(6.0, &w4, 0);
	t.addWayPoint(8.0, &w5, 0);
	t.addWayPoint(10.0, &w6, 0);
	t.addWayPoint(12.0, &w7, 0);

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

	Animutator* tb2cAnim = new Animutator();

	KeyFrame tbc2KF1 = KeyFrame(glm::vec3(-7.5, 0.15, 5), 90, glm::vec3(0,1,0), glm::vec3(0.2,0.2,0.2));
	KeyFrame tbc2KF2 = KeyFrame(glm::vec3(-7.5, -0.3, 5), 90, glm::vec3(0,1,0), glm::vec3(0.2,0.2,0.2));

	tb2cAnim->addKeyFrame(0.0, &tbc2KF1);
	tb2cAnim->addKeyFrame(5.0, &tbc2KF2);
	tb2cAnim->addKeyFrame(5.5, &tbc2KF2);
	tb2cAnim->addKeyFrame(10.0, &tbc2KF1);
	tb2cAnim->addKeyFrame(10.5, &tbc2KF1);

	t2container.addAnimutator(tb2cAnim);

	Animutator* tb3cAnim = new Animutator();
	KeyFrame tb3KF1 = KeyFrame(glm::vec3(2, 1.90, 12.5), -90, glm::vec3(1,0,0), glm::vec3(0.4,0.4,0.4));
	KeyFrame tb3KF2 = KeyFrame(glm::vec3(2, 4, 12.5), -90, glm::vec3(1,0,0), glm::vec3(0.4,0.4,0.4));

	tb3cAnim->addKeyFrame(0.0, &tb3KF1);
	tb3cAnim->addKeyFrame(5.0, &tb3KF2);
	tb3cAnim->addKeyFrame(10.0, &tb3KF1);

	thunderBird3.addAnimutator(tb3cAnim);

	int count = 0;
	GLfloat lastTime = glfwGetTime();
	std::string s;

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
		if(tour){
			t.update();
		} else {
			viewer->update();
		}
		
		thunderBird1.draw();
		thunderBird2.draw();
		t2container.draw();
		thunderBird3.draw();
	
		for (int i = 0; i < eggs.size(); i++){
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
