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

    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX) {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    } else {
        glUniformBlockBinding(program, u_transformations_index, 1);
    }

    auto  u_diffuse_map_location = glGetUniformLocation(program, "diffuse_map");
    if(u_diffuse_map_location==-1) {
        std::cerr<<"Cannot find uniform diffuse_map\n";
    } else {
        glUniform1ui(u_diffuse_map_location,0);
    }

    auto u_light_index = glGetUniformBlockIndex(program, "Light");
    if(u_light_index == -1) {
        std::cerr << "Cannot find uniform Light" << std::endl;
    } else {
        glUniformBlockBinding(program, u_light_index, 2);
    }

    set_camera(new Camera);
    set_controler(new CameraControler(camera()));

    glGenBuffers(1, &u_pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4)+3*sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    auto V = camera()->view();

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &camera()->projection()[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &V[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &u_light_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);

    light_.position = V*glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
    light_.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    light_.a = glm::vec4(1.0f, 0.0f, 1.0f, 0.0f);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &light_.position[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &light_.color[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), &light_.a[0]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_light_buffer_);

    quad = new Quad;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    int w, h;
    std::tie(w, h) = frame_buffer_size();
    camera()->perspective(glm::pi<float>() / 4.0, (float)w / h, 0.1f, 100.0f);
    camera()->look_at(glm::vec3(-2.5f, 2.0f, -2.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glViewport(0, 0, w, h);
    glUseProgram(program);
//    start_ = std::chrono::steady_clock::now();
//    rotation_period = 4.0f;
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h);
    camera()->set_aspect((float) w / h);
}

void SimpleShapeApplication::frame() {

    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    auto P = camera()->projection();
    auto VM = camera()->view();
    auto R = glm::mat3(VM);
    auto N = glm::transpose(glm::inverse(R));

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &P[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &N[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), &N[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec3), &N[2]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    light_.position = VM*glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &light_.position[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    quad->draw();
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

void SimpleShapeApplication::cleanup() {
    delete quad;
}