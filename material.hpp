//Standard includes
#include <stdlib.h> 
#include <stdio.h> 
#include <stddef.h> 
#include <string.h> 
#include <iostream> 
#include <vector> 

//GL stuff 
#include <GL/glew.h> 
#include <GL/glfw.h> 

//GLM stuff 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

//Local Includes 
#include "utils.hpp" 


class Material {
	private:
		GLfloat _Ns, _Ni, _d;
		glm::vec3 _Ka, _Kd, _Ks;

		GLuint _illum;

		char* _map_Ka, _map_Kd, _map_Ks;
	public:
		Material();
	
		void setAmbientColor(glm::vec3 Ka);
		void setDiffuseColor(glm::vec3 Kd);
		void setSpecularColor(glm::vec3 Ks);
		
		void setIllumination(GLuint illum);
		void setTransparency(GLfloat Tr);
		void setSpecularity(GLfloat Ns);
		void setOpticalDensity(GLfloat Ni);

		void setAmbientTexture(const char* file_name);
		void setDiffuseTexture(const char* file_name);
		void setSpecularTexture(const char* file_name);
};
