// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Camera.h"
#include "LoadShaders.h"
#include "CubePrimitive.h"
#include "ProceduralSphere.h"
// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/random.hpp"
using namespace glm;
using namespace std;
GLuint _mainWindow;
GLuint _shaderProg;
GLuint _mvpMatrixID;
CubePrimitive _cube;
ProceduralSphere _sphere;
Camera _camera;
float _rotationX = 0.0;
float _rotationY = 0.0;
vector<ProceduralSphere> spheres;
int _curMouseX, _curMouseY;
void DrawSpheres(glm::mat4 vp)
{
	for(int i = 0; i< spheres.size(); i++)
	{
		glm::mat4 Model = glm::mat4(1);
		Model = glm::rotate(Model,(_rotationX),glm::vec3(0,1,0));
		Model = glm::rotate(Model,(_rotationY),glm::vec3(-1,0,0));
		Model = Model * spheres.at(i).GetModelTransform();
		glm::mat4 MVP = vp * Model;
		glUseProgram(_shaderProg);
		glUniformMatrix4fv(_mvpMatrixID, 1, GL_FALSE, &MVP[0][0]);
		spheres.at(i).Draw();
	}

}
void MoveSpheres()
{
	for(int i = 0; i< spheres.size(); i++)
	{
		spheres.at(i).Update();
	}
}
void display()
{
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model      = glm::mat4(1.0f);
		Model = glm::rotate(Model,(_rotationX),glm::vec3(0,1,0));
		Model = glm::rotate(Model,(_rotationY),glm::vec3(-1,0,0));
		
        // Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP  = _camera.GetVP() * Model; // Remember, matrix multiplication is the other way around
		MoveSpheres();
	glUseProgram(_shaderProg);
	glUniformMatrix4fv(_mvpMatrixID, 1, GL_FALSE, &MVP[0][0]);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	_cube.Draw();
	DrawSpheres(_camera.GetVP());
	//_sphere.Draw();
	glUseProgram(0);
	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	/*case 'w':
		_camera.MoveForward();
		break;
	case 's':
		_camera.MoveBackward();
		break;
	case 'a':
		_camera.MoveLeft();
		break;
	case 'd':
		_camera.MoveRight();
		break;*/
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}
void mouseFunc(int button, int state, int x, int y)
{
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN){
		 _curMouseX = x;
		 _curMouseY = y;
		}
		
		break;
	}
}

void mouseMotionFunc(int x, int y)
{
	
	int _mousedx = x - _curMouseX;
	int _mousedy = y - _curMouseY;
	_rotationX += _mousedx;
	_rotationY += _mousedy;
	_curMouseX = x;
	_curMouseY = y;
}
void mouseWheel(int wheel, int direction, int x, int y)
{
	switch(direction)
	{
	case 1: //forward
		_camera.MoveForward();
		break;
	case -1:
		_camera.MoveBackward();
		break;
	}
}
void InitializeShaders()
{
	_shaderProg = LoadShaders("../src/shaders/vertex.glsl", "../src/shaders/frag.glsl");
	_mvpMatrixID = glGetUniformLocation(_shaderProg,"MVP");
}

void InitializeAll()
{
	_camera.init(vec3(0,0,-5), vec3(0,0,0), vec3(0,1,0));
	_camera.SetProjection(45.0f, 4.0f / 4.0f, 0.1f, 100.0f);
	// Enable depth test
glEnable(GL_DEPTH_TEST);
// Accept fragment if it closer to the camera than the former one
glDepthFunc(GL_LESS);
	_cube.Init();
	for(int i=0; i< 20; i++)
	{
		ProceduralSphere newSphere;
		newSphere.CreateSphere(0.1f, 20,20);
		spheres.push_back(newSphere);
	}
		
		//_sphere.CreateSphere(2.0f, 20, 20);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(600,600);
	glutInitWindowPosition(0,0);
	_mainWindow = glutCreateWindow("Spheres");
	// Initialize GLEW
        glewExperimental = true; // Needed for core profile
        if (glewInit() != GLEW_OK) {
                fprintf(stderr, "Failed to initialize GLEW\n");
                return -1;
        }
	InitializeShaders();
	InitializeAll();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
	glutMouseWheelFunc(mouseWheel);
	glutMainLoop();
	return 0;
}