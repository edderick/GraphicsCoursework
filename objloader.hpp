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

#define DEBUG 1

class ObjLoader {

	private: 
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		
		std::vector<GLuint> elements; 
		std::vector<GLuint> normal_refs; 
		
		void handle_face(char* line, int length);
		void handle_vector(char* line, int length, std::vector<glm::vec3> *vector);
		void handle_vertex(char* line, int length);
		void handle_normal(char* line, int length);
		void process_line(char* line, int length);

	public:
		//TODO Should this be the constructor?
		void load_model(const char* file_name);
		std::vector<glm::vec3> getVertices();
		std::vector<glm::vec3> getNormals();
		std::vector<GLuint> getElements();
};
