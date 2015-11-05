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

terrarium::terrarium(const unsigned int sim, const double r): radius(r), ldepth(200), lwidth(50), rdepth(350), rwidth(90) {
    switch (sim) {
        case 1:
            gen::flag(S, {20, 20, 10}, {10, 20, 20}, 10, 10);
            
            state::gravity = v3<double>(0.0, -20, 0.0);
            state::wind = v3<double>(0.0, 0.0, 0.0);
            state::dragc = 0.0;
            state::liftc = 0.0;
            break;
            
        case 2:
            gen::flag(S, {20, 20, 10}, {10, 21, 20}, 10, 10, true);
            
            state::gravity = v3<double>(0.0, -20, 0.0);
            state::wind = v3<double>(0.0, 0.0, 0.0);
            state::dragc = 0.01;
            state::liftc = 0.005;
            break;
            
        case 3:
            gen::rddh(S, {20,15,20}, {0,0,0}, 1, 1.0, 5);
            
            state::gravity = v3<double>(0.0, 0.0, 0.0);
            state::wind = v3<double>(0.0, 0.0, 0.0);
            state::dragc = 0.01;
            state::liftc = 0.005;
            break;
            
        case 4:
            gen::icosahedron(S, {20,15,20}, {0,0,0}, 25, 1.0, 0.2);
            
            state::gravity = v3<double>(0.0, 0.0, 0.0);
            state::wind = v3<double>(0.0, 0.0, 0.0);
            state::dragc = 0.0;
            state::liftc = 0.0;
            break;
            
        case 5:
            gen::flag(S, {20, 20, 10}, {10, 20, 20}, 10, 10, true);
            gen::icosahedron(S, {15,10,15}, {0,0,0}, 5, 1, 1, true);
            
            state::gravity = v3<double>(0.0, -20, 0.0);
            state::wind = v3<double>(0.0, 0.0, 0.0);
            state::dragc = 0.01;
            state::liftc = 0.005;
            break;
            
        case 6:
            gen::flag(S, {20, 20, 10}, {10, 20, 20}, 10, 10);
            gen::icosahedron(S, {0.02,5,0.02}, {3,2,3}, 15.0/3.0, 0.1, 3.0/4.0);
            
            state::gravity = v3<double>(0.0, -20, 0.0);
            state::wind = v3<double>(0.0, 0.0, 0.0);
            state::dragc = 0.0;
            state::liftc = 0.0;
            break;
            
        case 7:
            gen::flag(S, {20, 20, 10}, {10, 20, 20}, 10, 10);
            gen::rddh(S, {0,5,0}, {3,2,3}, 15.0/3.0, 0.1, 3.0/4.0);
            
            state::gravity = v3<double>(0.0, 0.0, 0.0);
            state::wind = v3<double>(0.0, 0.0, 0.0);
            state::dragc = 0.0;
            state::liftc = 0.0;
            break;
            
        default:
            gen::test(S);
            
            state::gravity = v3<double>(0.0, -9.81, 0.0);
            state::wind = v3<double>(0.0, 0.0, 0.0);
            state::dragc = 0.0;
            state::liftc = 0.0;
            break;
    }
}

void terrarium::draw_lights() {
    GLfloat light0color[] = { 0.9, 0.9, 0.8 };
    GLfloat light0pos[] = { static_cast<GLfloat>(radius/2), static_cast<GLfloat>(radius), static_cast<GLfloat>(radius) };
    GLfloat light1color[] = { 0.4, 0.4, 0.2 };
    GLfloat light1pos[] = { 0.0, 5.0, static_cast<GLfloat>(radius) };
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
    
    glColor3fv(water_ambient);
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
    
    glColor3fv(gnd_ambient);
    glBegin(GL_QUADS); {
        glNormal3f(0,1,0);
        glVertex3f(-radius, 0, -radius);
        glVertex3f(-radius, 0, ldepth - lwidth);
        glVertex3f(radius, 0, rdepth - rwidth);
        glVertex3f(radius, 0, -radius);
    } glEnd();
}

void terrarium::step(const double t) {
    state::vector Snew = RK::four(S, t, state::derive);
    
    S.calc_intersects(Snew);
    
    S = Snew;
}

void terrarium::draw(bool wireframe) {
    glClearColor(0.7, 1.0, 1.0, 0.0);

    draw_lights();
    draw_ground();
    
    GLfloat black[] = { 0.0, 0.0, 0.0 };
    GLfloat dark[] = { 0.3, 0.3, 0.3 };
    GLfloat flat[] = { 0.0 };
    GLfloat dull[] = { 1.0 };
    
    if (wireframe) {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
        glMaterialfv(GL_FRONT, GL_SPECULAR, black);
        glMaterialfv(GL_FRONT, GL_SHININESS, flat);

        S.draw_vertices();
        S.draw_edges();
    }
    else {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, dark);
        glMaterialfv(GL_FRONT, GL_SPECULAR, dark);
        glMaterialfv(GL_FRONT, GL_SHININESS, dull);

        S.draw_faces();
    }
}
