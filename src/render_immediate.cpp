
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <vector>
#include "load_mesh.hpp"

void setupLightMat()
{
	const GLfloat Ia[4] = {0.2f, 0.2f, 0.2f, 1};

	const GLfloat L_amb[4]  = {0.0f, 0.0f, 0.0f, 1};
	const GLfloat L_diff[4] = {1.0f, 1.0f, 1.0f, 1};
	const GLfloat L_spec[4] = {1.0f, 1.0f, 1.0f, 1};

	const GLfloat ka[4]     = {1, 1, 1, 1};
	const GLfloat kd[4]     = {1, 1, 1, 1};
	const GLfloat ks[4]     = {0.3f, 0.3f, 0.3f, 1};
	const GLfloat shininess = 20.0f;

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ia);
	glLightfv(GL_LIGHT0, GL_AMBIENT, L_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, L_diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, L_spec);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glEnable(GL_NORMALIZE);
	glDisable(GL_CULL_FACE);
}

void render_immediate(GLFWwindow *window)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	float aspect = width / (float)height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.1f * aspect, 0.1f * aspect, -0.1f, 0.1f, 0.1f, 1000.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	const GLfloat light_pos[4] = {1, 1, 1, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	glTranslatef(0.1f, -1.0f, -1.5f);
	glScalef(10.0f, 10.0f, 10.0f);

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
}

void init_immediate()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
}
