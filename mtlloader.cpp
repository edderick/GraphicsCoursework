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

void resetTok(char * token){
	token[strlen(token)] = ' ';
}

void MtlLoader::handle_new_material(char* line, int length){
	char * token = strtok(line, " \n");
	token = strtok(NULL, " \n");

	std::cout << "Creating material: " << token << "\n";

	material = new Material(token);
	materials.insert( std::pair<const char*, Material*>(token, material));
}

void MtlLoader::handle_ambient_color(char* line, int length){
	char * token = strtok(line, " \n");

	token = strtok(NULL, " \n");
	float r = atof(token);
	
	token = strtok(NULL, " \n");
	float g = atof(token);
	
	token = strtok(NULL, " \n");
	float b = atof(token);

	std::cout << "Setting ambient color: " << r << ", " << g << ", " << b << "\n";
	material->setAmbientColor(glm::vec3(r,g,b));
}

void MtlLoader::handle_diffuse_color(char* line, int length){
	char * token = strtok(line, " \n");

	token = strtok(NULL, " \n");
	float r = atof(token);

	token = strtok(NULL, " \n");
	float g = atof(token);
	
	token = strtok(NULL, " \n");
	float b = atof(token);

	std::cout << "Setting diffuse color: " << r << ", " << g << ", " << b << "\n";
	material->setDiffuseColor(glm::vec3(r,g,b));
}

void MtlLoader::handle_specular_color(char* line, int length){
	char * token = strtok(line, " \n");

	token = strtok(NULL, " \n");
	float r = atof(token);

	token = strtok(NULL, " \n");
	float g = atof(token);
	
	token = strtok(NULL, " \n");
	float b = atof(token);

	std::cout << "Setting specular color: " << r << ", " << g << ", " << b << "\n";
	material->setSpecularColor(glm::vec3(r,g,b));
}

void MtlLoader::handle_illumination(char* line, int length){
	char * token = strtok(line, " \n");

	token = strtok(NULL, " \n");
	GLuint i = atoi(token);

	std::cout << "Setting Illumination: " << i << "\n";
	material->setIllumination(i);
}


void MtlLoader::handle_transparency(char* line, int length){
	char * token = strtok(line, " \n");

	token = strtok(NULL, " \n");
	GLfloat Tr = atof(token);

	std::cout << "Setting transparency: " << Tr << "\n";
	material->setTransparency(Tr);
}

void MtlLoader::handle_specularity(char* line, int length){
	char * token = strtok(line, " \n");

	token = strtok(NULL, " \n");
	GLfloat Ns = atof(token);

	std::cout << "Setting Specularity: " << Ns << "\n";
	material->setSpecularity(Ns);
}

void MtlLoader::handle_optical_density(char* line, int length){
	char * token = strtok(line, " \n");

	token = strtok(NULL, " \n");
	GLfloat Ni = atof(token);

	std::cout << "Setting Optical Density: " << Ni << "\n";
	material->setOpticalDensity(Ni);
}

void MtlLoader::handle_ambient_texture(char* line, int length){
	char * token = strtok(line, " \n");

	token = strtok(NULL, " \n");

	std::cout << "Setting Ambient Texture: " << token << "\n";
	material->setAmbientTexture(token);
}

void MtlLoader::handle_diffuse_texture(char* line, int length){
	char * token = strtok(line, " \n");

	token = strtok(NULL, " \n");

	std::cout << "Setting Diffuse Texture: " << token << "\n";
	material->setDiffuseTexture(token);
}

void MtlLoader::handle_specular_texture(char* line, int length){
	char * token = strtok(line, " \n");

	token = strtok(NULL, " \n");

	std::cout << "Setting Specular Texture: " << token << "\n";
	material->setSpecularTexture(token);
}

Material* MtlLoader::getMaterial(char* material_name){
	return materials[material_name];
}


void MtlLoader::process_line(char* line, int length){
	//YAGNI - Can add more things as they are needed!
	
	char * token = strtok(line, " ");

	//std::cout << token << ": ";

	if (strcmp("newmtl", token) == 0){
		resetTok(token);
		handle_new_material(line, length);	
	} else if (strcmp("Ka", token) == 0){
		resetTok(token);
		handle_ambient_color(line, length);
	} else if (strcmp("Kd", token) == 0){
		resetTok(token);
		handle_diffuse_color(line, length);
	} else if (strcmp("Ks", token) == 0){
		resetTok(token);
		handle_specular_color(line, length);
	} else if (strcmp("illum", token) == 0){
		resetTok(token);
		handle_illumination(line, length);
	} else if ((strcmp("d", token) == 0) || (strcmp("Tr", token) == 0)){
		resetTok(token);
		handle_transparency(line, length);
	} else if (strcmp("Ns", token) == 0){
		resetTok(token);
		handle_specularity(line, length);
	} else if (strcmp("Ni", token) == 0){
		resetTok(token);
		handle_optical_density(line, length);
	} else if (strcmp("map_Ka", token) == 0){
		resetTok(token);
		handle_ambient_texture(line, length);
	} else if (strcmp("map_Kd", token) == 0){
		resetTok(token);
		handle_diffuse_texture(line, length);
	} else if (strcmp("map_Ks", token) == 0){
		resetTok(token);
		handle_specular_texture(line, length);
	} 	
}


void MtlLoader::load_materials(char* file_name){
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
	ml.load_materials((char*)"models/normals.mtl");
	return 1;
}
#endif
