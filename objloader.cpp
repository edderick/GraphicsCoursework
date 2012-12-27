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
#include "objloader.hpp"

#define DEBUG 1

void ObjLoader::handle_vector(char* line, int length,  std::vector<glm::vec3> *vector){
	// Print out original file
	//for (int i = 0; i < length; i++) std::cout << line[i];
	
	// This code is essentially an FSM
	int state = 0;
	double x, y, z;

	char* token = line;
	int token_length = 0;
	
	int i = 0;
	while (i < length){
		token_length++;
		if (line[i] == ' ' || line[i] == '\n'){
			// Buffer and null terminate current token
			char* buf = new char[token_length + 1];
			for (int j = 0; j < token_length; j++) buf[j] = token[j];
			buf[token_length] = 0;
			
			//Perform state exit action
			switch (state){
				case (0): break;
				case (1): x = atof(buf); break;
				case (2): y = atof(buf); break;
				case (3): z = atof(buf); break;
			}

			// Progress to next state
			state++;
			token = token + token_length;
			token_length = 0;
		}
		i++;
	}

	//Print Out X Y Z values for Vertex
	//std::cout << x << ", " << y << ", " << z << "\n";

	//Create Vertex
	(*vector).push_back(glm::vec3(x,y,z));
}

void ObjLoader::handle_normal(char* line, int length){
	handle_vector(line, length, &normals);
}

void ObjLoader::handle_vertex(char* line, int length){
	handle_vector(line, length, &vertices);
}

void ObjLoader::handle_texture_coord(char* line, int length){
	handle_vector(line, length, &textureCoords);
}

//Faces should always be triangles
//Warning: Indexed from ONE!!!
void ObjLoader::handle_face(char* line, int length){
	// Print out original file
	// for (int i = 0; i < length; i++) std::cout << line[i];
	
	// This code is essentially an FSM
	int state = 0;
	//sub_state 0 - v, 1 - t, 2 - n
	int sub_state = 0;
	int v1 = -1, v2 = -1, v3 = -1;
	int t1 = -1, t2 = -1, t3 = -1;
	int n1 = -1, n2 = -1, n3 = -1;

	char* token = line;
	int token_length = 0;
	
	int i = 0;
	while (i < length){
		token_length++;
		if (line[i] == ' ' || line[i] == '\n' || line[i] == '/'){
			// Buffer and null terminate current token
			char* buf = new char[token_length + 1];
			for (int j = 0; j < token_length; j++) buf[j] = token[j];
			buf[token_length] = 0;
		
			switch (sub_state){
				case (0):
				//Perform state exit action
				switch (state){
					case (0): break;
					case (1): v1 = atoi(buf) - 1; break;
					case (2): v2 = atoi(buf) - 1; break;
					case (3): v3 = atoi(buf) - 1; break;
				}
				break;
	
				case(1):
				//Perform state exit action
				switch (state){
					case (0): break;
					case (1): t1 = atoi(buf) - 1; break;
					case (2): t2 = atoi(buf) - 1; break;
					case (3): t3 = atoi(buf) - 1; break;
				}
				break;

				case(2):
				//Perform state exit action
				switch (state){
					case (0): break;
					case (1): n1 = atoi(buf) - 1; break;
					case (2): n2 = atoi(buf) - 1; break;
					case (3): n3 = atoi(buf) - 1; break;
				}
				break;
			}

			// Progress to next state
			if(line[i] == '/') {
				sub_state++;
			} else {
				state++;
				sub_state = 0;
			}

			token = token + token_length;
			token_length = 0;
		}
		i++;
	}

	//Print Out X Y Z values for Vertex
	//std::cout << "v: " << v1 << ", " << v2 << ", " << v3 << "\n";
	//std::cout << "t: " << t1 << ", " << t2 << ", " << t3 << "\n";
	//std::cout << "n: " << n1 << ", " << n2 << ", " << n3 << "\n";

	//Add triangle to elements list
	elements.push_back(v1); elements.push_back(v2); elements.push_back(v3);
	texture_refs.push_back(t1); texture_refs.push_back(t2); texture_refs.push_back(t3);
	normal_refs.push_back(n1); normal_refs.push_back(n2); normal_refs.push_back(n3);
}

