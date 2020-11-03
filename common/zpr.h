#ifndef ZPR_H
#define ZPR_H

/*
 * Zoom-pan-rotate mouse manipulation module for GLUT
 *
 * Originally written by Nigel Stewart
 *
 * Hacked quite a bit by Mikael Kalms
 *
 * Hacked quite a bit more by Ingemar Ragnemalm 2013, for using
 * VectorUtils3.
 */

// MS
#include <windows.h>
#include <stdio.h>
#include <glew.h>
#include <wglew.h>


#include "MicroGlut.h"
#include "VectorUtils3.h"



void zprInit(mat4 *viewMatrix, vec3 cam, vec3 point);

void updateCameraMatrix(mat4 *matrix);    // matrix is passed as a parameter when both cameraMatrix
                                                 // and camera and lookAtPoint have to be transformed.
                                                 //  if matrix = NULL, only cameraMatrix is updated

void zprMouse(int button, int state, int x, int y);  // records mouse buttons' state
void zprKey(unsigned char key, int x, int y); // camera control with keys. Updates viewMatrix
void zprMouseFunc(int x, int y);



#endif
