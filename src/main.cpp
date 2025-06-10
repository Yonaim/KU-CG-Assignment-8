#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "load_mesh.hpp"
#include "modern.hpp"

#define WIN_WIDTH  512
#define WIN_HEIGHT 512

float  gTotalTimeElapsed = 0;
int    gTotalFrames      = 0;
GLuint gTimer;

std::string read_text_file(const char *filename)
{
	std::ifstream ifs(filename);
	if (!ifs.is_open())
	{
		std::cerr << "Cannot open shader file: " << filename << std::endl;
		exit(-1);
	}
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	return buffer.str();
}

void start_timing()
{
	glBeginQuery(GL_TIME_ELAPSED, gTimer);
}
float stop_timing()
{
	glEndQuery(GL_TIME_ELAPSED);
	GLint available = GL_FALSE;
	while (available == GL_FALSE)
		glGetQueryObjectiv(gTimer, GL_QUERY_RESULT_AVAILABLE, &available);
	GLint result;
	glGetQueryObjectiv(gTimer, GL_QUERY_RESULT, &result);
	return result / (1000.0f * 1000.0f * 1000.0f);
}

void render(GLFWwindow *window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	start_timing();

	glUseProgram(prog);

	glm::mat4 model = glm::mat4(1.0f);
	model           = glm::translate(model, glm::vec3(0.1f, -1.0f, -1.5f));
	model           = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	glm::mat4 view  = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1),
								  glm::vec3(0, 1, 0));
	glm::mat4 proj  = glm::frustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1000.0f);

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

	float dt = stop_timing();
	++gTotalFrames;
	gTotalTimeElapsed += dt;
	char title[128];
	sprintf(title, "OpenGL Bunny (Q2): %.2f FPS",
			gTotalFrames / gTotalTimeElapsed);
	glfwSetWindowTitle(window, title);
}

int main()
{
	// GLFW 초기화
	if (!glfwInit())
	{
		std::cerr << "GLFW init fail\n";
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(
		WIN_WIDTH, WIN_HEIGHT, "OpenGL Bunny Modern (GLFW)", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW init fail\n";
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glGenQueries(1, &gTimer);
	glClearColor(0.05f, 0.05f, 0.08f, 1.0f);

	// 메쉬 및 셰이더 준비
	load_mesh("bunny.obj");
	std::string vsrc_str = read_text_file("./shader/bunny.vert");
	std::string fsrc_str = read_text_file("./shader/bunny.frag");
	setup_shader_program(vsrc_str.c_str(), fsrc_str.c_str());
	upload_mesh_to_gpu();

	// 메인 루프
	while (!glfwWindowShouldClose(window))
	{
		render(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
// 