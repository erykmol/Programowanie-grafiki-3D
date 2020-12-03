//
// Created by pbialas on 05.08.2020.
//


#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Application/application.h"
#include "Application/utils.h"

#include "glad/glad.h"
#include "Camera.h"

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void framebuffer_resize_callback(int w, int h) override;
    void scroll_callback(double xoffset, double yoffset) override {
        Application::scroll_callback(xoffset, yoffset);
        camera()->zoom(yoffset / 30.0f);
    }
    void init() override;;

    void frame() override;

    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }

    ~SimpleShapeApplication() {
        if (this->camera_) {
            delete this->camera_;
        }
    }

    void scroll_callback(double xoffset, double yoffset) override {
        Application::scroll_callback(xoffset, yoffset);
        camera()->zoom(-yoffset / 30.0f);
    }



private:
    GLuint vao_;

    GLuint u_pvm_buffer_;
    Camera *camera_;
};