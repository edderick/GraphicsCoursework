#include "viewer.hpp"

const float PI = 3.14;

Viewer::Viewer() {
	_position = glm::vec3(1.42417, 0.116667, 0.276567);
	_direction = glm::vec3(0,0,1);
	_up = glm::vec3(0,1,0);

	_elevation = glm::vec3(0,0.2,0);

	_velocity = glm::vec3(0,0,0);
	_cameraRotationVelocity = 0;

	_current_angle = -PI / 10;

	_lastAccessedTime = (GLfloat) glfwGetTime();
	
	_radius = 0.1;

	_fall_speed = 0.01;
}

void Viewer::resetElevation(){
	_elevation = glm::vec3(0,0.2,0);
}

void Viewer::addTerrain(Object* object){
	_terrain.push_back(object);
}

void Viewer::addCollidesWith(Object* object){
	_collidesWith.push_back(object);
}

void Viewer::gotoLocation(glm::vec3 position, glm::vec3 direction){
	_position = position;
	_direction = glm::normalize(direction);
	
	glm::vec3 origin_direction(0,0,1);

	double y = (_direction.x * origin_direction.z) - (origin_direction.x * _direction.z);
	double x = (_direction.x * origin_direction.x) + (_direction.z * origin_direction.z);

	_current_angle = atan2(y,x);
}

bool Viewer::checkTerrainCollision(){
	bool collision = 0;
	
	for (int i = 0; i < _terrain.size(); i++){
	
		//Prevent clipping though things	
		glm::vec3 hyp_pos = _position + (glm::vec3(_radius * 2, _radius * 2, _radius * 2) * _direction );

		glm::mat4 m = _terrain[i]->getModelMatrix();
		glm::mat4 inverseM = glm::inverse(m);
	
		glm::vec4 model_position_4 = inverseM * glm::vec4(hyp_pos.x, hyp_pos.y, hyp_pos.z, 1);
		glm::vec3 model_position = glm::vec3(model_position_4.x, model_position_4.y, model_position_4.z);

		std::vector<glm::vec3> vertices = _terrain[i]->getFaceAverages();

		glm::vec3* min_vertex;
		GLfloat distance = FLT_MAX;
		
		for(int j=0; j < vertices.size(); j++){
		
			if (distance == 0.1) {
				break;
			}

			GLfloat this_distance = pow(model_position.x - vertices[j].x, 2) + pow(model_position.z - vertices[j].z, 2);
			if (this_distance < distance){
				min_vertex = &vertices[j];
				distance = this_distance;
			}

		}
		
		glm::vec4 world_vertex = m * glm::vec4(min_vertex->x, min_vertex->y, min_vertex->z, 1);
		
		
		if(world_vertex.y + 0.1 > _position.y){
			_fall_speed = 0;
		 	//If we can climb, then climb
			if (world_vertex.y - _position.y < 0.1)	{
				_position.y = world_vertex.y + 0.1;
			} else {
				//Otherwise, ABORT
				collision = 1;
			}
		} else {
			_fall_speed += 0.01;
		}

	}

	return collision;
}

bool Viewer::checkObjectCollisions(){
	bool collision = 0;

	for(int i = 0; i < _collidesWith.size(); i++){
		glm::vec3 relative_positon = (_position + (_direction * glm::vec3(0.1,0.1,0.1) ))- (_collidesWith[i]->getPosition() );
		GLfloat distance_squared = pow(relative_positon.x, 2) + pow(relative_positon.y, 2) + pow(relative_positon.z, 2);
		GLfloat min_distance_squared =  pow(_collidesWith[i]->getRadius(), 2);

		if(distance_squared < min_distance_squared){
			collision = 1;
		}
	}

	return collision;
}

void Viewer::update(){

 	GLfloat elapsedTime = glfwGetTime() - _lastAccessedTime;
	_lastAccessedTime = glfwGetTime();
	changeElevation(_elevation_velocity * elapsedTime);

	_velocity = _velocity - glm::vec3(0,_fall_speed,0);
	_direction.y = 0;

	glm::vec3 displacement = _velocity * elapsedTime;
	GLfloat cameraRotation = _cameraRotationVelocity * elapsedTime;

	rotateCamera(cameraRotation);
	move(displacement.x, displacement.y, - displacement.z);
}

void Viewer::move(float dx, float dy, float dz){
	glm::vec3 old_position = _position;

	_position.x = _position.x + dz * -sin(_current_angle) + dx * -sin(_current_angle + (PI /2));
	_position.y = _position.y + dy;
	_position.z = _position.z + dz * -cos(_current_angle) + dx * -cos(_current_angle + (PI/2));

	if (checkTerrainCollision()){
		//ROLLBACK
		_position = old_position;
		
		setVelocity(0,0,0);
	} 

	if (checkObjectCollisions()){
		_position.x = _position.x - dz * -sin(_current_angle);
		_position.z = _position.z - dz * -cos(_current_angle);

		setForwardVelocity(0);
	}
}

glm::vec3 Viewer::getMovePos(float dx, float dy, float dz){
	glm::vec3 dposition = glm::vec3(0,0,0);

	dposition.x = _position.x + dz * -sin(_current_angle) + dx * -sin(_current_angle + (PI /2));
	dposition.y = _position.y + dy;
	dposition.z = _position.z + dz * -cos(_current_angle) + dx * -cos(_current_angle + (PI/2));

	return dposition;
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

void Viewer::changeElevation(GLfloat delta){

	if(_elevation.y + delta < 0.1) return;
	if(_elevation.y + delta > 2) return;

	_elevation = _elevation + glm::vec3(0, delta, 0);
}

void Viewer::setElevationVelocity(GLfloat velocity){
	_elevation_velocity = velocity;
}

void Viewer::changeVelocity(GLfloat dax, GLfloat day, GLfloat daz){
	_velocity.x = _velocity.x + dax;
	_velocity.y = _velocity.y + day;
	_velocity.z = _velocity.z + daz;

	if (_velocity.x < 0){
		_velocity.x = 0;
	} 
	if (_velocity.y < 0){
		_velocity.y = 0;
	}
	if (_velocity.z < 0){
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
	return _position + _elevation;
}

glm::vec3 Viewer::getLookAt(){
	return _position + _direction + _elevation;
}

glm::vec3 Viewer::getDirection(){
	return _direction;
}

glm::vec3 Viewer::getUp(){
	return _up;
}





