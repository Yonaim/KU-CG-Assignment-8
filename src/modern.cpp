#include "modern.hpp"
#include <vector>
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>

GLuint vao = 0, vbo_pos = 0, vbo_nor = 0, ebo = 0;

// Q2: 메쉬를 GPU에 업로드 (std::vector<Vector3>와 Triangle을 사용)
void upload_mesh_to_gpu()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Vertex Positions
	glGenBuffers(1, &vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * gPositions.size(),
				 gPositions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
						  (void *)0); // aPos → location=0
	glEnableVertexAttribArray(0);

	// Vertex Normals
	glGenBuffers(1, &vbo_nor);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_nor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * gNormals.size(),
				 gNormals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
						  (void *)0); // aNor → location=1
	glEnableVertexAttribArray(1);

	// Triangle Indices (EBO)
	std::vector<unsigned int> indices;
	indices.reserve(3 * gTriangles.size());
	for (const auto &tri : gTriangles)
	{
		indices.push_back(tri.indices[0]);
		indices.push_back(tri.indices[1]);
		indices.push_back(tri.indices[2]);
	}
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
				 indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

GLuint prog = 0;

GLuint compile_shader(GLenum type, const char *src)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	GLint status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		char buf[1024];
		glGetShaderInfoLog(shader, 1024, NULL, buf);
		fprintf(stderr, "Shader compile error: %s\n", buf);
		exit(-1);
	}
	return shader;
}

void setup_shader_program(const char *vsrc, const char *fsrc)
{
	GLuint vs = compile_shader(GL_VERTEX_SHADER, vsrc);
	GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fsrc);
	prog      = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);
	GLint status = 0;
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (!status)
	{
		char buf[1024];
		glGetProgramInfoLog(prog, 1024, NULL, buf);
		fprintf(stderr, "Program link error: %s\n", buf);
		exit(-1);
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
}
