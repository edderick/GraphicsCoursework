/**
This file is based on the one provided by 
John Carter as examples of GL programs.
**/

#ifndef _UTILS_H_
#define _UTILS_H_

#include <GL/glew.h>
#include <GL/glfw.h>

//#ifndef GLfloat
//#include <GL3/gl3.h>
//#endif

/* A simple function that will read a file into an allocated char pointer buffer */
char* filetobuf(const char *file);

/* A simple function that will load in two shaders and produce a program for them */
GLuint setupShaders(const char *vertexShaderFilename, const char *fragmentShaderFilename); 

#endif /* #ifndef _UTILS_H_ */
