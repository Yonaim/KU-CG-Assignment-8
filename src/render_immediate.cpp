#include <GL/glew.h>
#include <GL/glu.h> // gluLookAt
#include "load_mesh.hpp"

void applyTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	glTranslatef(0.1f, -1.0f, -1.5f);
	glScalef(10.f, 10.f, 10.f);
}
void setupLightMat()
{
	const GLfloat ka[4] = {1, 1, 1, 1}, kd[4] = {1, 1, 1, 1};
	const GLfloat Ia[4]         = {0.2f, 0.2f, 0.2f, 1};
	const GLfloat light_diff[4] = {1, 1, 1, 1};
	const GLfloat light_pos[4]  = {-1, -1, -1, 0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ia);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
	glDisable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
}

void render_immediate()
{
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
}
