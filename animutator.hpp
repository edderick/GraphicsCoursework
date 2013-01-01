#ifndef __ANIMUTATOR_HPP__
#define __ANIMUTATOR_HPP__

#include "gl_common.hpp"
#include <vector>

class KeyFrame {
	//TODO Evaluate need for accessors
	public:
		KeyFrame(glm::vec3 position, 
			 GLfloat rotation_magnitude = 0, 
			 glm::vec3 rotation_axis = glm::vec3(0,1,0), 
			 glm::vec3 scale = glm::vec3(1,1,1));
		
		glm::vec3 _position;
		GLfloat _rotation_magnitude;
		glm::vec3 _rotation_axis;
		glm::vec3 _scale;
};


class Animutator {
	private:
		std::vector<KeyFrame* > _keyFrames;
		std::vector<GLfloat> _times;

		GLfloat _startTime, _duration;
	
		glm::vec3 _position;
		GLfloat _rotation_magnitude;
		glm::vec3 _rotation_axis;
		glm::vec3 _scale;

		int getIndexForTime(GLfloat time);
		float getPercentOfKeyFrame(int index, GLfloat time);
	
		glm::vec3 extrapolateVector(glm::vec3 v1, glm::vec3 v2, float percent);
		GLfloat extrapolateFloat(GLfloat f1, GLfloat f2, float percent);			

		glm::vec3 calculatePosition(int index, GLfloat percent);
		glm::vec3 calculateScale(int index, GLfloat percent);
		glm::vec3 calculateRotationAxis(int index, GLfloat percent);
		GLfloat calculateRotationMagnitude(int index, GLfloat percent);

	public:
		Animutator();
		void addKeyFrame(GLfloat time, KeyFrame* keyFrame);

		void update();

		glm::vec3 getPosition();
		GLfloat getRotationMagnitude();
		glm::vec3 getRotationAxis();
		glm::vec3 getScale();
};


#endif
