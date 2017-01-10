#include <stdio.h>
#include <string.h>
#include <math.h>

#include "OGL\Include\glew.h"
#include "OGL\Include\freeglut.h"
#include "OGL\Include\ogldev_util.h"
#include "OGL\Include\ogldev_math_3d.h"

//Variables
GLuint VBO;
const GLchar * pVSFileName = "Assets/Shaders/vertex.shader";
const GLchar * pFSFileName = "Assets/Shaders/fragment.shader";
GLuint gScaleLocation;
GLuint gWorldLocation;

//Functions

static void AddShader(GLuint ShaderProgram, const char * pShaderSource, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);
	if (!ShaderObj)
	{
		fprintf(stderr, "Error creating shader object of type %d.\n", ShaderType);
		exit(1);
	}

	const GLchar * p[1];
	p[0] = pShaderSource;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderSource);
	glShaderSource(ShaderObj, 1, p, Lengths);

	glCompileShader(ShaderObj);

	GLint Success = 0;
	GLchar InfoLog[1024] = { 0 };
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader: %s of type: %d.\n", InfoLog, ShaderType);
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
		fprintf(stderr, "Error creatinf Shdaer Program.\n");
		exit(1);
	}

	if (!ReadFile(pVSFileName, vs))
	{
		fprintf(stderr, "Error reading vertex shader.\n");
		exit(1);
	}

	if (!ReadFile(pFSFileName, fs))
	{
		fprintf(stderr, "Error reading fragment shader.\n");
		exit(1);
	}

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	glLinkProgram(ShaderProgram);

	GLint Success = 0;
	GLchar InfoLog[1024] = { 0 };
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error linking shader program: %s.\n", InfoLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);

	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error validating shader program: %s.\n", InfoLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);

	//Getting uniform variables
	gScaleLocation = glGetUniformLocation(ShaderProgram, "gScale");
	assert(gScaleLocation != 0xFFFFFFFF);

	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	assert(gWorldLocation != 0xFFFFFFFF);
}

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

	//Scaling
	static float scale = 0.0f;
	scale += 0.02f;
	glUniform1f(gScaleLocation, sinf(scale));

	//Translation and Rotation
	Matrix4f world;
	world.m[0][0] = cosf(scale); world.m[0][1] = -sinf(scale); world.m[0][2] = 0.0f; world.m[0][3] = sinf(scale);
	world.m[1][0] = sinf(scale); world.m[1][1] = cosf(scale); world.m[1][2] = 0.0f; world.m[1][3] = 0.0f;
	world.m[2][0] = 0.0f; world.m[2][1] = 0.0f; world.m[2][2] = 1.0f; world.m[2][3] = 0.0f;
	world.m[3][0] = 0.0f; world.m[3][1] = 0.0f; world.m[3][2] = 0.0f; world.m[3][3] = 1.0f;
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &world.m[0][0]);

	//Draw Triangle
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

int main(int argc, char ** argv)
{
	//Initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("OGL PRactice 06");

	GLenum glewResult = glewInit();
	if (glewResult != GLEW_OK)
	{
		fprintf(stderr, "Error initialization GLEW: %s.\n", glewGetErrorString(glewResult));
		exit(1);
	}

	printf("OpenGL Version: %s.\n", glGetString(GL_VERSION));

	InitializeGlutCallbacks();

	//Draw
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

	CreateVertexBuffer();

	CompileShaders();

	//Update
	glutMainLoop();
	
	return 0;
}