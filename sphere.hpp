//Standard includes
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

//GL stuff
#include <GL/glew.h>
#include <GL/glfw.h>

//GLM stuff
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//local includes
#include "utils.hpp"

class Sphere {
	private:
		int _num_of_faces;
		GLuint _programID;

		GLuint _vaoID;
		GLuint _sphere_vboID;
		GLuint _normal_vboID;
		GLuint _spines_vboID;

		GLenum _draw_mode;
		GLboolean _is_hedgehog;
		GLboolean _is_shaded;
	public:
		Sphere(int num_of_faces, GLuint programID, GLboolean is_hedgehog = GL_FALSE, GLboolean is_shaded = GL_FALSE, GLenum draw_mode = GL_LINE);
		void setUpDefaultMVP();
		void draw();
};

