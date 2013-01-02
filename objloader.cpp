//Standard includes
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

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

#define NOISEY 0

template <typename T>
struct pointer_values_equal
{
	const T* to_find;

	bool operator()(const T* other) const
	{
		return *to_find == *other;
	}
};

void ObjLoader::handle_vector(char* line, int length,  std::vector<glm::vec3> *vector){
	// Print out original file
	//for (int i = 0; i < length; i++) std::cout << line[i];

	// This code is essentially an FSM
	int state = 0;
	double x, y, z;

	char* token = line;
	int token_length = 0;

	int i = 0;
	bool digit_seen = 0;

	while (i < length){
		token_length++;

		if ((line[i] == ' ' || line[i] == '\n') && digit_seen){
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
			digit_seen = 0;
		} else { 
			digit_seen++;
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
	int v1 = -1, v2 = -1, v3 = -1, v4 = -1;
	int t1 = -1, t2 = -1, t3 = -1, t4 = -1;
	int n1 = -1, n2 = -1, n3 = -1, n4 = -1;

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
						case (4): v4 = atoi(buf) - 1; break;
					}
					break;

				case(1):
					//Perform state exit action
					switch (state){
						case (0): break;
						case (1): t1 = atoi(buf) - 1; break;
						case (2): t2 = atoi(buf) - 1; break;
						case (3): t3 = atoi(buf) - 1; break;
						case (4): t4 = atoi(buf) - 1; break;
					}
					break;

				case(2):
					//Perform state exit action
					switch (state){
						case (0): break;
						case (1): n1 = atoi(buf) - 1; break;
						case (2): n2 = atoi(buf) - 1; break;
						case (3): n3 = atoi(buf) - 1; break;
						case (4): n4 = atoi(buf) - 1; break;
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
	elements[currentMaterialNum].push_back(v1); elements[currentMaterialNum].push_back(v2); elements[currentMaterialNum].push_back(v3);
	texture_refs[currentMaterialNum].push_back(t1); texture_refs[currentMaterialNum].push_back(t2); texture_refs[currentMaterialNum].push_back(t3);
	normal_refs[currentMaterialNum].push_back(n1); normal_refs[currentMaterialNum].push_back(n2); normal_refs[currentMaterialNum].push_back(n3);
	materialNums[currentMaterialNum].push_back(currentMaterialNum);materialNums[currentMaterialNum].push_back(currentMaterialNum);materialNums[currentMaterialNum].push_back(currentMaterialNum);

	if(v4 != -1){
		//v1 = tl; v2 = tr; v3 = bl; v4 = br
		elements[currentMaterialNum].push_back(v1); elements[currentMaterialNum].push_back(v3); elements[currentMaterialNum].push_back(v4);
		texture_refs[currentMaterialNum].push_back(t1); texture_refs[currentMaterialNum].push_back(t3); texture_refs[currentMaterialNum].push_back(t4);
		normal_refs[currentMaterialNum].push_back(n1); normal_refs[currentMaterialNum].push_back(n3); normal_refs[currentMaterialNum].push_back(n4);
		materialNums[currentMaterialNum].push_back(currentMaterialNum);materialNums[currentMaterialNum].push_back(currentMaterialNum);materialNums[currentMaterialNum].push_back(currentMaterialNum);
	}

}

void ObjLoader::handle_use_material(char* line, int length){
	char* head = strtok(line, " ");
	char* token = strtok(NULL, " \n");

	if(NOISEY) std::cout << "Using material: "<< token << "\n";
	Material* temp_material = mtlLoader.getMaterial(token);

	if(temp_material == NULL) std::cout << "Material Not found";
	else {
		std::vector<Material*>::iterator it;

		pointer_values_equal<Material> eq = { temp_material };

		it = std::find_if(materials.begin(), materials.end(), eq);

		if(it != materials.end()) {
			/* v contains x */
			currentMaterialNum = distance(materials.begin(), it);
		} else {
			/* v does not contain x */
			materials.push_back(temp_material);
			currentMaterialNum = materials.size() - 1;

			materialNums.resize(materials.size());
			elements.resize(materials.size());
			texture_refs.resize(materials.size());
			normal_refs.resize(materials.size());
		}


	}
	token[strlen(token)] = ' ';
}

void ObjLoader::handle_material_library(char* line, int length){
	char* token = strtok(line, " ");

	token = strtok(NULL, " \n");

	if(NOISEY) std::cout << "Loading: " << _models_dir << "/" << token << "\n";

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


ObjLoader::ObjLoader(const char* models_dir, const char* file_name){
	currentMaterialNum = -1;

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

	if (NOISEY) {
		std::cout << "Characters: " << i;
		std::cout << "\nLines: " << n;
		std::cout << "\nVertices: "  << vertices.size();
		std::cout << "\nElements: " << elements.size() << "\n";
	}
}

std::vector<glm::vec3> ObjLoader::getVertices(){
	std::vector<glm::vec3> vertex_list;

	for (int j = 0; j < elements.size(); j++){
	for (int i = 0; i < elements[j].size(); i++){
		if (elements[j][i] == -1) return std::vector<glm::vec3>();
		vertex_list.push_back(vertices[elements[j][i]]);
		//	std::cout << vertex_list[normal_refs[i]].x << ","<< vertex_list[normal_refs[i]].y << ","<< vertex_list[normal_refs[i]].z << "\n";
	}
	}
	return vertex_list;
}

std::vector<glm::vec3> ObjLoader::getNormals(){
	std::vector<glm::vec3> normal_list;
	
	for (int j = 0; j < normal_refs.size(); j++){
	for (int i = 0; i < normal_refs[j].size(); i++){
		if (normal_refs[j][i] == -1) return std::vector<glm::vec3>();
		normal_list.push_back(normals[normal_refs[j][i]]);
		//	std::cout << normals[normal_refs[i]].x << ","<< normals[normal_refs[i]].y << ","<< normals[normal_refs[i]].z << "\n";
	}
	}
	return normal_list;
}

std::vector<glm::vec2> ObjLoader::getTextureCoords(){
	std::vector<glm::vec2> texture_coords_list;
	for (int j = 0; j < normal_refs.size(); j++){
	for (int i = 0; i < normal_refs[j].size(); i++){
		if (texture_refs[j][i] == -1) return std::vector<glm::vec2>();
		texture_coords_list.push_back(glm::vec2(textureCoords[texture_refs[j][i]].x, textureCoords[texture_refs[j][i]].y));
		//	std::cout << textureCoords[texture_refs[i]]).x << ","<< textureCoords[normal_refs[i]].y << "\n";
	}
	}	
	return texture_coords_list;
}

std::vector<Material*> ObjLoader::getMaterials(){
	return materials;
}

std::vector<GLuint> ObjLoader::getMaterialNums(){
	std::vector<GLuint> result;

	for (int j= 0; j < materialNums.size(); j++){
		for (int i = 0; i < materialNums[j].size(); i++){
			result.push_back(materialNums[j][i]);
		}
	}

	return result;
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
