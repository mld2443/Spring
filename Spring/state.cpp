//
//  state.cpp
//  Spring
//
//  Created by Matthew Dillard on 11/4/15.
//  Copyright © 2015 Matthew Dillard. All rights reserved.
//

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include "state.h"

std::vector<state::vector::strut> state::edges;
v3<double> state::gravity;

state::vector state::derive(const vector& S, const double t) {
    vector Sdot;
    
    return S;
}

void state::vector::draw_edges() {
    for (auto &e : edges){
        double ratio = (vertices[e.v2].pos - vertices[e.v1].pos).abs() / e.l;
        
        if (ratio <= 1.0)
            glColor3d(ratio, ratio, 1.0);
        else
            glColor3d(1.0/ratio, 0.5 + (0.5/ratio), 0.5 + (0.5/ratio));
        
        glBegin(GL_LINE_STRIP); {
            glVertex3d(vertices[e.v1].pos.x, vertices[e.v1].pos.y, vertices[e.v1].pos.z);
            glVertex3d(vertices[e.v2].pos.x, vertices[e.v2].pos.y, vertices[e.v2].pos.z);
        } glEnd();
    }
}

state::vector state::vector::operator+(const vector& s) const {
    vector rvalue;
    for (unsigned long i = 0; i < vertices.size(); ++i)
        rvalue.vertices.push_back(vertices[i] + s.vertices[i]);
    
    return rvalue;
}

state::vector state::vector::operator-(const vector& s) const {
    vector rvalue;
    for (unsigned long i = 0; i < vertices.size(); ++i)
        rvalue.vertices.push_back(vertices[i] - s.vertices[i]);
    
    return rvalue;
}

state::vector state::vector::operator*(const double value) const {
    vector rvalue;
    for (unsigned long i = 0; i < vertices.size(); ++i)
        rvalue.vertices.push_back(vertices[i] * value);
    
    return rvalue;
}

state::vector* state::vector::operator=(const vector& s) {
    vertices.clear();
    for (auto &p : s.vertices)
        vertices.push_back(mesh::vertex<double>(p));
    return this;
}

