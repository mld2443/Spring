//
//  generator.h
//  Spring
//
//  Created by Matthew Dillard on 11/2/15.
//  
//

#ifndef generator_h
#define generator_h

#define SQRT2 1.41421356237
#define SQRT3 1.73205080757
#define GOLDRATIO 1.6180339887

namespace gen {
    void test(state::vector& S) {
        const unsigned long start = S.size();
        
        S.add_vertex({{10,10,10}, {0,0,0}, 1.0, true});
        S.add_vertex({{10,1,10}, {0,0,0}, 1.0, false});
        
        state::edges.push_back({0.5, 0.0, 5.0, start+0, start+1});
    }
    
    unsigned long index(const unsigned int x, const unsigned int y, const unsigned int rows) { return x * (rows+1) + y; }
    
    void add_edges(state::vector& S, const unsigned int x, const unsigned int y, const unsigned long start, const unsigned int rows, const double dist) {
        if (y > 0) {
            state::edges.push_back({0.5, 0.5, dist, start + index(x, y, rows), start + index(x, y-1, rows)});
            if (x > 0) {
                state::edges.push_back({0.5, 0.5, dist, start + index(x, y, rows), start + index(x-1, y, rows)});
                state::faces.push_back({start+index(x-1, y-1, rows),start+index(x-1, y, rows),start+index(x, y, rows), {0.2, 0.2, 0.3}});
                state::faces.push_back({start+index(x-1, y-1, rows),start+index(x, y, rows),start+index(x, y-1, rows), {0.2, 0.3, 0.3}});
            }
        }
        if (x > 0) {
            if (y > 0)
                state::edges.push_back({0.2, 0.2, SQRT2 * dist, start + index(x, y, rows), start + index(x-1, y-1, rows)});
            if (y < rows)
                state::edges.push_back({0.2, 0.2, SQRT2 * dist, start + index(x, y, rows), start + index(x-1, y+1, rows)});
        }
    }
    
    void flag(state::vector& S, const v3<double>& x1, const v3<double>& x2, const unsigned int cols, const unsigned int rows, const bool side = false) {
        const unsigned long start = S.size();
        const v3<double> step = (x2-x1)/(double)cols;
        double dist = step.abs();
        v3<double> down;
        if (side)
            down = step.cross({0,-1,0}).normalize() * dist;
        else
            down = step.cross({0,-1,0}).cross(step).normalize() * dist;
        
        v3<double> top = x1, lower;
        for (int i = 0; i <= cols; ++i){
            S.add_vertex({top,{0,0,0},1.0,true});
            add_edges(S, i, 0, start, rows, dist);
            
            lower = top + down;
            for (int j = 1; j <= rows; ++j) {
                S.add_vertex({lower,{0,0,0},0.1,false});
                add_edges(S, i, j, start, rows, dist);
                lower += down;
            }
            top += step;
        }
    }
    
    void particle(state::vector& S, const v3<double>& x, const v3<double>& v, const double mass) {
        S.add_vertex({x,v,mass,false});
    }
    
