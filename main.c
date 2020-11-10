// Project by Petra Gunnarsdotter
// Hair Project

// Built on Lab2s and demos of TSBK03 ground created by Ingemar 2010 and
// Geometry shader demo by Ingemar Ragnemalm

// Built for windows 

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifdef __APPLE__
// Mac
	#include <OpenGL/gl3.h>
//uses framework Cocoa
#else
	#ifdef WIN32
// MS
		#include <stdio.h>
		#include <glew.h>
	#else
// Linux
		#include <GL/gl.h>
	#endif
#endif

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include <string.h>

// vertex array object
unsigned int bunnyVertexArrayObjID;
Model* m;
// Reference to shader program
#define kNumPrograms 5
GLuint program[kNumPrograms];
int currentProgram;
GLuint tex;

mat4 worldToViewMatrix, modelToWorldMatrix;
mat4 modelViewMatrix, projectionMatrix;

void init(void)
{
	int i;
	mat4 camMatrix;
	mat4 projectionMatrix;

	// GL inits
	glClearColor(1, 1, 1, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_TRUE);

	// Load and compile shaders
	program[0] = loadShadersG("shaders/minimal.vert", "shaders/minimal.frag", "shaders/passthrough.gs");
	program[1] = loadShadersG("shaders/minimal.vert", "shaders/minimal.frag", "shaders/flatshading.gs");
	program[2] = loadShadersG("shaders/minimal.vert", "shaders/minimal.frag", "shaders/cracking.gs");
	program[3] = loadShadersG("shaders/minimal.vert", "shaders/black.frag", "shaders/wireframe.gs");
	program[4] = loadShadersG("shaders/hairvert.vert", "shaders/black.frag", "shaders/hair.gs");
	
	currentProgram = 1;
	glUseProgram(program[currentProgram]);

	// Upload geometry to the GPU:
	m = LoadModelPlus("objects/stanford-bunny.obj");
	CenterModel(m);
	ScaleModel(m, 1, 1, 1);
	ReloadModelData(m);
	
	modelToWorldMatrix = IdentityMatrix();
	worldToViewMatrix = lookAt(0, 0.0, 3.5, 0, 0, 0, 0, 1, 0);
	projectionMatrix = frustum(-0.4, 0.4, -0.4, 0.4, 1.0, 300.0);

	for (i = 0; i < kNumPrograms; i++)
	{
		glUseProgram(program[i]);

		glUniformMatrix4fv(glGetUniformLocation(program[i], "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

		//		glUniformMatrix4fv(glGetUniformLocation(program[currentProgram], "worldToViewMatrix"), 1, GL_TRUE, worldToViewMatrix.m);

		glUniform1i(glGetUniformLocation(program[i], "tex"), 0); // Texture unit 0
		LoadTGATextureSimple("textures/maskros512.tga", &tex); // 5c
	}
}

GLfloat a = 0.0;

///////////////////////////////////////////////
//		 D R A W  C Y L I N D E R
// Desc:	sätter bone positionen i vertex shadern


void display(void)
{

	// clear the screen
	glClearColor(0.4, 0.4, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT + GL_DEPTH_BUFFER_BIT);
	//Draw the shape first then the normals/hair
	if (currentProgram == 4) {
		glUseProgram(program[1]);
		mat4 modelViewMatrix = Mult(worldToViewMatrix, modelToWorldMatrix);
		glUniformMatrix4fv(glGetUniformLocation(program[0], "modelViewMatrix"), 1, GL_TRUE, modelViewMatrix.m);
		DrawModel(m, program[0], "inPosition", "inNormal", "inTexCoord");
	}
	glUseProgram(program[currentProgram]);
	mat4 modelViewMatrix = Mult(worldToViewMatrix, modelToWorldMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program[currentProgram], "worldToViewMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program[currentProgram], "modelViewMatrix"), 1, GL_TRUE, modelViewMatrix.m);
	DrawModel(m, program[currentProgram], "inPosition", "inNormal", "inTexCoord");
	glutSwapBuffers();
	glutPostRedisplay();
}

void OnTimer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(20, &OnTimer, value);
}

void keyboardFunc( unsigned char key, int x, int y)
{
  if(key == 27)	//Esc
    exit(1);
}

void reshape(GLsizei w, GLsizei h)
{
	vec3 cam = {16,0,30};
	vec3 look = {16,0,0};

    glViewport(0, 0, w, h);
    GLfloat ratio = (GLfloat) w / (GLfloat) h;
    projectionMatrix = perspective(90, ratio, 0.1, 1000);
	modelViewMatrix  = lookAt(cam.x, cam.y, cam.z,
											look.x, look.y, look.z, 
											0,1,0);
}


void key(unsigned char key, int x, int y)
{
	if (key >= '1' && key <= '1' + kNumPrograms)
	{
		currentProgram = key - '1';
	}
}

int prevx = 0, prevy = 0;

void mouseUpDown(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		prevx = x;
		prevy = y;
	}
}

void mouseDragged(int x, int y)
{
	vec3 p;
	mat4 m;

	// This is a simple and IMHO really nice trackball system:
	// You must have two things, the worldToViewMatrix and the modelToWorldMatrix
	// (and the latter is modified).

	// Use the movement direction to create an orthogonal rotation axis
	p.y = x - prevx;
	p.x = -(prevy - y);
	p.z = 0;

	// Transform the view coordinate rotation axis to world coordinates!
	mat3 wv3 = mat4tomat3(worldToViewMatrix);
	p = MultMat3Vec3(InvertMat3(wv3), p);

	// Create a rotation around this axis and premultiply it on the model-to-world matrix
	m = ArbRotate(p, sqrt(p.x * p.x + p.y * p.y) / 50.0);
	modelToWorldMatrix = Mult(m, modelToWorldMatrix);

	prevx = x;
	prevy = y;

	glutPostRedisplay();
}

/////////////////////////////////////////
//		M A I N
//
int main(int argc, char **argv)
{
  glutInit(&argc, argv);

  glutInitWindowSize(800, 800); //change to H and W
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitContextVersion(3, 2); // Might not be needed in Linux
  glutCreateWindow("GL3 geometry shading example"); //change

  // initiering
#ifdef WIN32
  glewInit();
#endif

  //glutDisplayFunc(DisplayWindow);
  glutTimerFunc(20, &OnTimer, 0);
  glutKeyboardFunc( keyboardFunc ); 
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  init();
  glutKeyboardFunc(key);
  glutMouseFunc(mouseUpDown);
  glutMotionFunc(mouseDragged);
 
  reshape(800, 800);
  glutMainLoop();
  exit(0);
}
