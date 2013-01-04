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
	height = heights[0].size();
	width = heights.size();

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

	std::vector<glm::vec3> _face_normals;

	//Calculate triangle normals...
	for (int i = 0; i < _vertices.size(); i+= 3){
		glm::vec3 & a = _vertices[i+0];
		glm::vec3 & c = _vertices[i+1];
		glm::vec3 & b = _vertices[i+2];

		_face_normals.push_back(glm::normalize(glm::cross(c - a, b - a)));
	}

	//std::cout <<  " start " << width << " " << height ;

		for (int count = 0; count < _vertices.size(); count++){

			int i = _vertices[count].x;
			int j = _vertices[count].z;

			int h = height -1;

			int index1 = 2 * ((j-1) + (i-1)*h);
			int index2 = index1 + 1;

			int index3 = 2 * (j + (i-1)*h) + 1;
			int index4 = 2 * ((j-1) + i*h) + 1;

			int index5 = 2 *(j + i*h);
			int index6 = index5 + 1;

			glm::vec3 normal;

			if (index1 >= 0 && index1 < _face_normals.size()) normal = normal + _face_normals[index1];
			if (index2 >= 0 && index2 < _face_normals.size()) normal = normal + _face_normals[index2];
			if (index3 >= 0 && index3 < _face_normals.size()) normal = normal + _face_normals[index3];
			if (index4 >= 0 && index4 < _face_normals.size()) normal = normal + _face_normals[index4];
			if (index5 >= 0 && index5 < _face_normals.size()) normal = normal + _face_normals[index5];
			if (index6 >= 0 && index6 < _face_normals.size()) normal = normal + _face_normals[index6];
			
			_normals.push_back(glm::normalize(normal));
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

std::vector<Material*> HeightMapLoader::getMaterials(){
	MtlLoader mtl = MtlLoader();
	mtl.load_materials((char*)"models/ground.mtl");
	Material* m = mtl.getMaterial((char*)"Material_gnd.jpg");
	
	std::vector<Material*> v;
	v.push_back(m);

	return v;
	//Just use mtlLoader
}

std::vector<GLuint> HeightMapLoader::getMaterialNums(){
	std::vector<GLuint> v;
	
	for (int i = 0 ; i < _normals.size(); i++){
		v.push_back(0);
	}
	
	return v;
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
