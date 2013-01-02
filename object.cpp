#include "object.hpp"

const GLfloat PI = 3.14159; 


std::vector<glm::vec3> _faceAvg;

Object::Object(GeometryGenerator* gg, GLuint programID, Viewer* viewer, GLenum draw_mode) {
	_programID = programID;
	_draw_mode = draw_mode;

	_viewer = viewer;

	_vertices = gg->getVertices();
	_normals = gg->getNormals();
	_UVs = gg->getTextureCoords();
	_material_nums = gg->getMaterialNums();

	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	//TODO confirm
	int start = 0;
	int count = 1;
	for (int i = 0; i < _material_nums.size() -1; i++){
		if(_material_nums[i] == _material_nums[i + 1]){
			count++;
		} else {
			group_start.push_back(start);
			group_length.push_back(count);
			count = 1;
			start = i + 1;
		}
	}
	group_start.push_back(start);
	group_length.push_back(count);

	_vertex_vboID.resize(group_start.size());
	_normal_vboID.resize(group_start.size());
	_UV_vboID.resize(group_start.size());
	_Material_Num_vboID.resize(group_start.size());
	
	for(int i = 0; i < group_start.size(); i++){
		glGenBuffers(1, &_vertex_vboID[i]);
		glBindBuffer(GL_ARRAY_BUFFER, _vertex_vboID[i]);
		glBufferData(GL_ARRAY_BUFFER, group_length[i] * sizeof(_vertices[0]), &_vertices[group_start[i]], GL_STATIC_DRAW); 

		glGenBuffers(1, &_normal_vboID[i]);
		glBindBuffer(GL_ARRAY_BUFFER, _normal_vboID[i]);
		glBufferData(GL_ARRAY_BUFFER,  group_length[i]  * sizeof(_normals[0]), &_normals[group_start[i]], GL_STATIC_DRAW);

		glGenBuffers(1, &_UV_vboID[i]);
		glBindBuffer(GL_ARRAY_BUFFER, _UV_vboID[i]);
		glBufferData(GL_ARRAY_BUFFER,  group_length[i]  * sizeof(_UVs[0]), &_UVs[group_start[i]], GL_STATIC_DRAW);

		glGenBuffers(1, &_Material_Num_vboID[i]);
		glBindBuffer(GL_ARRAY_BUFFER, _Material_Num_vboID[i]);
		glBufferData(GL_ARRAY_BUFFER,  group_length[i] * sizeof(_material_nums[0]), &_material_nums[group_start[i]], GL_STATIC_DRAW);
	}

	_materials = gg->getMaterials();

	_position = glm::vec3(0,0,0);
	_scale = glm::vec3(1,1,1);
	//_scale = glm::vec3(0.1,0.01,0.1);
	_rotation_axis = glm::vec3(1,0,0);
	_rotation_magnitude = 0;

	_ambient_mode.resize(_materials.size());
	_diffuse_mode.resize(_materials.size());
	_specular_mode.resize(_materials.size());


	for (int i = 0; i < _materials.size(); i++){
		_ambient_texture_num.push_back(0 + i * 3);
		_diffuse_texture_num.push_back(1 + i * 3);
		_specular_texture_num.push_back(2 + i * 3);

		_ambient_mode.push_back(0);
		_diffuse_mode.push_back(0);
		_specular_mode.push_back(0);

		_ambient_colors.push_back(_materials[i]->getAmbientColor());
		_diffuse_colors.push_back(_materials[i]->getDiffuseColor());
		_specular_colors.push_back(_materials[i]->getSpecularColor());


		//TODO confirm that this works...
		if(_ambient_colors[i] != glm::vec3(-1,-1,-1)){
			_ambient_mode[i]= 1;
		}
		if(_diffuse_colors[i] != glm::vec3(-1,-1,-1)){
			_diffuse_mode[i] = 1;
		}
		if(_specular_colors[i] != glm::vec3(-1,-1,-1)){
			_specular_mode[i] =1;
		}

		_ambient_texture.push_back(setUpTexture(_materials[i]->getAmbientTexture(), _ambient_texture_num[i]));//, "AmbientSampler");
		_diffuse_texture.push_back(setUpTexture(_materials[i]->getDiffuseTexture(), _diffuse_texture_num[i]));//, "DiffuseSampler");
		_specular_texture.push_back(setUpTexture(_materials[i]->getSpecularTexture(), _specular_texture_num[i]));//, "SpecularSampler");

		//TODO confirm that this works...
		if(_ambient_texture[i] != -1){
			_ambient_mode[i] = 2;
		}
		if(_diffuse_texture[i] != -1){
			_diffuse_mode[i] = 2;
		}
		if(_specular_texture[i] != -1){
			_specular_mode[i] = 2;
		}
	
		_specularities.push_back(_materials[i]->getSpecularity());

	}
	_animutator = NULL;

	for (int i = 0; i < _vertices.size(); i +=3){
		glm::vec3 avg = glm::vec3((_vertices[i+0].x + _vertices[i+1].x + _vertices[i+2].x) / 3,
				(_vertices[i+0].y + _vertices[i+1].y + _vertices[i+2].y) / 3,
				(_vertices[i+0].z + _vertices[i+1].z + _vertices[i+2].z) / 3);
		_faceAvg.push_back(avg);
	}

	calculateRadius();
}		

