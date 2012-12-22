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
#include "objloader.hpp"

class Object {
	private:
		const char* _obj_file_name;
		
		GLuint _num_of_faces;
		
		GLuint _programID;

		GLuint _vaoID;  
		GLuint _vertex_vboID;
		GLuint _element_vboID;

		GLenum _draw_mode;
		
		std::vector<glm::vec3> _vertices;
		std::vector<GLuint> _elements;
	public:
		Object(const char* obj_file_name, GLuint programID, GLenum draw_mode = GL_LINE);
		void setUpDefaultMVP();
		void draw();
};

