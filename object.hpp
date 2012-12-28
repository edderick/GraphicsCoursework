#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "gl_common.hpp"

//local includes 
#include "utils.hpp"
#include "objloader.hpp"
#include "geometrygenerator.hpp"

#include "material.hpp"
#include "viewer.hpp"

class Object {
	private:
		const char* _obj_file_name;
		
		Material* _material;

		GLuint _programID;
		GLenum _draw_mode;

		GLuint _ambient_texture, _diffuse_texture, _specular_texture;

		GLuint _vaoID; 
		GLuint _vertex_vboID;
		GLuint _normal_vboID;
		GLuint _UV_vboID;
		
		std::vector<glm::vec3> _vertices;
		std::vector<glm::vec3> _normals;
		std::vector<glm::vec2> _UVs;

		static const GLuint AMBIENT_TEXTURE = GL_TEXTURE0;
		static const GLuint AMBIENT_TEXTURE_NUM = 0;

		static const GLuint DIFFUSE_TEXTURE = GL_TEXTURE1;
		static const GLuint DIFFUSE_TEXTURE_NUM = 1;

		static const GLuint SPECULAR_TEXTURE = GL_TEXTURE2;
		static const GLuint SPECULAR_TEXTURE_NUM = 2;

	protected:
		virtual glm::mat4 makeModelMatrix();
		virtual glm::mat4 makeViewMatrix();
		virtual glm::mat4 makeProjectionMatrix();

		Viewer* _viewer;

		glm::vec3 _position;
		glm::vec3 _scale;
		GLfloat _rotation_magnitude;
		glm::vec3 _rotation_axis;

	public:
		Object(GeometryGenerator* gg, GLuint programID, Viewer* viewer, GLenum draw_mode = GL_LINE);
		void setUpTransformations();
		GLuint setUpTexture(char* texture_file_name, GLuint ActiveTexture, GLuint ActiveTextureNum ,const char* SamplerName);
		void draw();
};

#endif
