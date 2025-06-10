#pragma once
#include "load_mesh.hpp"
#include <GL/glew.h>

void          upload_mesh_to_gpu();
void          setup_shader_program(const char *vsrc, const char *fsrc);
extern GLuint vao, prog;
