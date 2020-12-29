//
// Created by HYPERBOOK on 29.12.2020.
//

#pragma once

#include "glad/glad.h"

class Pyramid  {
public:
    Pyramid();
    ~Pyramid();
    void draw();
private:
    GLuint vao_;
    GLuint buffer_[2];
};

