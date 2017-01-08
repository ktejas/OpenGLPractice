#include <stdio.h>
#include <string.h>
#include <math.h>

#include "OGL\Include\glew.h"
#include "OGL\Include\freeglut.h"
#include "OGL\Include\ogldev_math_3d.h"
#include "OGL\Include\ogldev_util.h"

//Variables
GLuint VBO;
GLuint gScaleLocation;
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

	//Uniform Variables
	static float Scale = 0.0f;
	Scale += 0.01f;
	glUniform1f(gScaleLocation, sinf(Scale));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
}

void AddShader(GLuint ShaderProgram, const char * pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (!ShaderObj)
	{
		fprintf(stderr, "Error: Can't create Shader Object of type %d.", ShaderType);
		exit(1);
	}

	const GLchar * p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);

	glCompileShader(ShaderObj);

	GLint Success = 0;
	GLchar ErrorLog[1024];
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(ShaderObj, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error: Can't Get Shader Object of type %d: %s.\n", ShaderType, ErrorLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

void CompileShaders()
{
	string vs;
	string fs;

	GLuint ShaderProgram = glCreateProgram();
	if (!ShaderProgram)
	{
		fprintf(stderr, "Error: Can't create Shader Program.");
		exit(1);
	}

	if (!ReadFile(pVSFileName, vs))
	{
		fprintf(stderr, "Error: Reading Vertex Shader.\n");
		exit(1);
	}

	if (!ReadFile(pFSFileName, fs))
	{
		fprintf(stderr, "Error: Reading Fragment Shader.\n");
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
		fprintf(stderr, "Error: Can't Link Shader Program: %s.\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error: Can't validate shader program: %s.\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);

	glGetUniformLocation(gScaleLocation, "gScale");
	assert(gScaleLocation != 0xFFFFFFFF);
}

int main(int argc, char ** argv)
{
	//Initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("OGL Practice 04");

	GLenum glewResult = glewInit();
	if (glewResult != GLEW_OK)
	{
		fprintf(stderr, "Error: Can't initialize GLEW: %s.\n", glewGetErrorString(glewResult));
		exit(1);
	}

	printf("OpenGL Version: %s.\n", glGetString(GL_VERSION));

	//Draw
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	InitializeGlutCallbacks();

	CreateVertexBuffer();

	CompileShaders();

	//Update
	glutMainLoop();

	return 0;
}