#include "skybox.hpp"


Skybox::Skybox(GeometryGenerator* gg, GLuint programID, Viewer* viewer, GLenum draw_mode) : Object(gg, programID, viewer, draw_mode) { }

glm::mat4 Skybox::makeModelMatrix(){
        glm::mat4 model = glm::mat4(1.f);
        model = glm::scale(model, glm::vec3(100.f, 100.f, 100.f));
        return model;
}

glm::mat4 Skybox::makeViewMatrix(){
        glm::mat4 view = glm::mat4(1.f);
	_viewer->getPosition();
        view = glm::lookAt(glm::vec3(0,0,0),_viewer->getDirection(), _viewer->getUp());
        return view;
}

glm::mat4 Skybox::makeProjectionMatrix(){
        return Object::makeProjectionMatrix();
}

