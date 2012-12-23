/**
  This file is based on the one provided by 
  John Carter in the examples of GL programs.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.hpp"

/* A simple function that will read a file into an allocated char pointer buffer */
char* filetobuf(const char *file)
{
	FILE *fptr;
	long length;
	char *buf;

	fptr = fopen(file, "r"); /* Open file for reading */
	if (!fptr){
		fprintf(stderr, "%s not found\n", file);
		exit(0);
	} /* Return NULL on failure */
	fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
	length = ftell(fptr); /* Find out how many bytes into the file we are */
	buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file plus a null terminator */
	fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
	fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
	fclose(fptr); /* Close the file */
	buf[length] = 0; /* Null terminator */

	return buf; /* Return the buffer */
}

GLuint setupShaders(const char *vertexShaderFilename, const char *fragmentShaderFilename){
	
	
	fprintf(stderr, "Set up shaders\n");

	//Load files into buffers
	GLchar *vertexSource, *fragmentSource; 
	vertexSource = filetobuf(vertexShaderFilename);
	fragmentSource = filetobuf(fragmentShaderFilename);

	//Create shaders 
	GLuint vertexShaderID, fragmentShaderID; 
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//Fill shaders from source
	glShaderSource(vertexShaderID, 1, (const GLchar**)&vertexSource, 0); 
	glShaderSource(fragmentShaderID, 1, (const GLchar**)&fragmentSource, 0); 

	//Compile shaders
	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	//Create program
	GLuint programID = glCreateProgram();

	//Attatch shaders to program
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	//Link program
	glLinkProgram(programID);

	//Ensure there are no errors

	//The maximum length logs may be in characters
	const int MAX_LOG_LENGTH = 1000;
	char text[MAX_LOG_LENGTH];

	int length; 
	glGetProgramInfoLog(programID, MAX_LOG_LENGTH, &length, text);

	if (length > 0) {
		fprintf(stderr, "An Error occured seting up shaders: \n%s\n", text);
	}   

	return programID;
}


