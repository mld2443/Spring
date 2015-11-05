//
//  state.cpp
//  Spring
//
//  Created by Matthew Dillard on 11/4/15.
//
//

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include "state.h"

#define RHO 0.9
#define MU 0.1

std::vector<mesh::strut<double>> state::edges;
std::vector<mesh::face<double>> state::faces;
v3<double> state::gravity;
v3<double> state::wind;
double state::dragc;
double state::liftc;

state::vector state::derive(const vector& S, const double t) {
    vector Sdot;
    
    for (auto &v : S) {
        if (v)
            Sdot.add_vertex({v.vel, {0,0,0}, v.mass, v.fixed});
        else {
            v3<double> accel;
            accel += gravity * t;
            Sdot.add_vertex({v.vel, accel, v.mass, v.fixed});
        }
    }
    
    for (auto &face : faces) {
        v3<double> P0 = S[face.i0].pos, P1 = S[face.i1].pos, P2 = S[face.i2].pos;
        v3<double> V0 = S[face.i0].vel, V1 = S[face.i1].vel, V2 = S[face.i2].vel;
        v3<double> normal = (P0 - P1).cross(P2 - P1).normalize();
        
        v3<double> Vf = (V0 + V1 + V2) / 3;
        v3<double> Vr = Vf - wind;
        double A = (P0 - P1).cross(P2 - P1).abs() / 2;
        
        v3<double> Fd, Fl;
        if (Vr.x != 0.0 && Vr.y != 0.0 && Vr.z != 0.0) {
            Fd = Vr.normalize() * -dragc * A*(normal.dot(Vr));
            v3<double> ncv = normal.cross(Vr);
            if (ncv.x != 0.0 && ncv.y != 0.0 && ncv.z != 0.0)
                Fl = (Vr.cross(ncv.normalize())) * -liftc * A*(normal.dot(Vr));
        }
        
        double r0 = (P1 - P0).angle(P2 - P0)/180;
        double r1 = (P2 - P1).angle(P0 - P1)/180;
        double r2 = (P0 - P2).angle(P1 - P2)/180;
        
        if (!S[face.i0]) Sdot[face.i0].vel += ((Fd + Fl) * r0)/Sdot[face.i0].mass;
        if (!S[face.i1]) Sdot[face.i1].vel += ((Fd + Fl) * r1)/Sdot[face.i1].mass;
        if (!S[face.i2]) Sdot[face.i2].vel += ((Fd + Fl) * r2)/Sdot[face.i2].mass;
    }
    
    for (auto &e : edges) {
        const mesh::vertex<double> *Vi = &S[e.i1], *Vj = &S[e.i2];
        
        double Lij = (Vj->pos - Vi->pos).abs();
        v3<double> Xij = (Vj->pos - Vi->pos).normalize();
        
        v3<double> force = v3<double>();
        force += Xij * e.k * (Lij - e.l);
        force += Xij * e.d * (Vj->vel - Vi->vel).dot(Xij);
        
        if (!Sdot[e.i1]) {
            Sdot[e.i1].vel += (force/Vi->mass);
            if (Sdot[e.i2])
                Sdot[e.i1].vel += (force/Vi->mass);
        }
        if (!Sdot[e.i2]) {
            Sdot[e.i2].vel -= (force/Vj->mass);
            if (Sdot[e.i1])
                Sdot[e.i2].vel -= (force/Vj->mass);
        }
    }
    
    return Sdot;
}

void state::vector::calc_intersects(vector& Snew) {
    for (unsigned int i = 0; i < size(); ++i) {
        for (auto &f : faces){
            const v3<double> c1 = Snew[f.i0].pos, c2 = Snew[f.i1].pos, c3 = Snew[f.i2].pos;
            const v3<double> normal = ((c1 - c2).cross(c3 - c2)).normalize();
            if (f.i0 != i && f.i1 != i && f.i2 != i) {
                if (f.cross_plane(c1, normal, vertices[i].pos, Snew[i].pos)){
                    v3<double> intersect = f.find_intersect(c1, normal, vertices[i].pos, Snew[i].pos);
                    if (f.projection_intersection(normal, c1, c2, c3, intersect)){
                        v3<double> d = normal * f.dist_from_plane(c1, normal, Snew[i].pos);
                        Snew[i].pos = Snew[i].pos - (d * (1 + RHO));
                        
                        v3<double> v_norm = normal * (Snew[i].vel.dot(normal));
                        v3<double> v_tan = Snew[i].vel - v_norm;
                        Snew[i].vel = (v_norm * -RHO) + (v_tan * (1 - MU));
                    }
                }
            }
        }
    }
}

void state::vector::draw_edges() {
    for (auto &e : edges)
        e.draw(vertices[e.i1], vertices[e.i2]);
}

void state::vector::draw_faces() {
    for (auto &f : faces)
        f.draw(vertices[f.i0], vertices[f.i1], vertices[f.i2]);
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

