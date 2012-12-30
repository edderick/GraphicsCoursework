#include "heightmaploader.hpp"

HeightMapLoader::HeightMapLoader(const char* height_map_dir, const char* file_name) {
	
	//TODO Collapse these loops into one?

	char path[255];
	strcpy(path, height_map_dir);
	strcat(path, "/");
	strcat(path, file_name);


	std::vector<std::vector<unsigned char> > heights;

	//Load in the file 
	//glload must be initialized for glimg texture creation to work. 
	if(glload::LoadFunctions() == glload::LS_LOAD_FAILED) 
		std::cout << "Failed To Load"; 

	//Loading succeeded. Now load a texture. 
	try 
	{ 
		std::auto_ptr<glimg::ImageSet> pImgSet(glimg::loaders::stb::LoadFromFile(path));
		glimg::SingleImage img = pImgSet->GetImage(0);

		int width = img.GetDimensions().width;
		int height = img.GetDimensions().height;

		heights.resize(width);
		for (int i = 0; i < width; i++){
			heights[i].resize(height);
			for (int j = 0; j < height; j++){
				unsigned char x =  ((char*) img.GetImageData())[j * width + i] ;
				heights[i][j] = x;
			}
		}


	} catch(glimg::ImageCreationException  &e) {
		//Image file loading failed.
		std::cout << "Image creation failed";
		return;
	}


	//Create a 2D array of the heights

	//Create a triangle strip (best of just making plain triangles..)
	int height = heights[0].size();
	int width = heights.size();

	float MULT = 1;

	for (int i = 0; i < width - 1; i++){
		for (int j = 0; j < height - 1; j++){
			_vertices.push_back(glm::vec3(i,heights[i][j] * MULT, j));
			_vertices.push_back(glm::vec3(i, heights[i][j+1] * MULT, j+1));
			_vertices.push_back(glm::vec3(i+1, heights[i+1][j] * MULT, j));

			_vertices.push_back(glm::vec3(i, heights[i][j+1]* MULT, j+1));
			_vertices.push_back(glm::vec3(i+1, heights[i+1][j+1] *MULT, j+1));	
			_vertices.push_back(glm::vec3(i+1, heights[i+1][j]* MULT, j));
			//std::cout << heights[i][j] * MULT << " ";
		}
	}

	//Texture coords should be simple? Just the percentage of the way across in a given direction....?

	for (int i = 0; i < width - 1; i++){
		for (int j = 0; j < height - 1; j++){
			_tex_coords.push_back(glm::vec2(0, 0));
			_tex_coords.push_back(glm::vec2(1, 0));
			_tex_coords.push_back(glm::vec2(0, 1));

			_tex_coords.push_back(glm::vec2(0, 1));
			_tex_coords.push_back(glm::vec2(1, 1));
			_tex_coords.push_back(glm::vec2(1, 0));	
		}
	}

	//Calculate triangle normals...
	for (int i = 0; i < _vertices.size(); i+= 3){
		glm::vec3 & a = _vertices[i+0];
		glm::vec3 & c = _vertices[i+1];
		glm::vec3 & b = _vertices[i+2];

		_normals.push_back(glm::normalize(glm::cross(c - a, b - a)));
		_normals.push_back(glm::normalize(glm::cross(c - a, b - a)));
		_normals.push_back(glm::normalize(glm::cross(c - a, b - a)));
	}

}

std::vector<glm::vec3> HeightMapLoader::getVertices(){
	return _vertices;
} 
std::vector<glm::vec2> HeightMapLoader::getTextureCoords(){
	return _tex_coords;
} 
std::vector<glm::vec3> HeightMapLoader::getNormals(){
	return _normals;
}

Material* HeightMapLoader::getMaterial(){

	MtlLoader mtl = MtlLoader();
	mtl.load_materials((char*)"models/ground.mtl");
	return mtl.getMaterial((char*)"Material_gnd.jpg");

	//Just use mtlLoader

}

#ifdef HMTEST
int main(int args, char** lol){





	//Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "There was a problem initializing glfw");
		exit(EXIT_FAILURE);
	}

	//glfw hints
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Open a window
	if (!glfwOpenWindow(0, 0, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
		fprintf(stderr, "There was a problem opening a window");
		exit(EXIT_FAILURE);
	}

	//Turn on experimental features to prevent seg fault
	glewExperimental = GL_TRUE;

	//Intitialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "There was a problem initialising GLEW");
		exit(EXIT_FAILURE);
	}

	HeightMapLoader hml("textures", "img2.jpg");

	return 1;
}
#endif
