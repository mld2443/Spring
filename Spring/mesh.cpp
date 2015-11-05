//
//  mesh.cpp
//  Spring
//
//  Created by Matthew Dillard on 11/4/15.
//  
//

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include "mesh.h"

void mesh::print(const double x, const double y, const double z, void* font, const std::string s) {
    glRasterPos3d(x, y, z);
    int len, i;
    len = (int)s.length();
    for (i = 0; i < len; i++)
        glutBitmapCharacter(font, s[i]);
}
