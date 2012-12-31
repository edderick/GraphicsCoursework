#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "gl_common.hpp"

//local includes 
#include "utils.hpp"
#include "objloader.hpp"
#include "geometrygenerator.hpp"

#include "material.hpp"
#include "viewer.hpp"

class Viewer;

class Object {
	private:
		const char* _obj_file_name;
		
		Material* _material;

		GLuint _programID;
		GLenum _draw_mode;

		GLuint _ambient_texture_num, _diffuse_texture_num, _specular_texture_num;
		GLuint _ambient_texture, _diffuse_texture, _specular_texture;
		GLuint _ambient_mode, _diffuse_mode, _specular_mode;

		GLuint _vaoID; 
		GLuint _vertex_vboID;
		GLuint _normal_vboID;
		GLuint _UV_vboID;
		
		std::vector<glm::vec3> _vertices;
		std::vector<glm::vec3> _normals;
		std::vector<glm::vec2> _UVs;

		GLuint setUpTexture(char* texture_file_name, GLuint ActiveTextureNum);
		void useTexture(GLuint textureID, GLuint ActiveTextureNum, const char* SamplerName);

		void setUpTransformations();
		void setUpMaterials();

		void calculateRadius();
	
	protected:
		virtual glm::mat4 makeProjectionMatrix();
		virtual glm::mat4 makeViewMatrix();
		virtual glm::mat4 makeModelMatrix();

		Viewer* _viewer;

		glm::vec3 _position;
		GLfloat _rotation_magnitude;
		glm::vec3 _rotation_axis;
		glm::vec3 _scale;

		GLfloat _radius;
		
	public:
		Object(GeometryGenerator* gg, GLuint programID, Viewer* viewer, GLenum draw_mode = GL_LINE);

		std::vector <glm::vec3>  getFaceAverages();
		GLfloat getRadius();

		void setPosition(glm::vec3 position);
		glm::vec3 getPosition();

		void setRotation(GLfloat magnitude, glm::vec3 axis);
		glm::vec3 getRotation();

		void setScale(glm::vec3 scale);
		glm::vec3 getScale();

		glm::mat4 getModelMatrix();

		//TODO move back?
		void draw();
};

#endif
