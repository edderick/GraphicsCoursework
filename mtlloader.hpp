//Standard includes
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <utility>

//GL stuff
#include <GL/glew.h>
#include <GL/glfw.h>

//GLM stuff
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Local Includes
#include "utils.hpp"

#if !defined( MATERIAL_HPP )
#define MATERIAL_HPP
#include "material.hpp"
#endif

#define DEBUG 1

struct ltstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};


class MtlLoader {
	private: 
		Material* material;
		std::map<const char*, Material*, ltstr> materials;

		void process_line(char* line, int length);

		void handle_new_material(char* line, int length);

		void handle_ambient_color(char* line, int length);
		void handle_diffuse_color(char* line, int length);
		void handle_specular_color(char* line, int length);

		void handle_illumination(char* line, int length);
		void handle_transparency(char* line, int length);
		void handle_specularity(char* line, int length);
		void handle_optical_density(char* line, int length);

		void handle_ambient_texture(char* line, int length);
		void handle_diffuse_texture(char* line, int length);
		void handle_specular_texture(char* line, int length);

	public:
		//TODO Should this be the constructor?
		void load_materials(char* file_name);
		Material* getMaterial(char* material_name);
} ;
