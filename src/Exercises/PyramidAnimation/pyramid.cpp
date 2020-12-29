//
// Created by HYPERBOOK on 29.12.2020.
//


#include "pyramid.h"
#include <vector>

Pyramid::Pyramid() {

    std::vector<GLfloat> vertices = {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

            0.5f, 1.0f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,

            0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,

            0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    };

    std::vector<GLushort> indices = {
            0,2,1,3,1,2,4,5,6,7,8,9,10,11,12,13,14,15
    };

    glGenBuffers(2, buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(sizeof(GLfloat)*3));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Pyramid::~Pyramid() {
    //Tu usuwamy VAO i bufory
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(2, buffer_);
}

void Pyramid::draw() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
    glBindVertexArray(0);
}