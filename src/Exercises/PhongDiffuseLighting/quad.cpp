//
// Created by damia on 18.12.2020.
//

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/src/stb/stb_image.h"
#include "quad.h"
#include <vector>
#include <string>
#include <iostream>

Quad::Quad() {
    /*
        Tu dodajemy kod tworzący bufory indeksów i wierzchołków oraz obiekt VAO,
        Potem ładujemy dane wierzchołków i indeksow do buforów  i  podłaczamy je do atrubutów w szaderze
   */

    std::vector<GLfloat> vertices = {
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    std::vector<GLushort> indices = {
            3,1,2,2,1,0
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
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(sizeof(GLfloat)*3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(sizeof(GLfloat)*5));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    int width, height, n_channels;
    auto texture_filename = std::string(PROJECT_DIR) + "/Textures/silver.png";
    uint8_t  *data = stbi_load(texture_filename.c_str(), &width, &height, &n_channels, 0);
    if (*data)
    {
        std::cout << width << std::endl;
        std::cout << height << std::endl;
    }
    else {
        std::cout << "Cannot load file" << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Quad::~Quad() {
    //Tu usuwamy VAO i bufory
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(2, buffer_);
}

void Quad::draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,diffuse_texture_);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
