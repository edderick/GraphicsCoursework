#include "skybox.hpp"


Skybox::Skybox(const char* obj_file_name, GLuint programID, Viewer* viewer, GLenum draw_mode) : Object(obj_file_name, programID, viewer, draw_mode) { }

glm::mat4 Skybox::makeModelMatrix(){
        glm::mat4 model = glm::mat4(1.f);
        model = glm::scale(model, glm::vec3(50.f, 50.f, 50.f));
        return model;
}

glm::mat4 Skybox::makeViewMatrix(){
        glm::mat4 view = glm::mat4(1.f);
	_viewer->getPosition();
        view = glm::lookAt(glm::vec3(0,0,0),_viewer->getDirection(), _viewer->getUp());
        return view;
}

glm::mat4 Skybox::makeProjectionMatrix(){
        glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.f);
        return projection;
}

