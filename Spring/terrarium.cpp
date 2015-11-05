//
//  terrarium.cpp
//  Spring
//
//  Created by Matthew Dillard on 11/2/15.
//  
//

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include "terrarium.h"
#include "generator.h"
#include "rungekutta.h"

void terrarium::draw_lights() {
    GLfloat light0color[] = { 0.9, 0.9, 0.8 };
    GLfloat light0pos[] = { 5000, 10000, 5000 };
    GLfloat light1color[] = { 0.4, 0.4, 0.2 };
    GLfloat light1pos[] = { 300, 300, 300 };
    glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0color);
    glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1color);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1color);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1color);
}

void terrarium::draw_ground() {
    GLfloat gnd_ambient[] = { 0.2, 0.4, 0.2 };
    GLfloat gnd_diffuse[] = { 0.0, 0.2, 0.0 };
    GLfloat gnd_specular[] = { 0.0, 0.0, 0.0 };
    GLfloat gnd_shininess[] = { 0.0 };
    
    GLfloat water_ambient[] = { 0.2, 0.2, 0.3 };
    GLfloat water_diffuse[] = { 0.1, 0.1, 0.3 };
    GLfloat water_specular[] = { 0.5, 0.5, 1.0 };
    GLfloat water_shininess[] = { 10.0 };
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, gnd_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gnd_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, gnd_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, gnd_shininess);
    
    glBegin(GL_QUADS); {
        glNormal3f(0,1,0);
        glVertex3f(radius, 0, radius);
        glVertex3f(radius, 0, rdepth + rwidth);
        glVertex3f(-radius, 0, ldepth + lwidth);
        glVertex3f(-radius, 0, radius);
    } glEnd();
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, water_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, water_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, water_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, water_shininess);
    
    glBegin(GL_QUADS); {
        glNormal3f(0,1,0);
        glVertex3f(-radius, 0, ldepth + lwidth);
        glVertex3f(radius, 0, rdepth + rwidth);
        glVertex3f(radius, 0, rdepth - rwidth);
        glVertex3f(-radius, 0, ldepth - lwidth);
    } glEnd();
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, gnd_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gnd_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, gnd_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, gnd_shininess);
    
    glBegin(GL_QUADS); {
        glNormal3f(0,1,0);
        glVertex3f(-radius, 0, -radius);
        glVertex3f(-radius, 0, ldepth - lwidth);
        glVertex3f(radius, 0, rdepth - rwidth);
        glVertex3f(radius, 0, -radius);
    } glEnd();
}

void terrarium::draw() {
    glClearColor(0.7, 1.0, 1.0, 0.0);

    draw_lights();
    draw_ground();
    
    state.draw_vertices();
    state.draw_edges();
}

terrarium::terrarium(const double r): radius(r), ldepth(200), lwidth(50), rdepth(350), rwidth(90) {
    state = gen::test();
    state::gravity = v3<double>(0.0, -9.81, 0.0);
}
