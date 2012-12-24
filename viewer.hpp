//GL stuff  
#include <GL/glew.h>  
#include <GL/glfw.h>  
 
//GLM stuff  
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>  

//Standard includes  
#include <stdlib.h>  
#include <stdio.h>  
#include <stddef.h>  
#include <string.h>  
#include <iostream>

//local includes  
#include "utils.hpp" 

class Viewer {
	private:
		glm::vec3 _position;
		glm::vec3 _direction;
		glm::vec3 _up;
		
		glm::vec3 _velocity;
		
		GLfloat _lastAccessedTime;

	public:
		Viewer();

		void move(GLfloat dx, GLfloat dy, GLfloat dz);
		
		void setVelocity(GLfloat ax, GLfloat ay, GLfloat az);
		void changeVelocity(GLfloat dax, GLfloat day, GLfloat daz);

		void rotateCamera(GLfloat angle);

		glm::vec3 getPosition();
		glm::vec3 getLookAt();
		glm::vec3 getUp();
};
