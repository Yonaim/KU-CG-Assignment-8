#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include "load_mesh.hpp" // gPositions, gNormals, gTriangles

#include "Renderparams.hpp"
#include "setup.hpp"
RenderParams params;

#define WIN_WIDTH  512
#define WIN_HEIGHT 512

void init_timer();
void display();
void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0); // 60Hz
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("OpenGL Bunny Immediate Mode");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "GLEW init failed\n");
		return -1;
	}

	init_timer();
    initGL(); // OpenGL 초기화
	glEnable(
		GL_DEPTH_TEST); // fixed-function 파이프라인이면 깊이 테스트도 켜 주세요

	load_mesh("bunny.obj");
	glutDisplayFunc(display);
	glutTimerFunc(16, timer, 0);
	glutMainLoop();
}
