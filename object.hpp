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
#include "objloader.hpp"

#if !defined( MATERIAL_HPP )
#define MATERIAL_HPP
#include "material.hpp"
#endif

#if !defined( VIEWER_HPP ) 
#define VIEWER_HPP
#include "viewer.hpp"
#endif

class Object {
	private:
		const char* _obj_file_name;
		
		Material* _material;

		GLuint _programID;
		GLenum _draw_mode;

		GLuint _vaoID; 
		GLuint _vertex_vboID;
		GLuint _normal_vboID;
		
		std::vector<glm::vec3> _vertices;
		std::vector<glm::vec3> _normals;

		Viewer* _viewer;

	public:
		Object(const char* obj_file_name, GLuint programID, Viewer* viewer, GLenum draw_mode = GL_LINE);
		void setUpTransformations();
		//void setUpMaterials();
		void draw();
};

