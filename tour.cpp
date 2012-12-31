#include "tour.hpp"

WayPoint::WayPoint(glm::vec3 position, glm::vec3 direction){
	_position = position;
	_direction = direction;
}

Tour::Tour(Viewer* viewer){
	_viewer = viewer;
}

void Tour::addWayPoint(GLfloat time, WayPoint* wayPoint){
	_wayPoints.push_back(wayPoint);
	_times.push_back(time);
}

void Tour::start(){
	_started = 1;
	_startTime = glfwGetTime();
}

void Tour::update(){
	GLfloat time = glfwGetTime() - _startTime;

	int index = getIndexForTime(time);
	GLfloat percent = getPercentOfWayPoint(index, time);

	glm::vec3 position = calculatePosition(index, percent);
	glm::vec3 direction = calculateDirection(index, percent);

	_viewer->gotoLocation(position, direction);

	_viewer->update();
}

void Tour::reset(){
	_startTime = glfwGetTime();
}

int Tour::getIndexForTime(GLfloat time){
	for(int i = 0; i < _times.size(); i++){
		if (_times[i] > time) return i;
	}
	
	//if not found, return the last index 
	return _times.size();
}

GLfloat Tour::getPercentOfWayPoint(int index, GLfloat time){
	if(index == _times.size()) return 1;

	GLfloat start_time = _times[index];	
	GLfloat end_time = _times[index + 1];
	
	return (time - start_time) / (end_time - start_time);
}

glm::vec3 Tour::calculatePosition(int index, GLfloat percent){
	if(index == _wayPoints.size()) return _wayPoints[index]->_position;

	glm::vec3 start_pos = _wayPoints[index]->_position;
	glm::vec3 end_pos = _wayPoints[index + 1]->_position;

	glm::vec3 delta = end_pos - start_pos;	
	
	return start_pos + (delta * glm::vec3(percent, percent, percent));
}

glm::vec3 Tour::calculateDirection(int index, GLfloat percent){

//TODO ME -- curently this just uses the direction of travel
// I want it more Dragon Ride like


	if(index == _wayPoints.size()) return _wayPoints[index]->_position;

	glm::vec3 start_pos = _wayPoints[index]->_position;
	glm::vec3 end_pos = _wayPoints[index + 1]->_position;

	glm::vec3 delta = end_pos - start_pos;	
	
	return start_pos + (delta * glm::vec3(percent + 0.1, percent + 0.1, percent + 0.1));


}


