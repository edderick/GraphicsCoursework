#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "gl_common.hpp"

//Local Includes 
#include "utils.hpp" 

class Material {
	private:
		
		GLfloat _Ns, _Ni, _d;
		glm::vec3 _Ka, _Kd, _Ks;

		GLuint _illum;

		char* _map_Ka_file_name; 
		char* _map_Kd_file_name; 
		char* _map_Ks_file_name;
	public:
		char* _name;
		Material(char* name);

	        bool operator==(const Material lhs) const;

		//Setters!
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
	

		//Getters!
		glm::vec3 getAmbientColor();
		glm::vec3 getDiffuseColor();
		glm::vec3 getSpecularColor();
		
		GLuint getIllumination();
		GLfloat getTransparency();
		GLfloat getSpecularity();
		GLfloat getOpticalDensity();

		char* getAmbientTexture();
		char* getDiffuseTexture();
		char* getSpecularTexture();
		
};

#endif
