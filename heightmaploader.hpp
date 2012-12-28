#ifndef __HEIGHTMAPLOADER_HPP__
#define __HEIGHTMAPLOADER_HPP__

#include "gl_common.hpp"
#include "geometrygenerator.hpp"

class HeightMapLoader : public GeometryGenerator {
	public:
		HeightMapLoader(const char* height_map_dir, const char* file_name);

		virtual std::vector<glm::vec3> getVertices();
		virtual std::vector<glm::vec2> getTextureCoords();
		virtual std::vector<glm::vec3> getNormals();

		virtual Material* getMaterial();
};

#endif
