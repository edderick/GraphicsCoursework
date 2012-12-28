#ifndef __MTLLOADER_HPP__
#define __MTLLOADER_HPP__

#include "gl_common.hpp"

#include <vector>
#include <map>
#include <utility>

//Local Includes
#include "utils.hpp"
#include "material.hpp"

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
};

#endif
