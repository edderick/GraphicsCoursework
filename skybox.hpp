//Standard includes 
#include <stdlib.h> 
#include <stdio.h> 
#include <stddef.h> 
#include <string.h> 

//GLimg
#include <memory>
#include <glload/gll.hpp>
#include <glimg/glimg.h>
#include <glimg/ImageCreatorExceptions.h>
#include <glimg/TextureGeneratorExceptions.h>

//GL stuff 
#include <GL/glew.h> 
#include <GL/glfw.h> 
 
//GLM stuff 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
 
//local includes 
#include "utils.hpp"

#if !defined (OBJECT_HPP) 
#define OBJECT_HPP
#include "object.hpp"
#endif

#if !defined( OBJLOADER_HPP)
#define OBJLOADER_HPP
#include "objloader.hpp"
#endif

#if !defined( MATERIAL_HPP )
#define MATERIAL_HPP
#include "material.hpp"
#endif

#if !defined( VIEWER_HPP ) 
#define VIEWER_HPP
#include "viewer.hpp"
#endif

class Skybox : public Object {
	public:
		Skybox(const char* obj_file_name, GLuint programID, Viewer* viewer, GLenum draw_mode = GL_LINE);

		virtual glm::mat4 makeModelMatrix();
		virtual glm::mat4 makeViewMatrix();
		virtual glm::mat4 makeProjectionMatrix();

};

