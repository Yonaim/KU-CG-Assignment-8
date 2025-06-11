#include "setup.hpp"
#include <GL/glew.h>
#include <GL/gl.h>s
#include <GL/glut.h>
#include <GL/glu.h>

void initGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
	glEnable(GL_NORMALIZE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);
}
