//***************************************************************************
// DC-101090467 (C) 2018 All Rights Reserved.
//
// Open GL Texture Basics
//
// Implements 2 rotating cubes with textures.
//
// Description:
//	- Click run to see the results
//  - A & D keys - Move left right
//	- W & S keys - Move forward and backwards
//	- R & F keys - Move up and down
//*****************************************************************************

#include <iostream>
#include <list>
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <SOIL.h>
#include "glm\gtx\rotate_vector.hpp"

using namespace glm; // Removes the need to write glm:: (Not Recommended).

GLuint NUM_ROWS;
GLuint NUM_COLUMS;

//Buffer objects
GLuint VAO;

GLuint VertexBuffer;
GLuint ColorBuffer;
GLuint ElementBuffer;
GLuint TextureBuffer;
GLuint UVBuffer;

//Matrices
std::list<mat4> modelViewStack;
mat4 projectionMatrix;
mat4 modelMatrix;
mat4 viewMatrix;

//Shader pointers
GLuint uMVP;
GLuint TextureID;

//Camera Variables
GLfloat cameraSpeed = 0.2f;
vec3 cameraPosition(0.0f, 0.0f, 6.0f);
vec3 initialCameraPosition = cameraPosition;

//Transformation Variables
GLfloat CurrentDegree = 0.0f;
GLfloat TransformBy = 0.0f;
GLfloat CurrentDepth = 1.0f;
GLfloat CurrentHight = -0.45f;
GLfloat rotationDelta = 2.0f;

//Image Texture.
unsigned char* image;
GLint textureWidth, textureHeight;

//Keyboard Input
bool keys[256];

GLfloat UVcoords[] =
{
	0.33333,0.66666,
	0.66666,0.66663,
	0.66666,0.33333,
	0.33333,0.33333,
	0.33333,0.66666,
	0.66666,0.66663,
	0.66666,0.33333,
	0.33333,0.33333,
	0.33333,0.66666,
	0.66666,0.66663,
	0.66666,0.33333,
	0.33333,0.33333,
	0.33333,0.66666,
	0.66666,0.66663,
	0.66666,0.33333,
	0.33333,0.33333,
	0.33333,0.66666,
	0.66666,0.66663,
	0.66666,0.33333,
	0.33333,0.33333,
	0.33333,0.66666,
	0.66666,0.66663,
	0.66666,0.33333,
	0.33333,0.33333,

};

//Object data
GLfloat vertices[] =
{
	-0.9f, -0.9f,  0.9f,//front
	0.9f, -0.9f,  0.9f,	//front
	0.9f,  0.9f,  0.9f,	//front
	-0.9f,  0.9f,  0.9f,//front

	0.9f, -0.9f,  0.9f, //right
	0.9f, -0.9f, -0.9f,	//right
	0.9f,  0.9f, -0.9f,	//right
	0.9f,  0.9f,  0.9f,	//right

	-0.9f, -0.9f,  -0.9f,//back
	0.9f, -0.9f,  -0.9f,//back
	0.9f,  0.9f,  -0.9f,//back
	-0.9f,  0.9f, -0.9f,//back

	-0.9f, -0.9f,  0.9f,//Left
	-0.9f, -0.9f, -0.9f,//Left
	-0.9f,  0.9f, -0.9f,//Left
	-0.9f,  0.9f,  0.9f,//Left

	0.9f,  0.9f,  0.9f,	//top
	0.9f,  0.9f,  -0.9f,//top
	-0.9f,  0.9f, -0.9f,//top
	-0.9f,  0.9f,  0.9f,//top

	0.9f,  -0.9f,  0.9f,//Bottom
	0.9f,  -0.9f,  -0.9f,//Bottom
	-0.9f, -0.9f, -0.9f,//Bottom
	-0.9f, -0.9f,  0.9f,//Bottom



	
};

GLfloat colors[] =
{
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f
};

