#include "object.hpp"

const GLfloat PI = 3.14159; 

Object::Object(GeometryGenerator* gg, GLuint programID, Viewer* viewer, GLenum draw_mode) {
	_programID = programID;
	_draw_mode = draw_mode;

	_viewer = viewer;

	_vertices = gg->getVertices();
	_normals = gg->getNormals();
	_UVs = gg->getTextureCoords();

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

	_material = gg->getMaterial();

	_position = glm::vec3(0,0,0);
	_scale = glm::vec3(1,1,1);
	//_scale = glm::vec3(0.1,0.01,0.1);
	_rotation_axis = glm::vec3(1,0,0);
	_rotation_magnitude = 0;


	_ambient_texture_num = 0;
	_diffuse_texture_num = 1;
	_specular_texture_num = 2;
	
	_ambient_texture = setUpTexture(_material->getAmbientTexture(), _ambient_texture_num);//, "AmbientSampler");
	_diffuse_texture = setUpTexture(_material->getDiffuseTexture(), _diffuse_texture_num);//, "DiffuseSampler");
	_specular_texture = setUpTexture(_material->getSpecularTexture(), _specular_texture_num);//, "SpecularSampler");
}		

void Object::draw() {
	setUpTransformations();
	setUpMaterials();

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


glm::mat4 Object::makeModelMatrix(){
	glm::mat4 model = glm::mat4(1.f);
	model = glm::scale(model, _scale);
	model = glm::rotate(model, _rotation_magnitude, _rotation_axis);
	model = glm::translate(model, _position);
	return model;
}

glm::mat4 Object::makeViewMatrix(){
	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(_viewer->getPosition(),_viewer->getLookAt(), _viewer->getUp());
	return view;
}

glm::mat4 Object::makeProjectionMatrix(){
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 200.f);
	return projection;
}

void Object::setUpTransformations(){
	//Allow the generation of the matricies to be overriden
	glm::mat4 projection = makeProjectionMatrix();
	glm::mat4 view = makeViewMatrix();
	glm::mat4 model = makeModelMatrix();

	//Calculating once here should be faster than calculating for each vertex
	glm::mat4 mv = view * model;
	glm::mat4 mvp = projection * mv;

	glUniformMatrix4fv(glGetUniformLocation(_programID, "m"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(_programID, "v"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(_programID, "p"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(_programID, "mv"), 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(glGetUniformLocation(_programID, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	//TODO Create Lightsource class and possibly multiple lightsources
	std::vector<glm::vec4> lightsources;
	lightsources.push_back(glm::vec4(0,10,0,1));

	glUniform1i(glGetUniformLocation(_programID, "num_of_light_sources"), lightsources.size());
	glUniform4fv(glGetUniformLocation(_programID, "light_position"), lightsources.size(), &lightsources[0][0]);

}	

void Object::setUpMaterials(){
	_ambient_mode = 0;
	_diffuse_mode = 0;
	_specular_mode = 0;

	if(_material != NULL){

		glm::vec3  ambient_color = _material->getAmbientColor();
		glm::vec3  diffuse_color = _material->getDiffuseColor();
		glm::vec3  specular_color = _material->getSpecularColor();

		if(ambient_color != glm::vec3(-1,-1,-1)){
			glUniform3fv(glGetUniformLocation(_programID, "in_ambient_color"), 1, glm::value_ptr(ambient_color));
			_ambient_mode = 1;
		}

		if(diffuse_color != glm::vec3(-1,-1,-1)){
			glUniform3fv(glGetUniformLocation(_programID, "in_diffuse_color"), 1, glm::value_ptr(diffuse_color));
			_diffuse_mode = 1;
		}

		if(specular_color != glm::vec3(-1,-1,-1)){
			glUniform3fv(glGetUniformLocation(_programID, "in_specular_color"), 1, glm::value_ptr(specular_color));
			_specular_mode = 1;
		}
	}

	if(_ambient_texture != -1){ 
		_ambient_mode = 2;
		useTexture(_ambient_texture, _ambient_texture_num, "AmbientSampler");
	}

	if(_diffuse_texture != -1){
		_diffuse_mode = 2;
		useTexture(_diffuse_texture, _diffuse_texture_num, "DiffuseSampler");
	}

	if(_specular_texture != -1){
		_specular_texture = 2;
		useTexture(_specular_texture, _diffuse_texture_num, "SpecularSampler");
	}

	glUniform1i(glGetUniformLocation(_programID, "ambient_mode"), _ambient_mode);
	glUniform1i(glGetUniformLocation(_programID, "diffuse_mode"), _diffuse_mode);
	glUniform1i(glGetUniformLocation(_programID, "specular_mode"), _specular_mode);

}

GLuint Object::setUpTexture(char* texture_file_name, GLuint ActiveTextureNum) {
	//-1 is "error" case
	GLuint textureID = -1;

	if(texture_file_name != NULL){

		//TEXTURE
		glActiveTexture(GL_TEXTURE0 + ActiveTextureNum);
		//glload must be initialized for glimg texture creation to work.
		if(glload::LoadFunctions() == glload::LS_LOAD_FAILED)
			std::cout << "Failed To Load";

		//Loading succeeded. Now load a texture.
		try
		{
			//TODO cache image...
			std::auto_ptr<glimg::ImageSet> pImgSet(glimg::loaders::stb::LoadFromFile(texture_file_name));
			textureID = glimg::CreateTexture(pImgSet.get(), 0);
		}
		catch(glimg::ImageCreationException  &e)
		{
			//Image file loading failed.
			std::cout << "Image creation failed";
		}
	}



	return textureID;
}

void Object::useTexture(GLuint textureID, GLuint ActiveTextureNum, const char* SamplerName) {
	if(textureID != -1){
		glActiveTexture(GL_TEXTURE0 + ActiveTextureNum);
		glBindTexture(GL_TEXTURE_2D, textureID);
		GLuint TextureID  = glGetUniformLocation(_programID, SamplerName);
		glUniform1i(TextureID, ActiveTextureNum);
	}

}
