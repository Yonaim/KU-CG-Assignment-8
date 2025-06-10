#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "load_mesh.hpp"
#include "modern.hpp"

// 전역 VAO/프로그램
extern GLuint vao, prog;

#include <fstream>
#include <sstream>
#include <string>

std::string read_text_file(const char *filename)
{
	std::ifstream ifs(filename);
	if (!ifs.is_open())
	{
		fprintf(stderr, "Cannot open shader file: %s\n", filename);
		exit(-1);
	}
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	return buffer.str();
}

void render_modern_init()
{
	// 셰이더·VAO 준비
	std::string vsrc_str = read_text_file("./shader/bunny.vert");
	std::string fsrc_str = read_text_file("./shader/bunny.frag");
	setup_shader_program(vsrc_str.c_str(), fsrc_str.c_str());
	upload_mesh_to_gpu();
}

void render_modern()
{
	glUseProgram(prog);
	glm::mat4 model
		= glm::translate(glm::mat4(1), glm::vec3(0.1f, -1.0f, -1.5f));
	model          = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1),
								 glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::frustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1000.0f);

	glUniformMatrix4fv(glGetUniformLocation(prog, "model"), 1, GL_FALSE,
					   glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(prog, "view"), 1, GL_FALSE,
					   glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(prog, "proj"), 1, GL_FALSE,
					   glm::value_ptr(proj));
	glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
	glUniform3fv(glGetUniformLocation(prog, "lightDir"), 1,
				 glm::value_ptr(lightDir));
	glm::vec3 viewPos = glm::vec3(0, 0, 0);
	glUniform3fv(glGetUniformLocation(prog, "viewPos"), 1,
				 glm::value_ptr(viewPos));
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3 * gTriangles.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