GLuint indices[] =
{
	0, 1, 2, //front
	0, 2, 3,
	4,5,6,//Right
	4,6,7,
	8,9,10,//Back
	8,10,11,
	12, 13, 14, //left
	12, 14, 15,
	16,17,18, //top
	16,18,19,
	20,21,22,//bottom
	20,22,23
};
//Camera Function
void CameraMovement()
{
	if (keys['w'])
	{
		if (cameraPosition.z >= -10)
			cameraPosition.z -= cameraSpeed;
	}
	else if (keys['s'])
	{
		if (cameraPosition.z <= 10)
			cameraPosition.z += cameraSpeed;
	}
	else if (keys['a'])
	{
		if (cameraPosition.x >= -10)
			cameraPosition.x -= cameraSpeed;
	}
	else if (keys['d'])
	{
		if (cameraPosition.x <= 10)
			cameraPosition.x += cameraSpeed;
	}
	else if (keys['r'])
	{
		if (cameraPosition.y <= 10)
			cameraPosition.y += cameraSpeed;
	}
	else if (keys['f'])
	{
		if (cameraPosition.y >= -10)
			cameraPosition.y -= cameraSpeed;
	}
	viewMatrix = lookAt(cameraPosition, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
}

//Frame Rate Limiter
void RefreshFrame(int id) {
	CameraMovement();

	glutPostRedisplay();

	glutTimerFunc(33.3, RefreshFrame, 0);
}



//Keyboard Functions
void onKeyDown(unsigned char key, int x, int y) {
	std::cout << key;
	switch (key)
	{
	case 'w':
	case 'W':
		keys['w'] = true;
		break;
	case 'a':
	case 'A':
		keys['a'] = true;
		break;
	case 's':
	case 'S':
		keys['s'] = true;
		break;
	case 'd':
	case 'D':
		keys['d'] = true;
		break;

	case 'r':
	case 'R':
		keys['r'] = true;
		break;

	case 'f':
	case 'F':
		keys['f'] = true;
		break;
	default:
		break;
	}
}

void onKeyUp(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
	case 'W':
		keys['w'] = false;
		break;
	case 'a':
	case 'A':
		keys['a'] = false;
		break;
	case 's':
	case 'S':
		keys['s'] = false;
		break;
	case 'd':
	case 'D':
		keys['d'] = false;
		break;
	case 'r':
	case 'R':
		keys['r'] = false;
		break;

	case 'f':
	case 'F':
		keys['f'] = false;
		break;
	default:
		break;
	}
}

//Pushes MVP matrix to shader program.
void uploadMatrixToShader()
{
	mat4 modelViewProjection = mat4(1.0f);
	modelViewProjection = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(uMVP, 1, GL_FALSE, value_ptr(modelViewProjection));
}

//Set up shader program, attributes and matrices.
void init()
{
	glEnable(GL_DEPTH_TEST);

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	//Program
	GLuint program = LoadShaders(shaders);

	glUseProgram(program);

	//Vertex Array Buffer
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	//Uniforms
	uMVP = glGetUniformLocation(program, "uMVP");

	TextureID = glGetUniformLocation(program, "texture0");

	//UV Coords
	glGenBuffers(1, &UVBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer); //Vertex buffer

	glBufferData(GL_ARRAY_BUFFER, sizeof(UVcoords), UVcoords, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(2);


	//Image texture
	image = SOIL_load_image("rubiksTexture.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);

	//Texture Buffer

	glGenTextures(1, &TextureBuffer);

	glBindTexture(GL_TEXTURE_2D, TextureBuffer);

	glActiveTexture(GL_TEXTURE0); //Indicates the texture you are binding to.

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glUniform1i(TextureID, 0);

	//Cube Buffers

	glGenBuffers(1, &VertexBuffer);

	glGenBuffers(1, &ColorBuffer);

	glGenBuffers(1, &ElementBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer); //Vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer); //Index list
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer); //Color buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

	//Initializing matrices
	modelMatrix = mat4(1.0f);

	viewMatrix = lookAt(initialCameraPosition, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	projectionMatrix = mat4(1.0f);
	projectionMatrix = perspective(radians(80.0f), 1.0f, 0.1f, 100.0f);

	uploadMatrixToShader();
}

void DrawCube(glm::vec3 transform , vec3 RotationAxis, float Degree) {
	modelMatrix = mat4(1.0f);

	modelMatrix = glm::translate(modelMatrix, transform);

	modelMatrix = glm::rotate(modelMatrix, glm::radians(Degree), RotationAxis);

	uploadMatrixToShader();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}



//Render function.
void display()
{

	CurrentHight = -0.45f;

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawCube(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),-CurrentDegree);

	DrawCube(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), CurrentDegree);

	CurrentDegree += rotationDelta;

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glCullFace(GL_BACK);

	glutInitWindowSize(1028, 1028);

	glutCreateWindow("OpenGL_TextureBasics");

	glewInit();

	init();

	glutDisplayFunc(display);

	glutKeyboardFunc(onKeyDown);

	glutKeyboardUpFunc(onKeyUp);

	glutTimerFunc(33.3, RefreshFrame, 0);

	glutMainLoop();
}