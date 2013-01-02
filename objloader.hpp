#ifndef __OBJLOADER_HPP__
#define __OBJLOADER_HPP__

#include "gl_common.hpp"

#include <vector>

//Local Includes
#include "utils.hpp"
#include "mtlloader.hpp"

#include "geometrygenerator.hpp"

#define DEBUG 1

class ObjLoader : public GeometryGenerator {
	private: 
		
		MtlLoader mtlLoader;

		GLuint currentMaterialNum;

		std::vector<std::vector<GLuint> > materialNums;
		std::vector<Material*> materials;

		const char* _models_dir;

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> textureCoords;
		std::vector<glm::vec3> normals;
		
		std::vector<std::vector<GLuint> > elements; 
		std::vector<std::vector<GLuint> > texture_refs;
		std::vector<std::vector<GLuint> > normal_refs; 
		
		void handle_face(char* line, int length);
		void handle_vector(char* line, int length, std::vector<glm::vec3> *vector);
		void handle_vertex(char* line, int length);
		void handle_texture_coord(char* line, int length);
		void handle_normal(char* line, int length);
		
		void handle_material_library(char* line, int length);
		void handle_use_material(char* line, int length);
		
		void process_line(char* line, int length);

	public:
		ObjLoader(const char* models_dir, const char* file_name);

		virtual std::vector<glm::vec3> getVertices();
		virtual std::vector<glm::vec2> getTextureCoords();
		virtual std::vector<glm::vec3> getNormals();

		virtual std::vector<Material*> getMaterials();
		virtual std::vector<GLuint> getMaterialNums();
};

#endif
