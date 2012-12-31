#ifndef __TOUR_HPP__
#define __TOUR_HPP__

#include "gl_common.hpp"
#include <vector>

#include "viewer.hpp"


class WayPoint {
	//TODO not public?
	public:
		WayPoint(glm::vec3 position, glm::vec3 direction);
		
		glm::vec3 _position;
		glm::vec3 _direction;
};

class Tour {
	private:
		std::vector<WayPoint* > _wayPoints;
		std::vector<GLfloat> _times;
		
		Viewer* _viewer;
		
		GLfloat _startTime; 
	
		bool _started;

		//time is passed in to ensure consitency
		glm::vec3 calculatePosition(int index, GLfloat percent);
	 	glm::vec3 calculateDirection(int index, GLfloat percent);

		int getIndexForTime(GLfloat time);
		float getPercentOfWayPoint(int index, GLfloat time);
	
	public:
		Tour(Viewer* viewer);

		void addWayPoint(GLfloat time, WayPoint* wayPoint);
		
		//YAGNI
		//void pause();
		//void un_pause(); 

		void start();
		void update();
		void reset();	
};
#endif
