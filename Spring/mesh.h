//
//  mesh.h
//  Spring
//
//  Created by Matthew Dillard on 11/3/15.
//  Copyright © 2015 Matthew Dillard. All rights reserved.
//

#ifndef mesh_h
#define mesh_h

#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <list>

#include "v3.h"

namespace mesh {
    template <class type> struct vertex {
        v3<type> pos, vel;
        type mass;
        bool fixed;
        
        vertex(const v3<type> p = v3<type>(), const v3<type> v = v3<type>(), const type m = 0.0, const bool f = false): pos(p), vel(v), mass(m), fixed(f) {}
        vertex(const vertex& v): pos(v.pos), vel(v.vel), mass(v.mass), fixed(v.fixed) {}
        
        vertex<type>& operator=(const vertex<type>& v) {
            pos = v.pos;
            vel = v.vel;
            mass = v.mass;
            fixed = v.fixed;
            return this;
        }
        
        vertex<type> operator+(const vertex<type>& v) const {
            vertex<type> rvalue;
            rvalue.pos = pos + v.pos;
            rvalue.vel = vel + v.vel;
            rvalue.mass = mass;
            rvalue.fixed = fixed;
            return rvalue;
        }
        
        vertex<type> operator-(const vertex<type>& v) const {
            vertex<type> rvalue;
            rvalue.pos = pos - v.pos;
            rvalue.vel = vel - v.vel;
            rvalue.mass = mass;
            rvalue.fixed = fixed;
            return rvalue;
        }
        
        vertex<type> operator*(const type value) const {
            vertex<type> rvalue;
            rvalue.pos = pos * value;
            rvalue.vel = vel * value;
            rvalue.mass = mass;
            rvalue.fixed = fixed;
            return rvalue;
        }
        
        void draw(const GLdouble radius = 0.25) const {
            glPushMatrix(); {
                glTranslated(pos.x, pos.y, pos.z);
                glutSolidSphere(radius, 10, 5);
            } glPopMatrix();
        }
    };
    
    template <class type> struct face {
        std::list<vertex<type>> vertices;
        std::list<type> angles;
        
        face() {}
    };
}

#endif /* mesh_h */
