#include "tour.hpp"

WayPoint::WayPoint(glm::vec3 position, glm::vec3 direction){
	_position = position;
	_direction = direction;
}

Tour::Tour(Viewer* viewer, bool *tour){
	_viewer = viewer;
	_tour = tour;

	MOTION_MODE = 0;
	LOOK_MODE = 1;
}

void Tour::addWayPoint(GLfloat time, WayPoint* wayPoint, int mode){
	_wayPoints.push_back(wayPoint);
	_times.push_back(time);
	_modes.push_back(mode);
}

void Tour::start(){
	_started = 1;
	_startTime = glfwGetTime();
	_viewer->resetElevation();
}

void Tour::update(){
	GLfloat time = glfwGetTime() - _startTime;

	int index = getIndexForTime(time);
	GLfloat percent = getPercentOfWayPoint(index, time);

	glm::vec3 position = calculatePosition(index, percent);
	glm::vec3 direction; 
	
	if(_modes[index] == MOTION_MODE){
		direction = calculateMotionDirection(index, percent);
	} else {
		direction = calculateLookDirection(index, percent);
	}
	
	_viewer->gotoLocation(position, direction);

	if(index >= (int) _times.size() -1 && percent == 1){
		*_tour = 0;
	}
}

void Tour::reset(){
	_startTime = glfwGetTime();
	_started = 0;
}

void Tour::restart(){
	reset();
	start();
}

int Tour::getIndexForTime(GLfloat time){
	for(unsigned int i = 0; i < _times.size(); i++){
		if (_times[i] > time) return i - 1;
	}
	
	//if not found, return the last index 
	return _times.size() - 1;
}

GLfloat Tour::getPercentOfWayPoint(int index, GLfloat time){
	if(index == (int) _times.size() - 1) return 1;

	GLfloat start_time = _times[index];	
	GLfloat end_time = _times[index + 1];
	
	return (time - start_time) / (end_time - start_time);
}

glm::vec3 Tour::calculatePosition(int index, GLfloat percent){
	if(index == (int) _wayPoints.size() - 1) return _wayPoints[index]->_position;

	glm::vec3 start_pos = _wayPoints[index]->_position;
	glm::vec3 end_pos = _wayPoints[index + 1]->_position;

	glm::vec3 delta = end_pos - start_pos;	
	
	return start_pos + (delta * glm::vec3(percent, percent, percent));
}

glm::vec3 Tour::calculateMotionDirection(int index, GLfloat percent){
	if(index == (int) _wayPoints.size() - 1){ 
		index--;	
	}

	glm::vec3 start_pos = _wayPoints[index]->_position;
	glm::vec3 end_pos = _wayPoints[index + 1]->_position;

	glm::vec3 delta = end_pos - start_pos;	
	
	return delta;
}


glm::vec3 Tour::calculateLookDirection(int index, GLfloat percent){
//TODO Verify that this is correct
	if(index == (int) _wayPoints.size() - 1){ 
		return _wayPoints[index]->_direction;
	}

	glm::vec3 start_dir = _wayPoints[index]->_direction;
	glm::vec3 end_dir = _wayPoints[index + 1]->_direction;

	glm::vec3 delta = end_dir - start_dir;	
	
	return start_dir + (delta * glm::vec3(percent, percent, percent));
}
