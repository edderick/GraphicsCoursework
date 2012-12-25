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


void MtlLoader::process_line(char* line, int length){
	//For now I am only concerned with vertices and faces
	switch(line[0]){
		case 'v': switch(line[1]) { 
		} break;
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

#ifdef OLTEST
int main(int argc, char *argv[]){
	MtlLoader ml;

	return 1;
}
#endif
