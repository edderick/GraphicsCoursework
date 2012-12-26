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
		char* _name;
		
		GLfloat _Ns, _Ni, _d;
		glm::vec3 _Ka, _Kd, _Ks;

		GLuint _illum;

		char* _map_Ka_file_name; 
		char* _map_Kd_file_name; 
		char* _map_Ks_file_name;
	public:
		Material(char* name);
	
		void setAmbientColor(glm::vec3 Ka);
		void setDiffuseColor(glm::vec3 Kd);
		void setSpecularColor(glm::vec3 Ks);
		
		void setIllumination(GLuint illum);
		void setTransparency(GLfloat Tr);
		void setSpecularity(GLfloat Ns);
		void setOpticalDensity(GLfloat Ni);

		void setAmbientTexture(char* file_name);
		void setDiffuseTexture(char* file_name);
		void setSpecularTexture(char* file_name);
		
		//TODO Add some getters :p
};
