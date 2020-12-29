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
#include "CameraControler.h"

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

    void set_controler(CameraControler *controler) { controler_ = controler; }

    Camera *camera() { return camera_; }

    void mouse_button_callback(int button, int action, int mods);

    void cursor_position_callback(double x, double y);

    ~SimpleShapeApplication() {
        if (this->camera_) {
            delete this->camera_;
        }
    }

private:
    GLuint vao_;

    GLuint u_pvm_buffer_;
    Camera *camera_;
    CameraControler *controler_;
};