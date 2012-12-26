//Standard includes
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>

//GL stuff
#include <GL/glew.h>
#include <GL/glfw.h>

//GLM stuff
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Local Includes
#include "utils.hpp"
#include "mtlloader.hpp"

#define DEBUG 1
void MtlLoader::handle_new_material(char* line, int length){
	//Don't know why the commented version won't work	
	char * token;// = strtok(line, " \n");
	token = strtok(NULL, " \n");
	
	std::cout << "Creating material: " << token << "\n";

	material = new Material();
	materials.insert( std::pair<const char*, Material*>(token, material));
}

void MtlLoader::process_line(char* line, int length){
	//YAGNI - Can add more things as they are needed!
	
	char * token = strtok(line, " ");

	//std::cout << token << ": ";

	if (strcmp("newmtl", token) == 0){
		handle_new_material(line, length);	
	} else if (strcmp("Ka", token) == 0){
		handle_ambient_color(line, length);
	} else if (strcmp("Kd", token) == 0){
		std::cout << "Diffuse Colour\n"; 
	} else if (strcmp("Ks", token) == 0){
		std::cout << "Specular Colour\n";
	} else if (strcmp("illum", token) == 0){
		std::cout << "Illumination\n";
	} else if ((strcmp("d", token) == 0) || (strcmp("Tr", token) == 0)){
		std::cout << "Transparency\n";
	} else if (strcmp("Ns", token) == 0){
		std::cout << "Specularity (Shininess)\n";
	} else if (strcmp("map_Ka", token) == 0){
		std::cout << "Ambient Texture Map\n";
	} else if (strcmp("map_Kd", token) == 0){
		std::cout << "Diffuse Texture Map\n";
	} else if (strcmp("map_Ks", token) == 0){
		std::cout << "Specular Texture Map\n";
	} else if (strcmp("Ni", token) == 0){
		std::cout << "Optical Density\n";
	}
	
}


void MtlLoader::load_materials(const char* file_name){
	char* mtl_file = filetobuf(file_name);

	int i = 0;
	int n = 0;
	
	int line_length = 0;
	char*  line_start = mtl_file;

	while (mtl_file[i] != 0){
		line_length++;
		
		if (mtl_file[i] == '\n'){
			process_line(line_start, line_length);
			line_start = line_start + line_length;
			line_length = 0;
			n++;
		}
		i++;
	}
	
	if (DEBUG) {
	}
}

#ifdef MTLTEST
int main(int argc, char *argv[]){
	MtlLoader ml;
	ml.load_materials("models/normals.mtl");
	return 1;
}
#endif
