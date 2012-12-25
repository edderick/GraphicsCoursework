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
#include "material.hpp"

#define DEBUG 1

class MtlLoader {
	private: 
		std::map<const char*, Material*> materials;
		void process_line(char* line, int length);
	public:
		//TODO Should this be the constructor?
		void load_materials(const char* file_name);
		Material* getMaterial(const char* material_name);
} ;
