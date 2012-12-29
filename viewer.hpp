#ifndef __VIEWER_HPP__
#define __VIEWER_HPP__

#include "gl_common.hpp"

//local includes  
#include "utils.hpp" 

class Viewer {
	private:
		GLfloat _current_angle;

		glm::vec3 _position;
		glm::vec3 _direction;
		glm::vec3 _up;
		
		glm::vec3 _velocity;
		GLfloat _cameraRotationVelocity;
		
		GLfloat _lastAccessedTime;
		
		void move(GLfloat dx, GLfloat dy, GLfloat dz);
		void rotateCamera(GLfloat angle);
	public:
		Viewer();
		
		void update();

		void setVelocity(GLfloat ax, GLfloat ay, GLfloat az);

		void setStrafeVelocity(GLfloat velocity);
		void setUpVelocity(GLfloat velocity);
		void setForwardVelocity(GLfloat velocity);

		void changeVelocity(GLfloat dax, GLfloat day, GLfloat daz);

		void setCameraRotationVelocity(GLfloat velocity);

		glm::vec3 getPosition();
		glm::vec3 getLookAt();
		glm::vec3 getUp();
		glm::vec3 getDirection();
};

#endif
