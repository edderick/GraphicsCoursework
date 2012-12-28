#include "skybox.hpp"

const GLfloat PI = 3.14159; 

Skybox::Skybox(const char* obj_file_name, GLuint programID, Viewer* viewer, GLenum draw_mode) {
	_programID = programID;
	_draw_mode = draw_mode;

	_viewer = viewer;

	_ambient_texture = -1;
	_diffuse_texture = -1;
	_specular_texture = -1;

	ObjLoader objLoader;
	//TODO magic word!!
	objLoader.load_model("models", obj_file_name);

	_vertices = objLoader.getVertices();
	_normals = objLoader.getNormals();
	_UVs = objLoader.getTextureCoords();

	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	glGenBuffers(1, &_vertex_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_vboID);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices[0]), &_vertices[0], GL_STATIC_DRAW); 

	glGenBuffers(1, &_normal_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _normal_vboID);
	glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(_normals[0]), &_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_UV_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _UV_vboID);
	glBufferData(GL_ARRAY_BUFFER, _UVs.size() * sizeof(_UVs[0]), &_UVs[0], GL_STATIC_DRAW);

	_material = objLoader.getMaterial();

	char* ambient_texture_file_name =  _material->getAmbientTexture();
	if(ambient_texture_file_name != NULL){
		//TEXTURE
		glActiveTexture(AMBIENT_TEXTURE);
		//glload must be initialized for glimg texture creation to work.
		if(glload::LoadFunctions() == glload::LS_LOAD_FAILED)
			std::cout << "Failed To Load";

		//Loading succeeded. Now load a texture.
		try
		{
			std::auto_ptr<glimg::ImageSet> pImgSet(glimg::loaders::stb::LoadFromFile(ambient_texture_file_name));
			_ambient_texture = glimg::CreateTexture(pImgSet.get(), 0);
		}
		catch(glimg::ImageCreationException  &e)
		{
			//Image file loading failed.
			std::cout << "IMAGE FILE LOADING FAILED";
		}
	}


	char* diffuse_texture_file_name = _material->getDiffuseTexture();
	if(diffuse_texture_file_name != NULL){
		//TEXTURE
		glActiveTexture(DIFFUSE_TEXTURE);
		//glload must be initialized for glimg texture creation to work.
		if(glload::LoadFunctions() == glload::LS_LOAD_FAILED)
			std::cout << "Failed To Load";

		//Loading succeeded. Now load a texture.
		try
		{
			std::auto_ptr<glimg::ImageSet> pImgSet(glimg::loaders::stb::LoadFromFile(diffuse_texture_file_name));
			_diffuse_texture = glimg::CreateTexture(pImgSet.get(), 0);
		}
		catch(glimg::ImageCreationException  &e)
		{
			//Image file loading failed.
			std::cout << "IMAGE FILE LOADING FAILED";
		}
	}

	char* specular_texture_file_name = _material->getSpecularTexture();
	if(specular_texture_file_name != NULL){
		//TEXTURE
		glActiveTexture(SPECULAR_TEXTURE);
		//glload must be initialized for glimg texture creation to work.
		if(glload::LoadFunctions() == glload::LS_LOAD_FAILED)
			std::cout << "Failed To Load";

		//Loading succeeded. Now load a texture.
		try
		{
			std::auto_ptr<glimg::ImageSet> pImgSet(glimg::loaders::stb::LoadFromFile(specular_texture_file_name));
			_specular_texture = glimg::CreateTexture(pImgSet.get(), 0);
		}
		catch(glimg::ImageCreationException  &e)
		{
			//Image file loading failed.
			std::cout << "IMAGE FILE LOADING FAILED";
		}
	}
}		

void Skybox::draw() {
	setUpTransformations();

	glUseProgram(_programID);

	glPolygonMode(GL_FRONT_AND_BACK, _draw_mode);

	glEnableVertexAttribArray(0);
	glBindAttribLocation(_programID, 0, "in_Positon");
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_vboID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glEnableVertexAttribArray(1);
	glBindAttribLocation(_programID, 1, "in_Normal");
	glBindBuffer(GL_ARRAY_BUFFER, _normal_vboID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glEnableVertexAttribArray(2);
	glBindAttribLocation(_programID, 2, "in_UV");
	glBindBuffer(GL_ARRAY_BUFFER, _UV_vboID);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);;

	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Skybox::setUpTransformations(){
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.f);

	glm::mat4 view = glm::mat4(1.f);

	_viewer->getPosition();

	view = glm::lookAt(glm::vec3(0,0,0),_viewer->getDirection(), _viewer->getUp());

	glm::mat4 model = glm::mat4(1.f);
	model = glm::rotate(model, (GLfloat) glfwGetTime() * 0.f, glm::vec3(0.f, 1.f, 0.f));
	//TODO Pick a posiition;
	model = glm::scale(model, glm::vec3(50,50,50));

	glm::mat4 mv = view * model;
	glm::mat4 mvp = projection * mv;

	glUniformMatrix4fv(glGetUniformLocation(_programID, "m"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(_programID, "v"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(_programID, "p"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_programID, "mv"), 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(glGetUniformLocation(_programID, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	if(_material != NULL){
		glUniform3fv(glGetUniformLocation(_programID, "in_ambient_color"), 1, glm::value_ptr(_material->getAmbientColor()));
		glUniform3fv(glGetUniformLocation(_programID, "in_diffuse_color"), 1, glm::value_ptr(_material->getDiffuseColor()));
		glUniform3fv(glGetUniformLocation(_programID, "in_specular_color"), 1, glm::value_ptr(_material->getSpecularColor()));
	}

	if(_ambient_texture != -1){
		glActiveTexture(AMBIENT_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, _ambient_texture);
		GLuint TextureID  = glGetUniformLocation(_programID, "AmbientSampler");
		glUniform1i(TextureID, AMBIENT_TEXTURE_NUM);
	}

	if(_diffuse_texture != -1){
		glActiveTexture(DIFFUSE_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, _diffuse_texture);
		GLuint TextureID  = glGetUniformLocation(_programID, "DiffuseSampler");
		glUniform1i(TextureID, DIFFUSE_TEXTURE_NUM);
	}

	if(_specular_texture != -1){
		glActiveTexture(SPECULAR_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, _specular_texture);
		GLuint TextureID  = glGetUniformLocation(_programID, "SpecularSampler");
		glUniform1i(TextureID, SPECULAR_TEXTURE_NUM);
	}
}