    void rddh(state::vector& S, const v3<double>& c, const v3<double>& v, double r2, const double mass, const double expansion) {
        const unsigned long start = S.size();
        const double r1 = r2/2;
        
        {
            S.add_vertex({{c.x,c.y+r2,c.z}, v, mass, false}); //0
            S.add_vertex({{c.x+r1,c.y+r1,c.z-r1}, v, mass, false}); //1
            S.add_vertex({{c.x+r1,c.y+r1,c.z+r1}, v, mass, false}); //2
            S.add_vertex({{c.x-r1,c.y+r1,c.z+r1}, v, mass, false}); //3
            S.add_vertex({{c.x-r1,c.y+r1,c.z-r1}, v, mass, false}); //4
            S.add_vertex({{c.x,c.y,c.z-r2}, v, mass, false}); //5
            S.add_vertex({{c.x+r2,c.y,c.z}, v, mass, false}); //6
            S.add_vertex({{c.x,c.y,c.z+r2}, v, mass, false}); //7
            S.add_vertex({{c.x-r2,c.y,c.z}, v, mass, false}); //8
            S.add_vertex({{c.x-r1,c.y-r1,c.z-r1}, v, mass, false}); //9
            S.add_vertex({{c.x+r1,c.y-r1,c.z-r1}, v, mass, false}); //10
            S.add_vertex({{c.x+r1,c.y-r1,c.z+r1}, v, mass, false}); //11
            S.add_vertex({{c.x-r1,c.y-r1,c.z+r1}, v, mass, false}); //12
            S.add_vertex({{c.x,c.y-r2,c.z}, v, mass, false}); //13
        }
        
        {
            const double edge_k = 0.3;
            const double edge_d = 0.2;
            const double edge_l = expansion * r1 * SQRT3;
            
            state::edges.push_back({edge_k, edge_d, edge_l, start+0, start+1});
            state::edges.push_back({edge_k, edge_d, edge_l, start+0, start+2});
            state::edges.push_back({edge_k, edge_d, edge_l, start+0, start+3});
            state::edges.push_back({edge_k, edge_d, edge_l, start+0, start+4});
            
            state::edges.push_back({edge_k, edge_d, edge_l, start+1, start+5});
            state::edges.push_back({edge_k, edge_d, edge_l, start+1, start+6});
            state::edges.push_back({edge_k, edge_d, edge_l, start+2, start+6});
            state::edges.push_back({edge_k, edge_d, edge_l, start+2, start+7});
            state::edges.push_back({edge_k, edge_d, edge_l, start+3, start+7});
            state::edges.push_back({edge_k, edge_d, edge_l, start+3, start+8});
            state::edges.push_back({edge_k, edge_d, edge_l, start+4, start+8});
            state::edges.push_back({edge_k, edge_d, edge_l, start+4, start+5});
            
            state::edges.push_back({edge_k, edge_d, edge_l, start+5, start+9});
            state::edges.push_back({edge_k, edge_d, edge_l, start+5, start+10});
            state::edges.push_back({edge_k, edge_d, edge_l, start+6, start+10});
            state::edges.push_back({edge_k, edge_d, edge_l, start+6, start+11});
            state::edges.push_back({edge_k, edge_d, edge_l, start+7, start+11});
            state::edges.push_back({edge_k, edge_d, edge_l, start+7, start+12});
            state::edges.push_back({edge_k, edge_d, edge_l, start+8, start+12});
            state::edges.push_back({edge_k, edge_d, edge_l, start+8, start+9});
            
            state::edges.push_back({edge_k, edge_d, edge_l, start+9, start+13});
            state::edges.push_back({edge_k, edge_d, edge_l, start+10, start+13});
            state::edges.push_back({edge_k, edge_d, edge_l, start+11, start+13});
            state::edges.push_back({edge_k, edge_d, edge_l, start+12, start+13});
        }
        
        {
            const double support_k = 0.7;
            const double support_d = 0.3;
            const double oct_l = expansion * r2 * SQRT2;
            const double cube_l = expansion * r2;
            
            state::edges.push_back({support_k, support_d, oct_l, start+0, start+5});
            state::edges.push_back({support_k, support_d, oct_l, start+0, start+6});
            state::edges.push_back({support_k, support_d, oct_l, start+0, start+7});
            state::edges.push_back({support_k, support_d, oct_l, start+0, start+8});
            state::edges.push_back({support_k, support_d, oct_l, start+5, start+6});
            state::edges.push_back({support_k, support_d, oct_l, start+6, start+7});
            state::edges.push_back({support_k, support_d, oct_l, start+7, start+8});
            state::edges.push_back({support_k, support_d, oct_l, start+8, start+5});
            state::edges.push_back({support_k, support_d, oct_l, start+5, start+13});
            state::edges.push_back({support_k, support_d, oct_l, start+6, start+13});
            state::edges.push_back({support_k, support_d, oct_l, start+7, start+13});
            state::edges.push_back({support_k, support_d, oct_l, start+8, start+13});
            
            state::edges.push_back({support_k, support_d, cube_l, start+1, start+2});
            state::edges.push_back({support_k, support_d, cube_l, start+2, start+3});
            state::edges.push_back({support_k, support_d, cube_l, start+3, start+4});
            state::edges.push_back({support_k, support_d, cube_l, start+4, start+1});
            state::edges.push_back({support_k, support_d, cube_l, start+1, start+10});
            state::edges.push_back({support_k, support_d, cube_l, start+2, start+11});
            state::edges.push_back({support_k, support_d, cube_l, start+3, start+12});
            state::edges.push_back({support_k, support_d, cube_l, start+4, start+9});
            state::edges.push_back({support_k, support_d, cube_l, start+9, start+10});
            state::edges.push_back({support_k, support_d, cube_l, start+10, start+11});
            state::edges.push_back({support_k, support_d, cube_l, start+11, start+12});
            state::edges.push_back({support_k, support_d, cube_l, start+12, start+9});
        }
        
        {
            const GLfloat r = 0.2, g = 0.4, b = 0.4;
            
            state::faces.push_back({start+0,start+6,start+1, r,g,b});
            state::faces.push_back({start+0,start+2,start+6, r,g,b});
            
            state::faces.push_back({start+0,start+7,start+2, r,g,b});
            state::faces.push_back({start+0,start+3,start+7, r,g,b});
            
            state::faces.push_back({start+0,start+8,start+3, r,g,b});
            state::faces.push_back({start+0,start+4,start+8, r,g,b});
            
            state::faces.push_back({start+0,start+5,start+4, r,g,b});
            state::faces.push_back({start+0,start+1,start+5, r,g,b});
            
            state::faces.push_back({start+1,start+6,start+5, r,g,b});
            state::faces.push_back({start+5,start+6,start+10, r,g,b});
            
            state::faces.push_back({start+2,start+7,start+6, r,g,b});
            state::faces.push_back({start+6,start+7,start+11, r,g,b});
            
            state::faces.push_back({start+3,start+8,start+7, r,g,b});
            state::faces.push_back({start+7,start+8,start+12, r,g,b});
            
            state::faces.push_back({start+4,start+5,start+8, r,g,b});
            state::faces.push_back({start+8,start+5,start+9, r,g,b});
            
            state::faces.push_back({start+13,start+8,start+9, r,g,b});
            state::faces.push_back({start+13,start+12,start+8, r,g,b});
            
            state::faces.push_back({start+13,start+5,start+10, r,g,b});
            state::faces.push_back({start+13,start+9,start+5, r,g,b});
            
            state::faces.push_back({start+13,start+6,start+11, r,g,b});
            state::faces.push_back({start+13,start+10,start+6, r,g,b});
            
            state::faces.push_back({start+13,start+7,start+12, r,g,b});
            state::faces.push_back({start+13,start+11,start+7, r,g,b});
        }
    }
    
