//
// Created by pbialas on 05.08.2020.
//


#pragma once

#include <vector>


#include "Application/application.h"
#include "Application/utils.h"

#include "glad/glad.h"
#include "camera.h"
#include "CameraControler.h"
#include "pyramid.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <chrono>

class SimpleShapeApplication : public xe::Application {
public:
    void framebuffer_resize_callback(int w, int h) override;
    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;;

    void frame() override;
    void scroll_callback(double xoffset, double yoffset) override {
        Application::scroll_callback(xoffset, yoffset);
        camera()->zoom(-yoffset / 30.0f);
    }
    void set_controler(CameraControler *controler) { controler_ = controler; }
    void mouse_button_callback(int button, int action, int mods);

    void cursor_position_callback(double x, double y);

    Pyramid *pyramid;
    std::chrono::steady_clock::time_point start_;

private:
    GLuint vao_;
    GLuint u_pvm_buffer_;

    Camera *camera_;
    CameraControler *controler_;
    float rotation_period;
};