/*
	How to use this code:

	Call init_timer before starting rendering, i.e., before calling
	glutMainLoop. Then, make sure your display function is organized
	roughly as the example below.
*/

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <float.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "load_mesh.hpp"
#include "Renderparams.hpp"
#include "setup.hpp"

float  					gTotalTimeElapsed 	= 0;
int 					gTotalFrames		= 0;
GLuint 					gTimer;

void init_timer()
{
	glGenQueries(1, &gTimer);
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

	float timeElapsed = result / (1000.0f * 1000.0f * 1000.0f);
	return timeElapsed;
}

/*
	Your display function should look roughly like the following.
*/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	start_timing();

	applyTransform();
	setupLightMat();

	glBegin(GL_TRIANGLES);
	for (const auto &tri : gTriangles)
		for (int j = 0; j < 3; ++j)
		{
			const auto &n = gNormals[tri.indices[j]];
			const auto &v = gPositions[tri.indices[j]];
			glNormal3f(n.x, n.y, n.z);
			glVertex3f(v.x, v.y, v.z);
		}
	glEnd();

	float dt = stop_timing();

	++gTotalFrames;
	gTotalTimeElapsed += dt;
	char title[128];
	sprintf(title, "OpenGL Bunny Immediate Mode: %.2f FPS",
			gTotalFrames / gTotalTimeElapsed);
	glutSetWindowTitle(title);

	glutSwapBuffers();
}
