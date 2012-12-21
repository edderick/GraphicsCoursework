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

std::vector<glm::vec3> vertices;
std::vector<int> elements; 

void handle_vertex(char* line, int length){
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
	vertices.push_back(glm::vec3(x,y,z));
}

//Faces should always be triangles
void handle_face(char* line, int length){
	// Print out original file
	// for (int i = 0; i < length; i++) std::cout << line[i];
	
	// This code is essentially an FSM
	int state = 0;
	int v1, v2, v3;

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
				case (1): v1 = atoi(buf); break;
				case (2): v2 = atoi(buf); break;
				case (3): v3 = atoi(buf); break;
			}

			// Progress to next state
			state++;
			token = token + token_length;
			token_length = 0;
		}
		i++;
	}

	//Print Out X Y Z values for Vertex
	//std::cout << v1 << ", " << v2 << ", " << v3 << "\n";

	//Add triangle to elements list
	elements.push_back(v1); elements.push_back(v2); elements.push_back(v3);
}

void process_line(char* line, int length){
	//For now I am only concerned with vertices and faces
	switch(line[0]){
		case 'v': handle_vertex(line, length); break;
		case 'f': handle_face(line, length); break;
	}
}

int main(int argc, char *argv[]){

	char* obj_file = filetobuf("models/cube.obj");

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
	
	std::cout << "Characters: ";
	std::cout << i;
	
	std::cout << "\nLines: ";
	std::cout << n;

	std::cout << "\nVertices: ";
	std::cout << vertices.size();

	std::cout << "\nElements: ";
	std::cout << elements.size();

	std::cout << "\n";

	return 1;
}
