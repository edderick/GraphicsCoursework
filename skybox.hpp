#ifndef __SKYBOX__
#define __SKYBOX__

#include "gl_common.hpp" 
 
//local includes 
#include "utils.hpp"
#include "objloader.hpp"

#include "object.hpp"
#include "material.hpp"
#include "viewer.hpp"

class Skybox : public Object {
	public:
		Skybox(GeometryGenerator* gg, GLuint programID, Viewer* viewer, GLenum draw_mode = GL_LINE);

		virtual glm::mat4 makeModelMatrix();
		virtual glm::mat4 makeViewMatrix();
		virtual glm::mat4 makeProjectionMatrix();

};

#endif
