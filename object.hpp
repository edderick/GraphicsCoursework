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

class Object {
	private:
		const char* _obj_file_name;
		
		GLuint _num_of_faces;
		
		GLuint _programID;

		GLuint _vaoID; //? 
		GLuint _base_vboID;
		GLuint _cone_vboID;

		GLenum _draw_mode;
	public:
		Object(const char* obj_file_name, GLuint programID, GLenum draw_mode = GL_LINE);
		void setUpDefaultMVP();
		void draw();
};

