#include "viewer.hpp"

const float PI = 3.14;

Viewer::Viewer() {
	_position = glm::vec3(0,1,5);
	_direction = glm::vec3(0,0,-1);
	_up = glm::vec3(0,1,0);

	_velocity = glm::vec3(0,0,0);
	_cameraRotationVelocity = 0;

	_current_angle = 3 * PI;

	_lastAccessedTime = (GLfloat) glfwGetTime();
	
	_radius = 0.1;

}

void Viewer::addTerrain(Object* object){
	_terrain.push_back(object);
}

void Viewer::addCollidesWith(Object* object){
	_collidesWith.push_back(object);
}

void Viewer::gotoLocation(glm::vec3 position, glm::vec3 direction){
	_position = position;
	_direction = direction;
	_current_angle = asin(_direction.x);
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
		
		
		if(world_vertex.y + 0.1> _position.y){
		 	//If we can climb, then climb
			if (world_vertex.y - _position.y < 0.2)	{
				_position.y = world_vertex.y + 0.1;
			} else {
				//Otherwise, ABORT
				collision = 1;
			}
		}

	}

	return collision;
}

bool Viewer::checkObjectCollisions(){
	bool collision = 0;

	for(int i = 0; i < _collidesWith.size(); i++){
		glm::vec3 relative_positon = _position - (_collidesWith[i]->getPosition() );
		GLfloat distance_squared = pow(relative_positon.x, 2) + pow(relative_positon.y, 2) + pow(relative_positon.z, 2);
		GLfloat min_distance_squared = pow(_radius, 2) + pow(_collidesWith[i]->getRadius(), 2);

		if(distance_squared < min_distance_squared){
			collision = 1;
		}
	}

	return collision;
}

void Viewer::update(){

 	GLfloat elapsedTime = glfwGetTime() - _lastAccessedTime;
	glm::vec3 displacement = _velocity * elapsedTime;
	GLfloat cameraRotation = _cameraRotationVelocity * elapsedTime;

	rotateCamera(cameraRotation);
	//if(moveFlag){
	move(displacement.x, displacement.y, - displacement.z);
	//}
	_lastAccessedTime = glfwGetTime();

}

void Viewer::move(float dx, float dy, float dz){
	glm::vec3 old_position = _position;

	_position.x = _position.x + dz * -sin(_current_angle) + dx * -sin(_current_angle + (PI /2));
	_position.y = _position.y + dy;
	_position.z = _position.z + dz * -cos(_current_angle) + dx * -cos(_current_angle + (PI/2));

	if (checkTerrainCollision() || checkObjectCollisions()){
		//ROLLBACK
		_position = old_position;
		
		setVelocity(0,0,0);

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





