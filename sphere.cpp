#include "sphere.hpp"

const GLfloat PI = 3.14159; 

Sphere::Sphere(int num_of_faces, GLuint programID, GLboolean is_hedgehog, GLboolean is_shaded, GLenum draw_mode){
	_num_of_faces = num_of_faces;
	_programID = programID;
	_draw_mode = draw_mode;
	_is_hedgehog = is_hedgehog;
	_is_shaded = is_shaded;

	glm::vec3 spherePoints[2 * _num_of_faces * _num_of_faces];
	glm::vec3 normals[2 * _num_of_faces * _num_of_faces];

	for (int i = 0; i < _num_of_faces; i ++){
		GLfloat phi  = (( PI) / _num_of_faces) * i - (PI /2)  ;
		GLfloat nextPhi = (( PI) / _num_of_faces) * (i + 1) - (PI/ 2);

		for (int j = 0; j < _num_of_faces; j++) {
			GLfloat theta = ((2 * PI) / _num_of_faces) * (j + 1) ;

			int index = 2 * _num_of_faces * i + 2*j;

			spherePoints[index + 0] = 
				glm::vec3(sin(theta) * cos(phi), cos(theta) * cos(phi), sin(phi));

			spherePoints[index + 1] = 
				glm::vec3(sin(theta) * cos(nextPhi), cos(theta) * cos(nextPhi), sin(nextPhi));

			glm::vec3 normal = glm::normalize(spherePoints[index + 0]);
			glm::vec3 nextNormal = glm::normalize(spherePoints[index + 1]);

			normals[index + 0] = normal;
			normals[index + 1] = nextNormal;
		}
	}

	glm::vec3 spines[2 * 2 * _num_of_faces * _num_of_faces];

	for (int i = 0; i < 2 * _num_of_faces * _num_of_faces; i++){
		spines[2 * i + 0] = spherePoints[i];
		spines[2 * i + 1] = spherePoints[i] + normals[i];
	}	 

	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	glGenBuffers(1, &_sphere_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _sphere_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spherePoints), spherePoints, GL_STATIC_DRAW); 

	glGenBuffers(1, &_spines_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _spines_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spines), spines, GL_STATIC_DRAW); 

	glGenBuffers(1, &_normal_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _normal_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW); 


}

void Sphere::setUpDefaultMVP(){
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.f);

	glm::mat4 view = glm::lookAt(glm::vec3(0.f,0.f,-5), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

	glm::mat4 model = glm::mat4(1.f);
	model = glm::rotate(model, 90.f , glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, (GLfloat) glfwGetTime() * 60.f, glm::vec3(1.f, 1.f, 1.f));


	glUniformMatrix4fv(glGetUniformLocation(_programID, "m"), 1, GL_FALSE, glm::value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(_programID, "v"), 1, GL_FALSE, glm::value_ptr(view));

	glUniformMatrix4fv(glGetUniformLocation(_programID, "p"), 1, GL_FALSE, glm::value_ptr(projection));
		

}

void Sphere::draw(){
	glUseProgram(_programID);

	glEnableVertexAttribArray(0);
	glBindAttribLocation(_programID, 0, "in_Positon");

	glPolygonMode(GL_FRONT_AND_BACK, _draw_mode);

	if(_is_shaded) {
		glEnableVertexAttribArray(1);
		glBindAttribLocation(_programID, 1, "in_Normal");
		glBindBuffer(GL_ARRAY_BUFFER, _normal_vboID);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _sphere_vboID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * _num_of_faces * _num_of_faces);

	if (_is_hedgehog) {
		glBindBuffer(GL_ARRAY_BUFFER, _spines_vboID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
		glDrawArrays(GL_LINES, 0, 2 * 2 * _num_of_faces * _num_of_faces);
	}

	glDisableVertexAttribArray(0);
}

