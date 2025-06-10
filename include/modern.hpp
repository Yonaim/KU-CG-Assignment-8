#pragma once
#include <GL/glew.h>

// 전역 핸들 선언 (구현은 modern.cpp)
extern GLuint vao, vbo_pos, vbo_nor, ebo, prog;

// 함수 선언
void upload_mesh_to_gpu();
void setup_shader_program(const char *vsrc, const char *fsrc);
