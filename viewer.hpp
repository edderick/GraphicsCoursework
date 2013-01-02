#ifndef __VIEWER_HPP__
#define __VIEWER_HPP__

#include "gl_common.hpp"

//local includes  
#include "utils.hpp" 
#include "object.hpp"

#include <vector>

class Object;

class Viewer {
	private:
		GLfloat _current_angle;
	
		GLfloat _elevation_velocity;
		glm::vec3 _elevation;

		glm::vec3 _position;
		glm::vec3 _direction;
		glm::vec3 _up;
	
		GLfloat _radius;

		GLfloat _fall_speed;

		glm::vec3 _velocity;
		GLfloat _cameraRotationVelocity;
		
		GLfloat _lastAccessedTime;
		
		std::vector<Object*> _terrain;
		std::vector<Object*> _collidesWith;

		void move(GLfloat dx, GLfloat dy, GLfloat dz);
		void rotateCamera(GLfloat angle);

		bool checkTerrainCollision();
		bool checkObjectCollisions();
	public:
		Viewer();
		
		void update();
		
		void resetElevation();

		void gotoLocation(glm::vec3 position, glm::vec3 direction);

		void setVelocity(GLfloat ax, GLfloat ay, GLfloat az);

		glm::vec3 getMovePos(GLfloat dx, GLfloat dy, GLfloat dz);

		void setStrafeVelocity(GLfloat velocity);
		void setUpVelocity(GLfloat velocity);
		void setForwardVelocity(GLfloat velocity);

		void changeVelocity(GLfloat dax, GLfloat day, GLfloat daz);

		void setCameraRotationVelocity(GLfloat velocity);

		void addTerrain(Object* object);
		void addCollidesWith(Object* object);

		void changeElevation(GLfloat delta);
		void setElevationVelocity(GLfloat velocity);


		glm::vec3 getPosition();
		glm::vec3 getLookAt();
		glm::vec3 getUp();
		glm::vec3 getDirection();
};

#endif
