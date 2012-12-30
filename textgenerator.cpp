#include "textgenerator.hpp"
#include <vector>

TextGenerator::TextGenerator(char* font_file_name, GLuint programID, char char_start, char char_stop, int num_chars_x, int num_chars_y){
	ACTIVE_TEXTURE = 0;
	
	_num_chars_x = num_chars_x;
	_num_chars_y = num_chars_y;

	_char_start = char_start;
	_char_stop = char_stop;

	_programID = programID;
	_textureID = setUpTexture(font_file_name, ACTIVE_TEXTURE);	


	glGenBuffers(1, &_vertex_buffer);
	glGenBuffers(1, &_UV_buffer);
}

void TextGenerator::printText(char* text, int x, int y, int size){


	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> textureCoords;

	for (int i = 0; i < strlen(text); i++){
		//Vertex Quad
		glm::vec2 v_t_l = glm::vec2(x+i*size, y+size);
		glm::vec2 v_t_r = glm::vec2(x+i*size+size, y+size);
		glm::vec2 v_b_l = glm::vec2(x+i*size, y);
		glm::vec2 v_b_r = glm::vec2(x+i*size+size, y);

		//Create first triangle of Quad
		vertices.push_back(v_t_l);
		vertices.push_back(v_b_l);
		vertices.push_back(v_t_r);

		//Create second triangle of Quad
		vertices.push_back(v_b_r);
		vertices.push_back(v_t_r);
		vertices.push_back(v_b_l);

		char c = text[i];	
			
		if (c < _char_start || c > _char_stop){
			std::cout << "A CHARACTER WAS NOT FOUND IN THE CHARACTER SET";
			return;
		}
		
		//Position in grid
		float uv_x = ((c - _char_start) % _num_chars_x) / (float) _num_chars_x;
		float uv_y = ((c - _char_start) / _num_chars_x) / (float) _num_chars_y;

		//UV quad
		glm::vec2 uv_t_l = glm::vec2(uv_x, 1.0f - uv_y);
		glm::vec2 uv_t_r = glm::vec2(uv_x + 1.0f/_num_chars_x, 1.0f - uv_y);
		glm::vec2 uv_b_l = glm::vec2(uv_x, 1.0f - (uv_y + 1.0f/_num_chars_y));
		glm::vec2 uv_b_r = glm::vec2(uv_x + 1.0f/_num_chars_x, 1.0f - (uv_y + 1.0f/_num_chars_y));  

		//Create first triangle of Quad
		textureCoords.push_back(uv_t_l);
		textureCoords.push_back(uv_b_l);
		textureCoords.push_back(uv_t_r);

		//Create second triangle of Quad
		textureCoords.push_back(uv_b_r);
		textureCoords.push_back(uv_t_r);
		textureCoords.push_back(uv_b_l);
	}

	glUseProgram(_programID);

	useTexture(_textureID, ACTIVE_TEXTURE, "fontTextureSampler");

	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, _UV_buffer);
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), &textureCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _UV_buffer);
	glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}



GLuint TextGenerator::setUpTexture(char* texture_file_name, GLuint ActiveTextureNum) {
        //-1 is "error" case
        GLuint textureID = -1;
        if(texture_file_name != NULL){
                //TEXTURE
                glActiveTexture(GL_TEXTURE0 + ActiveTextureNum);
                //glload must be initialized for glimg texture creation to work.
                if(glload::LoadFunctions() == glload::LS_LOAD_FAILED)
                        std::cout << "Failed To Load";
                //Loading succeeded. Now load a texture.
                try
                {
                        //TODO cache image...
                        std::auto_ptr<glimg::ImageSet> pImgSet(glimg::loaders::stb::LoadFromFile(texture_file_name));
                        textureID = glimg::CreateTexture(pImgSet.get(), 0);
                }
                catch(glimg::ImageCreationException  &e)
                {
                        //Image file loading failed.
                        std::cout << "Image creation failed";
                }
        }
        return textureID;
}


void TextGenerator::useTexture(GLuint textureID, GLuint ActiveTextureNum, const char* SamplerName) {
        if(textureID != -1){
                glActiveTexture(GL_TEXTURE0 + ActiveTextureNum);
                glBindTexture(GL_TEXTURE_2D, textureID);
                GLuint TextureID  = glGetUniformLocation(_programID, SamplerName);
                glUniform1i(TextureID, ActiveTextureNum);
        }

}