    void icosahedron(state::vector& S, const v3<double>& c, const v3<double>& v, double r2, const double mass, const double expansion, const bool fixed = false) {
        const unsigned long start = S.size();
        const double r1 = r2 / GOLDRATIO;
        
        {
            S.add_vertex({{c.x,c.y+r1,c.z+r2}, v, mass, fixed}); //0
            S.add_vertex({{c.x,c.y-r1,c.z+r2}, v, mass, fixed}); //1
            S.add_vertex({{c.x-r2,c.y,c.z+r1}, v, mass, fixed}); //2
            S.add_vertex({{c.x-r1,c.y+r2,c.z}, v, mass, fixed}); //3
            S.add_vertex({{c.x+r1,c.y+r2,c.z}, v, mass, fixed}); //4
            S.add_vertex({{c.x+r2,c.y,c.z+r1}, v, mass, fixed}); //5
            S.add_vertex({{c.x+r1,c.y-r2,c.z}, v, mass, fixed}); //6
            S.add_vertex({{c.x-r1,c.y-r2,c.z}, v, mass, fixed}); //7
            S.add_vertex({{c.x-r2,c.y,c.z-r1}, v, mass, fixed}); //8
            S.add_vertex({{c.x,c.y+r1,c.z-r2}, v, mass, fixed}); //9
            S.add_vertex({{c.x+r2,c.y,c.z-r1}, v, mass, fixed}); //10
            S.add_vertex({{c.x,c.y-r1,c.z-r2}, v, mass, fixed}); //11
        }
        
        {
            const double edge_k = 0.6;
            const double edge_d = 0.4;
            const double edge_l = expansion * r1 * 2;
            
            state::edges.push_back({edge_k, edge_d, edge_l, start+0, start+1});
            state::edges.push_back({edge_k, edge_d, edge_l, start+0, start+2});
            state::edges.push_back({edge_k, edge_d, edge_l, start+0, start+3});
            state::edges.push_back({edge_k, edge_d, edge_l, start+0, start+4});
            state::edges.push_back({edge_k, edge_d, edge_l, start+0, start+5});
            
            state::edges.push_back({edge_k, edge_d, edge_l, start+1, start+2});
            state::edges.push_back({edge_k, edge_d, edge_l, start+2, start+3});
            state::edges.push_back({edge_k, edge_d, edge_l, start+3, start+4});
            state::edges.push_back({edge_k, edge_d, edge_l, start+4, start+5});
            state::edges.push_back({edge_k, edge_d, edge_l, start+5, start+1});
            
            state::edges.push_back({edge_k, edge_d, edge_l, start+1, start+6});
            state::edges.push_back({edge_k, edge_d, edge_l, start+1, start+7});
            state::edges.push_back({edge_k, edge_d, edge_l, start+2, start+7});
            state::edges.push_back({edge_k, edge_d, edge_l, start+2, start+8});
            state::edges.push_back({edge_k, edge_d, edge_l, start+3, start+8});
            state::edges.push_back({edge_k, edge_d, edge_l, start+3, start+9});
            state::edges.push_back({edge_k, edge_d, edge_l, start+4, start+9});
            state::edges.push_back({edge_k, edge_d, edge_l, start+4, start+10});
            state::edges.push_back({edge_k, edge_d, edge_l, start+5, start+10});
            state::edges.push_back({edge_k, edge_d, edge_l, start+5, start+6});
            
            state::edges.push_back({edge_k, edge_d, edge_l, start+6, start+7});
            state::edges.push_back({edge_k, edge_d, edge_l, start+7, start+8});
            state::edges.push_back({edge_k, edge_d, edge_l, start+8, start+9});
            state::edges.push_back({edge_k, edge_d, edge_l, start+9, start+10});
            state::edges.push_back({edge_k, edge_d, edge_l, start+10, start+6});
            
            state::edges.push_back({edge_k, edge_d, edge_l, start+6, start+11});
            state::edges.push_back({edge_k, edge_d, edge_l, start+7, start+11});
            state::edges.push_back({edge_k, edge_d, edge_l, start+8, start+11});
            state::edges.push_back({edge_k, edge_d, edge_l, start+9, start+11});
            state::edges.push_back({edge_k, edge_d, edge_l, start+10, start+11});
        }
        
        {
            const GLfloat r = 0.3, g = 0.3, b = 0.2;
            
            state::faces.push_back({start+0,start+2,start+1, r,g,b});
            state::faces.push_back({start+0,start+3,start+2, r,g,b});
            state::faces.push_back({start+0,start+4,start+3, r,g,b});
            state::faces.push_back({start+0,start+5,start+4, r,g,b});
            state::faces.push_back({start+0,start+1,start+5, r,g,b});
            
            state::faces.push_back({start+1,start+2,start+7, r,g,b});
            state::faces.push_back({start+2,start+3,start+8, r,g,b});
            state::faces.push_back({start+3,start+4,start+9, r,g,b});
            state::faces.push_back({start+4,start+5,start+10, r,g,b});
            state::faces.push_back({start+5,start+1,start+6, r,g,b});
            
            state::faces.push_back({start+1,start+7,start+6, r,g,b});
            state::faces.push_back({start+2,start+8,start+7, r,g,b});
            state::faces.push_back({start+3,start+9,start+8, r,g,b});
            state::faces.push_back({start+4,start+10,start+9, r,g,b});
            state::faces.push_back({start+5,start+6,start+10, r,g,b});
            
            state::faces.push_back({start+6,start+7,start+11, r,g,b});
            state::faces.push_back({start+7,start+8,start+11, r,g,b});
            state::faces.push_back({start+8,start+9,start+11, r,g,b});
            state::faces.push_back({start+9,start+10,start+11, r,g,b});
            state::faces.push_back({start+10,start+6,start+11, r,g,b});
        }
    }
};

#endif /* generator_h */
