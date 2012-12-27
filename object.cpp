#include "object.hpp"

const GLfloat PI = 3.14159; 


Object::Object(const char* obj_file_name, GLuint programID, Viewer* viewer, GLenum draw_mode) {
	_programID = programID;
	_draw_mode = draw_mode;

	_viewer = viewer;
	
	ObjLoader objLoader;
	//TODO magic word!!
	objLoader.load_model("models", obj_file_name);

	_vertices = objLoader.getVertices();
	_normals = objLoader.getNormals();

	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	glGenBuffers(1, &_vertex_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_vboID);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices[0]), &_vertices[0], GL_STATIC_DRAW); 

	glGenBuffers(1, &_normal_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _normal_vboID);
	glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(_normals[0]), &_normals[0], GL_STATIC_DRAW);

	_material = objLoader.getMaterial();
	
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

	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Object::setUpTransformations(){
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.f);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(_viewer->getPosition(),_viewer->getLookAt(), _viewer->getUp());

	glm::mat4 model = glm::mat4(1.f);
	model = glm::rotate(model, (GLfloat) glfwGetTime() * 30.f, glm::vec3(0.f, 1.f, 0.f));

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

}

