#include "heightmaploader.hpp"

HeightMapLoader::HeightMapLoader(const char* height_map_dir, const char* file_name) {

//Load in the file 

//Create a 2D array of the heights

//Create a triangle strip (best of just making plain triangles..)

//Texture coords should be simple? Just the percentage of the way across in a given direction....?

//Calculate triangle normals...

}

std::vector<glm::vec3> HeightMapLoader::getVertices(){} 
std::vector<glm::vec2> HeightMapLoader::getTextureCoords(){} 
std::vector<glm::vec3> HeightMapLoader::getNormals(){}

Material* HeightMapLoader::getMaterial(){

	//Just use mtlLoader

}