void ObjLoader::handle_use_material(char* line, int length){
	char* head = strtok(line, " ");
	char* token = strtok(NULL, " \n");

	std::cout << "Using material: "<< token << "\n";
	
	material = mtlLoader.getMaterial(token);

	if(material == NULL) std::cout << "Material Not found";

	token[strlen(token)] = ' ';
}

void ObjLoader::handle_material_library(char* line, int length){
	char* token = strtok(line, " ");

	token = strtok(NULL, " \n");
	
	std::cout << "Loading: " << _models_dir << "/" << token << "\n";

	char path[255];
	strcpy(path, _models_dir);
	strcat(path, "/");
	strcat(path, token);

	mtlLoader.load_materials(path);
}

void ObjLoader::process_line(char* line, int length){
	//For now I am only concerned with vertices and faces
	switch(line[0]){
		case 'v': switch(line[1]) { 
			case(' '): handle_vertex(line, length); break;
			case('n'): handle_normal(line, length); break;
			case('t'): handle_texture_coord(line, length); break;
					} break;
		case 'f': handle_face(line, length); break;
		case('u'): handle_use_material(line, length); break;
		case('m'): handle_material_library(line, length); break;

	}
}


void ObjLoader::load_model(const char* models_dir, const char* file_name){
	char path[255];
	strcpy(path, models_dir);
	strcat(path, "/");
	strcat(path, file_name);

	_models_dir = models_dir;

	char* obj_file = filetobuf(path);

	int i = 0;
	int n = 0;
	
	int line_length = 0;
	char*  line_start = obj_file;

	while (obj_file[i] != 0){
		line_length++;
		
		if (obj_file[i] == '\n'){
			process_line(line_start, line_length);
			line_start = line_start + line_length;
			line_length = 0;
			n++;
		}
		i++;
	}
	
	if (DEBUG) {
		std::cout << "Characters: " << i;
		std::cout << "\nLines: " << n;
		std::cout << "\nVertices: "  << vertices.size();
		std::cout << "\nElements: " << elements.size() << "\n";
	}
}

std::vector<glm::vec3> ObjLoader::getVertices(){
	std::vector<glm::vec3> vertex_list;

	for (int i = 0; i < elements.size(); i++){
		if (elements[i] == -1) return std::vector<glm::vec3>();
		vertex_list.push_back(vertices[elements[i]]);
	//	std::cout << vertex_list[normal_refs[i]].x << ","<< vertex_list[normal_refs[i]].y << ","<< vertex_list[normal_refs[i]].z << "\n";
	}
	
	return vertex_list;
}

std::vector<glm::vec3> ObjLoader::getNormals(){
	std::vector<glm::vec3> normal_list;

	for (int i = 0; i < normal_refs.size(); i++){
		if (normal_refs[i] == -1) return std::vector<glm::vec3>();
		normal_list.push_back(normals[normal_refs[i]]);
	//	std::cout << normals[normal_refs[i]].x << ","<< normals[normal_refs[i]].y << ","<< normals[normal_refs[i]].z << "\n";
	}
	
	return normal_list;
}

std::vector<glm::vec2> ObjLoader::getTextureCoords(){
	std::vector<glm::vec2> texture_coords_list;

	for (int i = 0; i < normal_refs.size(); i++){
		if (texture_refs[i] == -1) return std::vector<glm::vec2>();
		texture_coords_list.push_back(glm::vec2(textureCoords[texture_refs[i]].x, textureCoords[texture_refs[i]].y));
	//	std::cout << textureCoords[texture_refs[i]]).x << ","<< textureCoords[normal_refs[i]].y << "\n";
	}
	
	return texture_coords_list;
}

Material* ObjLoader::getMaterial(){
	return material;
}

#ifdef OLTEST
int main(int argc, char *argv[]){
	ObjLoader ol;
	ol.load_model("models","normals.obj");

	std::vector<glm::vec2> out = ol.getTextureCoords();

	//for (int i = 0; i < out.size(); i++){
	//	std::cout << out[i].x << ", "<< out[i].y << "\n"; //<< out[i].z << "\n";
	//}

	return 1;
}
#endif
