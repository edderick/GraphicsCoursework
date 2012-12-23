#include "viewer.hpp"

Viewer::Viewer() {
	_position = glm::vec3(0,0,5);
	_direction = glm::vec3(0,0,-1);
	_up = glm::vec3(0,1,0);

	_lastAccessedTime = (GLfloat) glfwGetTime();
}

void Viewer::move(float dx, float dy, float dz){
	_position.x = _position.x + dx;
	_position.y = _position.y + dy;
	_position.z = _position.z + dz;
}

void Viewer::setVelocity(GLfloat ax, GLfloat ay, GLfloat az){
	_velocity.x = ax;
	_velocity.y = ay;
	_velocity.z = az;
}

void Viewer::changeVelocity(GLfloat dax, GLfloat day, GLfloat daz){
	_velocity.x = _velocity.x + dax;
	_velocity.y = _velocity.y + day;
	_velocity.z = _velocity.z + daz;

	if (_velocity.x < 0){
		_velocity.x = 0;
	} else if (_velocity.y < 0){
		_velocity.y = 0;
	} else if (_velocity.z < 0){
		_velocity.z = 0;
	}
}

// z inverting goes on here!
glm::vec3 Viewer::getPosition(){
	GLfloat elapsedTime = glfwGetTime() - _lastAccessedTime;
	
	glm::vec3 displacement = _velocity * elapsedTime;

	move(displacement.x, displacement.y, - displacement.z);
	
	_lastAccessedTime = glfwGetTime();
	return _position;
}

glm::vec3 Viewer::getLookAt(){
	return _position + _direction;
}

glm::vec3 Viewer::getUp(){
	return _up;
}





