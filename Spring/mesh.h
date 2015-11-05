//
//  mesh.h
//  Spring
//
//  Created by Matthew Dillard on 11/3/15.
//
//

#ifndef mesh_h
#define mesh_h

#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <iostream>

#include "v3.h"

namespace mesh {
    
    void print(const double x, const double y, const double z, void* font, const std::string s);

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
        
        operator bool() const { return fixed; }
        
        void draw(const GLdouble radius = 0.1, bool drawtext = false) const {
            if (fixed) {
                GLfloat color[] = {0.5, 0.5, 0.5};
                glMaterialfv(GL_FRONT, GL_AMBIENT, color);
            }
            else {
                GLfloat color[] = {0.2, 0.2, 0.2};
                glMaterialfv(GL_FRONT, GL_AMBIENT, color);
            }
            
            glPushMatrix(); {
                glTranslated(pos.x, pos.y, pos.z);
                glutSolidSphere(radius, 10, 5);
            } glPopMatrix();
            
            if (drawtext) {
                GLfloat color[] = { 1.0, 1.0, 1.0 };
                glMaterialfv(GL_FRONT, GL_AMBIENT, color);
                print(pos.x, pos.y, pos.z, GLUT_BITMAP_HELVETICA_10, pos.tostring());
            }
        }
    };
    
    template <class type> struct strut {
        type k, d, l;
        unsigned long i1, i2;
        
        void draw(const vertex<type>& v1, const vertex<type>& v2) {
            GLfloat ratio = (v1.pos - v2.pos).abs() / l;
            
            if (ratio <= 1.0) {
                GLfloat color[] = {1.0, ratio, ratio};
                glMaterialfv(GL_FRONT, GL_AMBIENT, color);
            }
            else {
                GLfloat color[] = {static_cast<GLfloat>(0.5 + (0.5/ratio)), static_cast<GLfloat>(0.5 + (0.5/ratio)), static_cast<GLfloat>(1.0/ratio)};
                glMaterialfv(GL_FRONT, GL_AMBIENT, color);
            }
            
            glBegin(GL_LINE_STRIP); {
                glVertex3d(v1.pos.x, v1.pos.y, v1.pos.z);
                glVertex3d(v2.pos.x, v2.pos.y, v2.pos.z);
            } glEnd();
        }
    };
    
    template <class type> struct face {
    private:
        v3<type> pick_axis_proj(const v3<type>& normal) const {
            if (normal.x > normal.y && normal.x > normal.z)
                return v3<type>(0,1,1);
            else if (normal.y > normal.x && normal.y > normal.z)
                return v3<type>(1,0,1);
            return v3<type>(1,1,0);
        }
        
    public:
        unsigned long i0, i1, i2;
        GLfloat color[3];
        
        bool cross_plane(const v3<type>& c, const v3<type>& normal, const v3<type>& x0, const v3<type>& x1) const {
            type sign1 = (x0 - c).dot(normal);
            type sign2 = (x1 - c).dot(normal);
            if (sign1 * sign2 < 0.0)
                return true;
            return false;
        }
        
        v3<type> find_intersect(const v3<type>& c, const v3<type>& normal, const v3<type>& x1, const v3<type>& x2) const {
            v3<type> l = (x2 - x1).normalize();
            float d = ((x1 - c).dot(normal))/(l.dot(normal));
            return x1 + l * d;
        }
        
        bool projection_intersection(const v3<type>& normal, const v3<type>& c1,const v3<type>& c2,const v3<type>& c3,const v3<type>& x) const {
            v3<type> proj = pick_axis_proj(normal), p_i = v3<type>(1,1,1)-proj;
            v3<type> v1 = (c2 - c1) * proj, x1 = (x - c1) * proj, cp1 = (v1.cross(x1) * p_i).normalize();
            v3<type> v2 = (c3 - c2) * proj, x2 = (x - c2) * proj, cp2 = (v2.cross(x2) * p_i).normalize();
            v3<type> v3 = (c1 - c3) * proj, x3 = (x - c3) * proj, cp3 = (v3.cross(x3) * p_i).normalize();
            
            if (cp1 == cp2 && cp2 == cp3)
                return true;
            return false;
        }
        
        type dist_from_plane(const v3<type>& c, const v3<type>& normal, const v3<type>& x) const {
            return (x - c).dot(normal);
        }
        
        void draw(const vertex<type>& v0, const vertex<type>& v1, const vertex<type>& v2) {
            glMaterialfv(GL_FRONT, GL_AMBIENT, color);
            
            v3<type> normal = ((v0.pos - v1.pos).cross(v2.pos - v1.pos)).normalize();
            
            glBegin(GL_TRIANGLES); {
                glNormal3d(normal.x, normal.y, normal.z);
                glVertex3d(v0.pos.x, v0.pos.y, v0.pos.z);
                glVertex3d(v1.pos.x, v1.pos.y, v1.pos.z);
                glVertex3d(v2.pos.x, v2.pos.y, v2.pos.z);
            } glEnd();
        }
    };
}

#endif /* mesh_h */
