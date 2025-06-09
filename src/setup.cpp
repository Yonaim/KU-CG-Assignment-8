#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Renderparams.hpp"

extern GLuint vao, vbo, ebo, prog;
extern RenderParams params;

#define WIN_WIDTH  512
#define WIN_HEIGHT 512

void setupOpenGL()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW init fail\n";
		exit(-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *win
		= glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT,
						   "Assignment 7 - Phong Shading", nullptr, nullptr);
	if (!win)
	{
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(win);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW init fail\n";
		exit(-1);
	}
	glEnable(GL_DEPTH_TEST);
}

void initGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glEnable(GL_NORMALIZE);           // 또는 GL_RESCALE_NORMAL (uniform 스케일일 때 성능↑)
    glShadeModel(GL_SMOOTH);  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0); // 과제 파라미터
}

void applyTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	glTranslatef(0.1f, -1.0f, -1.5f); // ← 이동 먼저
	glScalef(10.0f, 10.0f, 10.0f);    // ← 그다음 스케일
}

void setupLightMat()
{
	/*----------------------- 1) 머티리얼 -----------------------*/
	const GLfloat ka[]      = {1.0f, 1.0f, 1.0f, 1.0f}; // Ambient  (white)
	const GLfloat kd[]      = {1.0f, 1.0f, 1.0f, 1.0f}; // Diffuse  (white)
	const GLfloat ks[]      = {0.3f, 0.3f, 0.3f, 1.0f}; // Specular (subtle)
	const GLfloat shininess = 20.0f;                    // Highlight size

	/*----------------------- 2) 글로벌 앰비언트 -----------------*/
	const GLfloat Ia[] = {0.2f, 0.2f, 0.2f, 1.0f}; // Scene ambient

	/*----------------------- 3) 라이트 파라미터 ----------------*/
	const GLfloat light_amb[]  = {0.0f, 0.0f, 0.0f, 1.0f};
	const GLfloat light_diff[] = {1.0f, 1.0f, 1.0f, 1.0f};
	const GLfloat light_spec[] = {1.0f, 1.0f, 1.0f, 1.0f};

	/*-----------------------------------------------------------*/
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/* (A) 카메라 공간에서 조명의 위치/방향 고정 ───────────────*/
	glPushMatrix();   // 🔸 현재 MODELVIEW 저장
	glLoadIdentity(); // 🔸 카메라 공간 = eye space

	// 카메라 바로 앞쪽( -Z 축 방향 )에서 비추는 ‘헤드라이트’ 형태
	const GLfloat light_pos[] = {1.0f, 1.0f, 1.0f, 0.0f}; // w=0 → directional
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	// 조명 색상 설정
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);

	glPopMatrix(); // 🔸 원래 MODELVIEW 복원
	/*-----------------------------------------------------------*/

	/* (B) 글로벌 앰비언트 적용 */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ia);

	/* (C) 머티리얼 파라미터 적용 */\
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	/* (D) 기타 렌더 상태 */
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	/* (E) 노멀 재정규화(스케일링 보정) */
	glEnable(GL_NORMALIZE); // 또는 GL_RESCALE_NORMAL
}