void Object::draw() {
	glUseProgram(_programID);

	setUpTransformations();
	setUpMaterials();

	glPolygonMode(GL_FRONT_AND_BACK, _draw_mode);

	for (int i = 0; i < group_start.size(); i++){
		glEnableVertexAttribArray(0);
		glBindAttribLocation(_programID, 0, "in_Positon");
		glBindBuffer(GL_ARRAY_BUFFER, _vertex_vboID[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

		glEnableVertexAttribArray(1);
		glBindAttribLocation(_programID, 1, "in_Normal");
		glBindBuffer(GL_ARRAY_BUFFER, _normal_vboID[i]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

		glEnableVertexAttribArray(2);
		glBindAttribLocation(_programID, 2, "in_UV");
		glBindBuffer(GL_ARRAY_BUFFER, _UV_vboID[i]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);;

		glEnableVertexAttribArray(3);
		glBindAttribLocation(_programID, 3, "in_Material_Num");
		glBindBuffer(GL_ARRAY_BUFFER, _Material_Num_vboID[i]);
		glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, 0, (void *) 0);;

		glUniform1i(glGetUniformLocation(_programID, "AmbientSampler"), _ambient_texture_num[_material_nums[group_start[i]]]);
		glUniform1i(glGetUniformLocation(_programID, "DiffuseSampler"), _diffuse_texture_num[_material_nums[group_start[i]]]);
		glUniform1i(glGetUniformLocation(_programID, "SpecularSampler"), _specular_texture_num[_material_nums[group_start[i]]]);

		glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
	}
}

void Object::addAnimutator(Animutator* animutator){
	_animutator = animutator;
}

glm::mat4 Object::makeModelMatrix(){

	if(_animutator != NULL){
		_animutator->update();

		_scale = _animutator->getScale();
		_rotation_axis = _animutator->getRotationAxis();
		_rotation_magnitude = _animutator->getRotationMagnitude();
		_position = _animutator->getPosition();
	}

	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, _position);
	model = glm::scale(model, _scale);
	model = glm::rotate(model, _rotation_magnitude, _rotation_axis);
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
	if(_materials.size() > 0){

		glUniform3fv(glGetUniformLocation(_programID, "in_ambient_color"), _ambient_colors.size(), &_ambient_colors[0][0]);
		glUniform3fv(glGetUniformLocation(_programID, "in_diffuse_color"), _diffuse_colors.size(), &_diffuse_colors[0][0]);
		glUniform3fv(glGetUniformLocation(_programID, "in_specular_color"), _specular_colors.size(), &_specular_colors[0][0]);

		for(int i = 0; i < _ambient_texture.size(); i++){
			glActiveTexture(GL_TEXTURE0 + _ambient_texture_num[i]);
			glBindTexture(GL_TEXTURE_2D, _ambient_texture[i]);
		}	


		for(int i = 0; i < _diffuse_texture.size(); i++){
			glActiveTexture(GL_TEXTURE0 + _diffuse_texture_num[i]);
			glBindTexture(GL_TEXTURE_2D, _diffuse_texture[i]);
		}	

		
		for(int i = 0; i < _specular_texture.size(); i++){
			glActiveTexture(GL_TEXTURE0 + _specular_texture_num[i]);
			glBindTexture(GL_TEXTURE_2D, _specular_texture[i]);
		}	

		glUniform1fv(glGetUniformLocation(_programID, "specularity"), _specularities.size(), &_specularities[0]);

	}

	glUniform1iv(glGetUniformLocation(_programID, "ambient_mode"), _ambient_mode.size(), &_ambient_mode[0]);
	glUniform1iv(glGetUniformLocation(_programID, "diffuse_mode"), _diffuse_mode.size(),&_diffuse_mode[0]);
	glUniform1iv(glGetUniformLocation(_programID, "specular_mode"), _specular_mode.size(),&_specular_mode[0]);


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

std::vector <glm::vec3> Object::getFaceAverages(){
	return _faceAvg;
}

GLfloat Object::getRadius(){
	return _radius;
}

void Object::setPosition(glm::vec3 position){
	_position = position;
}

glm::vec3 Object::getPosition(){
	return _position;
}

void Object::setRotation(GLfloat magnitude, glm::vec3 axis){
	_rotation_magnitude = magnitude;
	_rotation_axis = axis;
}

glm::vec3 Object::getRotation(){
	return glm::vec3(_rotation_magnitude, _rotation_magnitude, _rotation_magnitude) * _rotation_axis;
}

void Object::setScale(glm::vec3 scale){
	_scale = scale;
}

glm::vec3 Object::getScale(){
	return _scale;
}

glm::mat4 Object::getModelMatrix(){
	return makeModelMatrix();
}


//TODO verify
void Object::calculateRadius(){
	glm::vec4 world_origin = glm::vec4(0,0,0,1) * getModelMatrix();

	GLfloat longest = 0;	

	for (int i = 0; i < _vertices.size(); i++){

		glm::vec4 world_vertex = glm::vec4(_vertices[i].x, _vertices[i].y, _vertices[i].z, 1.0) * getModelMatrix();

		GLfloat distance_from_origin_squared = pow(world_origin.x - world_vertex.x, 2) 
			+ pow(world_origin.y - world_vertex.y, 2) 
			+ pow(world_origin.z - world_vertex.z, 2); 	

		if (distance_from_origin_squared > longest){
			longest = distance_from_origin_squared;
		}
	}

	_radius =  sqrt(longest);
}
