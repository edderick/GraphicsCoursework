//Standard includes
#include <stdlib.h> 
#include <stdio.h> 
#include <stddef.h> 
#include <string.h> 
#include <iostream> 
#include <vector> 

//GL stuff 
#include <GL/glew.h> 
#include <GL/glfw.h> 

//GLM stuff 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

//Local Includes 
#include "utils.hpp" 


class Material {
	private:
		GLfloat Ns, Ni, d;
		glm::vec3 Ka, Kd, Ks;

		GLuint illum;

		//Map map_Kd;


};
