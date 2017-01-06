#include <stdio.h>
#include "OGL\Include\glew.h"
#include "OGL\Include\freeglut.h"
#include "OGL\Include\ogldev_math_3d.h"

//Global variables
GLuint VBO;

void CreateVertexBuffer()
{
	Vector3f vertices[3];
	vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);
	vertices[1] = Vector3f(1.0f, 0.0f, 0.0f);
	vertices[2] = Vector3f(1.0f, 1.0f, 0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glGenBuffers(1, &VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
}

int main(int argc, char ** argv)
{
	//Initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("OGL Practice 03");

	GLenum glewResult = glewInit();
	if (glewResult != GLEW_OK)
	{
		fprintf(stderr, "Error: %s.\n", glewGetErrorString(glewResult));
		return 1;
	}

	InitializeGlutCallbacks();
	
	//Draw
	CreateVertexBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Update
	glutMainLoop();

	return 0;
}