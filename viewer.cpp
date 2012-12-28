#include "viewer.hpp"

const float PI = 3.14;

Viewer::Viewer() {
	_position = glm::vec3(0,0,5);
	_direction = glm::vec3(0,0,-1);
	_up = glm::vec3(0,1,0);

	_velocity = glm::vec3(0,0,0);
	_cameraRotationVelocity = 0;

	_current_angle = 3 * PI;

	_lastAccessedTime = (GLfloat) glfwGetTime();
}

void Viewer::move(float dx, float dy, float dz){
	_position.x = _position.x + dz * -sin(_current_angle) + dx * -sin(_current_angle + (PI /2));
	_position.y = _position.y + dy;
	_position.z = _position.z + dz * -cos(_current_angle) + dx * -cos(_current_angle + (PI/2));
}

void Viewer::setVelocity(GLfloat ax, GLfloat ay, GLfloat az){
	_velocity.x = ax;
	_velocity.y = ay;
	_velocity.z = az;
}

void Viewer::setStrafeVelocity(GLfloat velocity){
	_velocity.x = velocity;
}


void Viewer::setUpVelocity(GLfloat velocity){
	_velocity.y = velocity;
}

void Viewer::setForwardVelocity(GLfloat velocity){
	_velocity.z = velocity;
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

void Viewer::rotateCamera(GLfloat angle){
	_current_angle = _current_angle + angle;

	_direction.x = sin(_current_angle);
	_direction.z = cos(_current_angle);
}

void Viewer::setCameraRotationVelocity(GLfloat velocity){
	_cameraRotationVelocity = velocity;
}

// z inverting goes on here!
glm::vec3 Viewer::getPosition(){
	GLfloat elapsedTime = glfwGetTime() - _lastAccessedTime;
	
	glm::vec3 displacement = _velocity * elapsedTime;
	GLfloat cameraRotation = _cameraRotationVelocity * elapsedTime;

	move(displacement.x, displacement.y, - displacement.z);
	rotateCamera(cameraRotation);
	
	_lastAccessedTime = glfwGetTime();
	return _position;
}

glm::vec3 Viewer::getLookAt(){
	return _position + _direction;
}

glm::vec3 Viewer::getDirection(){
	return _direction;
}

glm::vec3 Viewer::getUp(){
	return _up;
}





