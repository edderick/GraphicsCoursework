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

class Cone {
	private:
		int _num_of_faces;
		GLuint _programID;

		GLuint _vaoID; //? 
		GLuint _base_vboID;
		GLuint _cone_vboID;

		GLenum _draw_mode;
	public:
		Cone(int num_of_faces, GLuint programID, GLenum draw_mode = GL_LINE);
		void setUpDefaultMVP();
		void draw();
};

