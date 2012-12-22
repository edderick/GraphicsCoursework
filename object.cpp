#include "object.hpp"

const GLfloat PI = 3.14159; 

Object::Object(const char* obj_file_name, GLuint programID, GLenum draw_mode) {
	_programID = programID;
	_draw_mode = draw_mode;

	ObjLoader objLoader;
	objLoader.load_model(obj_file_name);
	
	_vertices = objLoader.getVertices();
	_elements = objLoader.getElements();

	_normals = objLoader.getNormals();
	_normal_refs = objLoader.getNormalRefs();
	
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	glGenBuffers(1, &_vertex_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_vboID);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices[0]), &_vertices[0], GL_STATIC_DRAW); 

	glGenBuffers(1, &_element_vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _element_vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _elements.size() * sizeof(_elements[0]), &_elements[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &_normal_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _normal_vboID);
	glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(_normals[0]), &_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_normal_ref_vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _normal_ref_vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _normal_refs.size() * sizeof(_normal_refs[0]), &_normal_refs[0], GL_STATIC_DRAW);
	
}		

void Object::draw() {
	glUseProgram(_programID);


	glPolygonMode(GL_FRONT_AND_BACK, _draw_mode);

	glEnableVertexAttribArray(1);
	glBindAttribLocation(_programID, 1, "in_Normal");
	glBindBuffer(GL_ARRAY_BUFFER, _normal_vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _normal_ref_vboID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glEnableVertexAttribArray(0);
	glBindAttribLocation(_programID, 0, "in_Positon");
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _element_vboID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	
	glDrawElements(GL_TRIANGLES, _elements.size(), GL_UNSIGNED_INT, 0);


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

//TODO: Not sure this code should be in here...
void Object::setUpDefaultMVP(){
		glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.f);

		glm::mat4 view = glm::mat4(1.f);
		view = glm::translate(view, glm::vec3(0.f, 0.f, -5.f));

		glm::mat4 model = glm::mat4(1.f);
		model = glm::rotate(model, (GLfloat) glfwGetTime() * 30.f, glm::vec3(1.f, 1.f, 1.f));

		glUniformMatrix4fv(glGetUniformLocation(_programID, "m"), 1, GL_FALSE, glm::value_ptr(model));
	
		glUniformMatrix4fv(glGetUniformLocation(_programID, "v"), 1, GL_FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(glGetUniformLocation(_programID, "p"), 1, GL_FALSE, glm::value_ptr(projection));
	
}

