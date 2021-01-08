//
// Created by HYPERBOOK on 29.12.2020.
//

#pragma once

#include "glad/glad.h"


class Quad {
public:
    Quad();
    ~Quad();
    void draw();
private:
    GLuint vao_;
    GLuint buffer_[2];
    GLuint diffuse_texture_;
};

