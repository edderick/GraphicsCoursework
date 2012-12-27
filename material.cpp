#include "material.hpp"

Material::Material(char* name){
	_name = name;
}

void Material::setAmbientColor(glm::vec3 Ka){
	_Ka = Ka;
}

void Material::setDiffuseColor(glm::vec3 Kd){
	_Kd = Kd;
}

void Material::setSpecularColor(glm::vec3 Ks){
	_Ks = Ks;
}

void Material::setIllumination(GLuint illum){
	_illum = illum;
}

void Material::setTransparency(GLfloat Tr){
	_d = Tr;
}

void Material::setSpecularity(GLfloat Ns){
	_Ns = Ns;
}

void Material::setOpticalDensity(GLfloat Ni){
	_Ni = Ni;
}

void Material::setAmbientTexture(char* file_name){
	_map_Ka_file_name = file_name;
}

void Material::setDiffuseTexture(char* file_name){
	_map_Kd_file_name = file_name;
}

void Material::setSpecularTexture(char* file_name){
	_map_Ks_file_name = file_name;
}


glm::vec3 Material::getAmbientColor(){
	return _Ka;
}

glm::vec3 Material::getDiffuseColor(){
 	return _Kd;
}

glm::vec3 Material::getSpecularColor(){
	return _Ks;
}

GLuint Material::getIllumination(){
	return _illum;
}

GLfloat Material::getTransparency(){
	return _d;
}

GLfloat Material::getSpecularity(){
	return _Ns;
}
GLfloat Material::getOpticalDensity(){
	return _Ni;
}

char* Material::getAmbientTexture(){
	return _map_Ka_file_name;
}

char* Material::getDiffuseTexture(){
	return _map_Kd_file_name;
}

char* Material::getSpecularTexture(){
	return _map_Ks_file_name;
}


