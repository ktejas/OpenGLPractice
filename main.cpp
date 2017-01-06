#include <stdio.h>
#include "OGL\Include\glew.h"
#include "OGL\Include\freeglut.h"
#include "OGL\Include\ogldev_math_3d.h"

//Global variables
GLuint VBO;

void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glGenBuffers(1, &VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_POINTS, 0, 1);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
}

void CreateVertexBuffer()
{
	Vector3f vertices[1];
	vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

int main(int argc, char ** argv)
{
	//Initialize GLUT and GLEW
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OGL Practice 02");

	InitializeGlutCallbacks();

	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		fprintf(stderr, "Error: %s.\n", glewGetErrorString(result));
		return 1;
	}

	//Draw stuff
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer();

	//Update stuff
	glutMainLoop();

	return 0;
}