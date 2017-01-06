#include <stdio.h>
#include <string.h>
#include "OGL\Include\glew.h"
#include "OGL\Include\freeglut.h"
#include "OGL\Include\ogldev_math_3d.h"
#include "OGL\Include\ogldev_util.h"


//Global variables
GLuint VBO;
const char * pVSFileName = "Assets\\Shaders\\vertex.shader";
const char * pFSFileName = "Assets\\Shaders\\fragment.shader";

//Functions
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

void AddShader(GLuint ShaderProgram, const char * pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0)
	{
		fprintf(stderr, "Error: Can't create shader object of type: %d.\n", ShaderObj);
		exit(1);
	}

	const GLchar * p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);

	glShaderSource(ShaderObj, 1, p, Lengths);

	glCompileShader(ShaderObj);

	GLint Success = 0;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &Success);
	if(!Success)
	{
		GLchar ErrorLog[1024] = { 0 };
		glGetShaderInfoLog(ShaderObj, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error compile shader type %d: %s.\n", ShaderObj, ErrorLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);

}

void CompileShaders()
{
	string vs, fs;

	GLuint ShaderProgram = glCreateProgram();

	if (!ShaderProgram)
	{
		fprintf(stderr, "Error: Can't create shader program.\n");
		exit(1);
	}

	if (!ReadFile(pVSFileName, vs))
	{
		fprintf(stderr, "Error: Can't read Vertex Shader.");
		exit(1);
	}

	if (!ReadFile(pFSFileName, fs))
	{
		fprintf(stderr, "Error: Can't read Fragment Shader.");
		exit(1);
	}

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	glLinkProgram(ShaderProgram);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (!Success)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: %s.\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);

	if (!Success)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error invalid shader program: %s.\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);
}

int main(int argc, char ** argv)
{
	//Initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(3840,2160);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("OGL Practice 04");

	InitializeGlutCallbacks();

	GLenum glewResult = glewInit();
	if (glewResult != GLEW_OK)
	{
		fprintf(stderr, "Error initializing GLEW: %s.", glewGetErrorString(glewResult));
		exit(1);
	}

	//Draw
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	CreateVertexBuffer();

	CompileShaders();

	//Update
	glutMainLoop();

	return 0;
}