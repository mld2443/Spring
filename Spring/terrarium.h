//
//  terrarium.h
//  Spring
//
//  Created by Matthew Dillard on 11/2/15.
//  
//

#ifndef terrarium_h
#define terrarium_h

#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>

#include "state.h"
#include "v3.h"

class terrarium {
private:
    double radius;
    double ldepth, lwidth, rdepth, rwidth;
    state::vector state;
    
    v3<double> wind;
    double windc;
    
    void draw_lights();
    void draw_ground();
    
public:
    terrarium(const double r = 5000);
    
    void draw();
};

#endif /* terrarium_h */
