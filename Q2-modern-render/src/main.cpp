#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include "load_mesh.hpp"
#include "render_immediate.hpp"
#include "render_modern.hpp"

enum RenderMode
{
	MODE_IMMEDIATE,
	MODE_MODERN
};

 //const RenderMode MODE = MODE_IMMEDIATE;
 const RenderMode MODE = MODE_MODERN;

#define WIN_WIDTH  512
#define WIN_HEIGHT 512

float  gTotalTimeElapsed = 0;
int    gTotalFrames      = 0;
GLuint gTimer;

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
	GLuint64 result;
	glGetQueryObjectui64v(gTimer, GL_QUERY_RESULT, &result);
	return result / 1e9f;
}
void init_immediate();

	void render(GLFWwindow *window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	start_timing();

	if (MODE == MODE_IMMEDIATE)
    {
        init_immediate();
		render_immediate(window);
    }
	else
		render_modern();

	float dt = stop_timing();
	gTotalTimeElapsed += dt;
	++gTotalFrames;
	char buf[128];
	sprintf(buf, "OpenGL Bunny (%s): %.2f FPS",
			(MODE == MODE_IMMEDIATE ? "Immediate" : "Modern"),
			gTotalFrames / gTotalTimeElapsed);
	glfwSetWindowTitle(window, buf);
}

int main()
{

	glfwInit();

	if (MODE == MODE_IMMEDIATE)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	}
	else
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	GLFWwindow *window
		= glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "OpenGL Bunny", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glGenQueries(1, &gTimer);
	glClearColor(0.05f, 0.05f, 0.08f, 1.0f);

	load_mesh("bunny.obj");
	if (MODE == MODE_MODERN)
		render_modern_init();

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
