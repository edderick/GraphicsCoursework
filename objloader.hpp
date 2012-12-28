#ifndef __OBJLOADER_HPP__
#define __OBJLOADER_HPP__

#include "gl_common.hpp"

#include <vector>

//Local Includes
#include "utils.hpp"
#include "mtlloader.hpp"

#define DEBUG 1

class ObjLoader {
	private: 
		
		MtlLoader mtlLoader;
		Material* material;

		const char* _models_dir;

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> textureCoords;
		std::vector<glm::vec3> normals;
		
		std::vector<GLuint> elements; 
		std::vector<GLuint> texture_refs;
		std::vector<GLuint> normal_refs; 
		
		void handle_face(char* line, int length);
		void handle_vector(char* line, int length, std::vector<glm::vec3> *vector);
		void handle_vertex(char* line, int length);
		void handle_texture_coord(char* line, int length);
		void handle_normal(char* line, int length);
		
		void handle_material_library(char* line, int length);
		void handle_use_material(char* line, int length);
		
		void process_line(char* line, int length);

	public:
		//TODO Should this be the constructor?
		void load_model(const char* models_dir, const char* file_name);

		std::vector<glm::vec3> getVertices();
		std::vector<glm::vec2> getTextureCoords();
		std::vector<glm::vec3> getNormals();

		Material* getMaterial();
};

#endif
