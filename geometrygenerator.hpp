#ifndef __GEOMETRYGENERATOR_HPP__
#define __GEOMETRYGENERATOR_HPP__

#include "gl_common.hpp"

#include <vector>

#include "material.hpp"

class GeometryGenerator {

	public:
		GeometryGenerator();

		virtual std::vector<glm::vec3> getVertices() = 0; 
		virtual std::vector<glm::vec2> getTextureCoords() = 0; 
		virtual std::vector<glm::vec3> getNormals() = 0;

		virtual Material* getMaterial() = 0;
};

#endif
