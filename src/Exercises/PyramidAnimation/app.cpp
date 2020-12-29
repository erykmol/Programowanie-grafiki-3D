//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"

void SimpleShapeApplication::init() {

    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    auto u_modifiers_index = glGetUniformBlockIndex(program, "Modifiers");
    if (u_modifiers_index == GL_INVALID_INDEX) {
        std::cout << "Cannot find Modifiers uniform block in program" << std::endl;
    }
    else {
        glUniformBlockBinding(program, u_modifiers_index, 0);
    }

    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX) {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    } else {
        glUniformBlockBinding(program, u_transformations_index, 1);
    }

    set_camera(new Camera);
    set_controler(new CameraControler(camera()));

//    GLuint ubo_handle(0u);
//    glGenBuffers(1, &ubo_handle);
//    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle);
//    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
//    float strength = 0.5f;
//    float light[3] = {1.0f, 0.2f, 0.1f};
//
//    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
//    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), light);

    glGenBuffers(1, &u_pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    pyramid = new Pyramid;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    int w, h;
    std::tie(w, h) = frame_buffer_size();
    camera()->perspective(glm::pi<float>() / 4.0, (float)w / h, 0.1f, 100.0f);
    camera()->look_at(glm::vec3{0.0, 0.0, 30.0}, glm::vec3{0.5f, 0.5f, 0.0f}, glm::vec3{0.0, 0.0, 1.0});

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glViewport(0, 0, w, h);
    glUseProgram(program);
    start_ = std::chrono::steady_clock::now();
    rotation_period = 4.0f;
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h);
    camera()->set_aspect((float) w / h);
}

void SimpleShapeApplication::frame() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(now - start_).count();
    auto rotation_angle = glm::two_pi<float>()*elapsed_time/rotation_period;

    glm::mat4 main_TR = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));

    glm::mat4 R = glm::rotate(glm::mat4(1.0f), rotation_angle ,glm::vec3(0.0f, 0.0f, 1.0f));
    glBindBuffer(GL_UNIFORM_BUFFER, this->u_pvm_buffer_);

    auto rotated = this->camera()->view() * R;

    auto orbital_rotation_period = 20.0f;
    auto orbital_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/orbital_rotation_period;
    auto a = 10.0f;
    auto b = 8.0f;
    float x = a*cos(orbital_rotation_angle);
    float y = b*sin(orbital_rotation_angle);

    glm::mat4 O = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,0.0f));
    auto orbited = this->camera()->view() * O * R;

    auto moon_rotation_period = 10.0f;
    auto moon_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/moon_rotation_period;
    float x_m = 3.0f*cos(moon_rotation_angle);
    float y_m = 3.0f*sin(moon_rotation_angle);
    glm::mat4 O_moon = glm::translate(glm::mat4(1.0f), glm::vec3(x_m,y_m,0));
    glm::mat4 R_moon = glm::rotate(glm::mat4(1.0f), moon_rotation_angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(0.794f, 0.794f, 0.794f));

    glm::mat4 moon_transform = orbited * O_moon * R_moon * S;
    auto orbited2 = this->camera()->view() * O;
    auto satellite_rotation_period = 2.0f;
    auto satellite_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/satellite_rotation_period;
    float x_sat = 1.5*cos(satellite_rotation_angle);
    float z_sat = 1.5*sin(satellite_rotation_angle);
    glm::mat4 O_sat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, x_sat, z_sat));
    glm::mat4 R_sat = glm::rotate(glm::mat4(1.0f), satellite_rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 S_sat = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
    glm::mat4 sat_transform = orbited2 * O_sat * R_sat * S_sat;

    glm::mat4 transforms[] = {
            rotated,
            orbited,
            moon_transform,
            sat_transform
    };

    for (glm::mat4 transform : transforms) {
        glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
        auto PVM = camera()->projection() * camera()->view() * transform * main_TR;
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        pyramid->draw();

//        auto PVM = transform * this->camera()->projection() * this->camera()->view();
//        glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
//        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
//        glBindBuffer(GL_UNIFORM_BUFFER, 0);
//this->pyramid->draw();
    }

//    glBindVertexArray(vao_);
//    glEnable(GL_DEPTH_TEST);
//    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
////    glDrawArrays(GL_TRIANGLES, 0, 9);
//    glBindVertexArray(0);
//
//    auto PVM = camera()->projection() * this->camera()->view();
//    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
//    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
//    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controler_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controler_->LMB_released(x, y);
    }
}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controler_) {
        controler_->mouse_moved(x, y);
    }
}