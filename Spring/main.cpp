//
//  main.cpp
//  Spring
//
//  Created by Matthew Dillard on 11/2/15.
//  
//

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <stdlib.h>

#include "terrarium.h"
#include "v3.h"

// timing information
unsigned int steps_per_frame;
double fps, timestep;
bool play;

// camera info
GLfloat c_ratio;
float movespeed;
int camPhi, camTheta;
v3<float> eyePos, lookDir;

// scene data
int window;
terrarium t;
bool wires;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    
    gluLookAt(eyePos.x, eyePos.y, eyePos.z, (eyePos+lookDir).x, (eyePos+lookDir).y, (eyePos+lookDir).z, 0, 1, 0);
    
    t.draw(wires);
    
    glPopMatrix();
    glutSwapBuffers();
}

void init() {
    // Enable Z-buffering, backface culling, and lighting
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, c_ratio, 1, 6000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // simulation timing
    steps_per_frame = 1;
    fps = 60.0;
    timestep = 2.5/(fps * (double)steps_per_frame);
    play = true;
    
    // looking and movement
    movespeed = 10.0;
    camPhi = 0;
    camTheta = 315;
    eyePos = v3<float>(0,10,0);
    lookDir = v3<float>(1,0,0).rotateZ(camPhi).rotateY(camTheta);
    
    wires = false;
    t = terrarium(4);
}

void resizeFunc(GLint newWidth, GLint newHeight) {
    c_ratio = (GLfloat)newWidth/(GLfloat)newHeight;
    init();
    glutPostRedisplay();
}

void newFrame(const int id) {
    glutTimerFunc(1000.0/fps, newFrame, 1);
    
    if (play) {
        unsigned int step = 0;
        
        while (step < steps_per_frame) {
            t.step(timestep);
            step++;
        }

        glutPostRedisplay();
    }
}

void key(const unsigned char c, const int x, const int y) {
    //space - play/pause
    //c - toggle free camera
    //1 - toggle which flock to follow, (make sure not to press this unless there's at least 2 flocks)
    //wasd - move camera in free camera
    //arrows - look aroud
    //esc - exit
    switch (c) {
        case ' ':
            play = !play;
            break;
        case 27:
            glutDestroyWindow(window);
            exit(0);
            break;
            
        case 9:
            wires = !wires;
            glutPostRedisplay();
            break;
            
        case 'w':
            eyePos += lookDir * movespeed;
            glutPostRedisplay();
            break;
        case 'a':
            eyePos -= lookDir.cross(v3<float>(0,1,0)).normalize() * movespeed;
            glutPostRedisplay();
            break;
        case 's':
            eyePos -= lookDir * movespeed;
            glutPostRedisplay();
            break;
        case 'd':
            eyePos += lookDir.cross(v3<float>(0,1,0)).normalize() * movespeed;
            glutPostRedisplay();
            break;
            
        case 'W':
            eyePos += lookDir * 2.0 * movespeed;
            glutPostRedisplay();
            break;
        case 'A':
            eyePos -= lookDir.cross(v3<float>(0,1,0)).normalize() * 2.0 * movespeed;
            glutPostRedisplay();
            break;
        case 'S':
            eyePos -= lookDir * 2.0 * movespeed;
            glutPostRedisplay();
            break;
        case 'D':
            eyePos += lookDir.cross(v3<float>(0,1,0)).normalize() * 2.0 * movespeed;
            glutPostRedisplay();
            break;
            
        default:
            break;
    }
}

void specialKey(const int c, const int x, const int y){
    switch (c) {
        case GLUT_KEY_UP:
            if (camPhi + 5 < 90) {
                camPhi += 5;
                lookDir = v3<float>(1,0,0).rotateZ(camPhi).rotateY(camTheta);
                glutPostRedisplay();
            }
            break;
        case GLUT_KEY_DOWN:
            if (camPhi - 5 > -90) {
                camPhi -= 5;
                lookDir = v3<float>(1,0,0).rotateZ(camPhi).rotateY(camTheta);
                glutPostRedisplay();
            }
            break;
        case GLUT_KEY_LEFT:
            camTheta += 5;
            if (camTheta < 0)
                camTheta += 360;
            lookDir = v3<float>(1,0,0).rotateZ(camPhi).rotateY(camTheta);
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            camTheta -= 5;
            if (camTheta >= 360)
                camTheta -= 360;
            lookDir = v3<float>(1,0,0).rotateZ(camPhi).rotateY(camTheta);
            glutPostRedisplay();
            break;
            
        default:
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    window = glutCreateWindow("Fly your colors");
    glutDisplayFunc(display);
    glutFullScreen();
    glutKeyboardFunc(key);
    glutSpecialFunc(specialKey);
    glutTimerFunc(1000.0/fps, newFrame, 0);
    glutReshapeFunc(resizeFunc);
    
    init();
    
    glutMainLoop();
    return 0;
}
