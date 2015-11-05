//
//  state.h
//  Spring
//
//  Created by Matthew Dillard on 11/2/15.
//  
//

#ifndef state_h
#define state_h

#include <vector>
#include <list>

#include "mesh.h"

namespace state {
    class vector {
    private:
        std::vector<mesh::vertex<double>> vertices;
        
    public:
        vector() {}
        vector(const vector& s) {
            for (auto &p : s.vertices)
                vertices.push_back(mesh::vertex<double>(p));
        }
        
        typename std::vector<mesh::vertex<double>>::const_iterator begin() const { return vertices.begin(); }
        typename std::vector<mesh::vertex<double>>::const_iterator end() const { return vertices.end(); }
        unsigned long size() { return vertices.size(); }
        
        void add_vertex(const mesh::vertex<double>& v) { vertices.push_back(v); }
        void clear_vertices() { vertices.clear(); }
        
        void calc_intersects(vector& Snew);
        
        void draw_vertices() {
            for (auto &p : vertices)
                p.draw();
        }
        
        void draw_edges();
        
        void draw_faces();
        
        mesh::vertex<double>& operator[](unsigned long i) { return vertices[i]; }
        const mesh::vertex<double>& operator[](unsigned long i) const { return vertices[i]; }
        
        vector operator+(const vector& s) const ;
        
        vector operator-(const vector& s) const ;
        
        vector operator*(const double value) const ;
        
        vector* operator=(const vector& s) ;
    };
    
    extern std::vector<mesh::strut<double>> edges;
    extern std::vector<mesh::face<double>> faces;
    extern v3<double> gravity;
    extern v3<double> wind;
    extern double dragc;
    extern double liftc;
    
    vector derive(const vector& S, const double t);
}

#endif /* state_h */
