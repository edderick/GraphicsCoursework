#include "object.hpp"

const GLfloat PI = 3.14159; 

Object::Object(const char* obj_file_name, GLuint programID, GLenum draw_mode) {
	_programID = programID;
	_draw_mode = draw_mode;
	
	//glGenVertexArrays(1, &_vaoID);
	//glBindVertexArray(_vaoID);

	//glGenBuffers(1, &_base_vboID);
	//glBindBuffer(GL_ARRAY_BUFFER, _base_vboID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(basePoints), basePoints, GL_STATIC_DRAW); 

	//glGenBuffers(1, &_cone_vboID);
	//glBindBuffer(GL_ARRAY_BUFFER, _cone_vboID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(conePoints), conePoints, GL_STATIC_DRAW);
}		

void Object::draw() {
	
	glUseProgram(_programID);

	glBindVertexArray(_vaoID);

	glEnableVertexAttribArray(0);
	glBindAttribLocation(_programID, 0, "in_Positon");

	glPolygonMode(GL_FRONT_AND_BACK, _draw_mode);

	glBindBuffer(GL_ARRAY_BUFFER, _base_vboID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, _num_of_faces + 1 + 1);

	glBindBuffer(GL_ARRAY_BUFFER, _cone_vboID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0)

	glDrawArrays(GL_TRIANGLE_FAN, 0, _num_of_faces + 1 + 1);

	glDisableVertexAttribArray(0);
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

