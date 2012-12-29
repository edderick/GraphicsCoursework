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

	_ambient_texture = setUpTexture(_material->getAmbientTexture(), AMBIENT_TEXTURE, AMBIENT_TEXTURE_NUM, "AmbientSampler");
	_diffuse_texture = setUpTexture(_material->getDiffuseTexture(), DIFFUSE_TEXTURE, DIFFUSE_TEXTURE_NUM, "DiffuseSampler");
	_specular_texture = setUpTexture(_material->getSpecularTexture(), SPECULAR_TEXTURE, SPECULAR_TEXTURE_NUM, "SpecularSampler");

	if(_material != NULL){
		glUniform3fv(glGetUniformLocation(_programID, "in_ambient_color"), 1, glm::value_ptr(_material->getAmbientColor()));
		glUniform3fv(glGetUniformLocation(_programID, "in_diffuse_color"), 1, glm::value_ptr(_material->getDiffuseColor()));
		glUniform3fv(glGetUniformLocation(_programID, "in_specular_color"), 1, glm::value_ptr(_material->getSpecularColor()));
	}

	_position = glm::vec3(0,0,0);
	_scale = glm::vec3(1,1,1);
	//_scale = glm::vec3(0.1,0.01,0.1);
	_rotation_axis = glm::vec3(1,0,0);
	_rotation_magnitude = 0;
}		

void Object::draw() {
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
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.f);
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
}

GLuint Object::setUpTexture(char* texture_file_name, GLuint ActiveTexture, GLuint ActiveTextureNum, const char* SamplerName) {
	//-1 is "error" case
	GLuint textureID = -1;

	if(texture_file_name != NULL){
		//TEXTURE
		glActiveTexture(ActiveTexture);
		//glload must be initialized for glimg texture creation to work.
		if(glload::LoadFunctions() == glload::LS_LOAD_FAILED)
			std::cout << "Failed To Load";

		//Loading succeeded. Now load a texture.
		try
		{
			std::auto_ptr<glimg::ImageSet> pImgSet(glimg::loaders::stb::LoadFromFile(texture_file_name));
			textureID = glimg::CreateTexture(pImgSet.get(), 0);
		}
		catch(glimg::ImageCreationException  &e)
		{
			//Image file loading failed.
			std::cout << "Image creation failed";
		}
	}

	if(textureID != -1){
		glActiveTexture(ActiveTexture);
		glBindTexture(GL_TEXTURE_2D, textureID);
		GLuint TextureID  = glGetUniformLocation(_programID, SamplerName);
		glUniform1i(TextureID, ActiveTextureNum);
	}

	return textureID;
}
