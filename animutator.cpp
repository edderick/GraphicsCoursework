#include "animutator.hpp"

KeyFrame::KeyFrame(glm::vec3 position, GLfloat rotation_magnitude, glm::vec3 rotation_axis, glm::vec3 scale) {
	_position = position;
	_rotation_magnitude = rotation_magnitude;
	_rotation_axis = rotation_axis;
	_scale = scale;
}

Animutator::Animutator(){
	_startTime = glfwGetTime();
	_duration = 0;
}

void Animutator::addKeyFrame(GLfloat time, KeyFrame* keyFrame){
	_keyFrames.push_back(keyFrame);
	_times.push_back(time);

	if(time > _duration) _duration = time;
}

void Animutator::update(){
	GLfloat time = glfwGetTime() - _startTime;
	int n = floor(time / _duration);
	time = time - (_duration * n);

	int index = getIndexForTime(time);
	GLfloat percent = getPercentOfKeyFrame(index, time);

	_position = calculatePosition(index, percent);
	_rotation_magnitude = calculateRotationMagnitude(index, percent);
	_rotation_axis = calculateRotationAxis(index, percent);
	_scale = calculateScale(index, percent);

}

int Animutator::getIndexForTime(GLfloat time){
	for(int i = 0; i < _times.size(); i++){
		        if (_times[i] > time) return i - 1;
	}

	//if not found, return the last index 
	return _times.size() - 1;
}

float Animutator::getPercentOfKeyFrame(int index, GLfloat time){
	if(index == _times.size() - 1) return 1;

	GLfloat start_time = _times[index];     
	GLfloat end_time = _times[index + 1];

	return (time - start_time) / (end_time - start_time);
}


glm::vec3 Animutator::getPosition(){
	return _position;
}

GLfloat Animutator::getRotationMagnitude(){
	return _rotation_magnitude;
}

glm::vec3 Animutator::getRotationAxis(){
	return _rotation_axis;
}

glm::vec3 Animutator::getScale(){
	return _scale;
}	

glm::vec3 Animutator::extrapolateVector(glm::vec3 v1, glm::vec3 v2, float percent){
	glm::vec3 delta = v2 - v1;  
	return v1 + (delta * glm::vec3(percent, percent, percent));
}

GLfloat Animutator::extrapolateFloat(GLfloat f1, GLfloat f2, float percent){
	GLfloat delta = f2 - f1;
	return f1 + delta * percent;
}

glm::vec3 Animutator::calculatePosition(int index, GLfloat percent){
	//Animation loops
	int next_index = index + 1;
	if (index == _keyFrames.size() - 1) {
		next_index = 0;
	}

	glm::vec3 start_pos = _keyFrames[index]->_position;
	glm::vec3 end_pos = _keyFrames[next_index]->_position;

	return extrapolateVector(start_pos, end_pos, percent);
}

glm::vec3 Animutator::calculateScale(int index, GLfloat percent){
	//Animation loops
	int next_index = index + 1;
	if (index == _keyFrames.size() - 1) {
		next_index = 0;
	}

	glm::vec3 start_scale = _keyFrames[index]->_scale;
	glm::vec3 end_scale = _keyFrames[next_index]->_scale;

	return extrapolateVector(start_scale, end_scale, percent);
}

glm::vec3 Animutator::calculateRotationAxis(int index, GLfloat percent){
	//Animation loops
	int next_index = index + 1;
	if (index == _keyFrames.size() - 1) {
		next_index = 0;
	}

	glm::vec3 start_axis = _keyFrames[index]->_rotation_axis;
	glm::vec3 end_axis = _keyFrames[next_index]->_rotation_axis;

	return extrapolateVector(start_axis, end_axis, percent);
}

GLfloat Animutator::calculateRotationMagnitude(int index, GLfloat percent){
	//Animation loops	
	int next_index = index + 1;
	if (index == _keyFrames.size() - 1) {
		next_index = 0;
	}

	GLfloat start_mag = _keyFrames[index]->_rotation_magnitude;
	GLfloat end_mag = _keyFrames[next_index]->_rotation_magnitude;

	return extrapolateFloat(start_mag, end_mag, percent);
}	
