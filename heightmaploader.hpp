#ifndef __HEIGHTMAPLOADER_HPP__
#define __HEIGHTMAPLOADER_HPP__

#include "gl_common.hpp"
#include "geometrygenerator.hpp"

#include "mtlloader.hpp"
#include <vector>

class HeightMapLoader : public GeometryGenerator {
	public:
		std::vector<glm::vec3> _vertices;
		std::vector<glm::vec2> _tex_coords;
		std::vector<glm::vec3> _normals;
		
		HeightMapLoader(const char* height_map_dir, const char* file_name);

		virtual std::vector<glm::vec3> getVertices();
		virtual std::vector<glm::vec2> getTextureCoords();
		virtual std::vector<glm::vec3> getNormals();

		virtual std::vector<Material*> getMaterials();
		virtual std::vector<GLuint> getMaterialNums();
};

#endif
