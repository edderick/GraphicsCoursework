#ifndef __TEXTGENERATOR_HPP__
#define __TEXTGENERATOR_HPP__

#include "gl_common.hpp"

class TextGenerator {

	private:
		GLint _textureID;
		GLuint _programID;

		GLuint _vertex_buffer;
		GLuint _UV_buffer;

		GLuint _uniformID;

		int _num_chars_x, _num_chars_y;
		char _char_start, _char_stop;
		int _window_width, _window_height;

		GLuint setUpTexture(char* texture_file_name, GLuint ActiveTextureNum);
		void useTexture(GLint textureID, GLuint ActiveTextureNum, const char* SamplerName);
		
		GLuint ACTIVE_TEXTURE;

	public:
		TextGenerator(char* font_file_name, 
			      GLuint programID, 
			      char char_start, 
			      char char_stop, 
			      int num_chars_x, 
			      int num_chars_y, 
			      int window_width, 
			      int window_height);
		void printText(char* text, int x, int y, int size);
	

};

#endif
