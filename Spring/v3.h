//
//  v3.h
//  Birds
//
//  Created by Matthew Dillard on 10/15/15.
//  self explainatory
//

#ifndef v3_h
#define v3_h

#include <math.h>

#define PI 3.141592653589793

template <class type>
class v3 {
public:
    type x,y,z;
    
    v3(const type xx = 0.0, const type yy = 0.0, const type zz = 0.0) : x(xx), y(yy), z(zz) {}
    
    v3<type>& operator =(const v3<type>& a) {
        x = a.x;
        y = a.y;
        z = a.z;
        return *this;
    }
    
    v3<type> operator +=(const v3<type>& a) {
        x += a.x;
        y += a.y;
        z += a.z;
        return *this;
    }
    
    v3<type> operator -=(const v3<type>& a) {
        x -= a.x;
        y -= a.y;
        z -= a.z;
        return *this;
    }
    
    v3<type> operator +(const v3<type>& a) const {
        return {x+a.x, y+a.y, z+a.z};
    }
    
    v3<type> operator -(const v3<type>& a) const {
        return {x-a.x, y-a.y, z-a.z};
    }
    
    v3<type> operator *(const v3<type>& a) const {
        return {x*a.x, y*a.y, z*a.z};
    }
    
    v3<type> operator *(const type& a) const {
        return {x*a, y*a, z*a};
    }
    
    v3<type> operator /(const type& a) const {
        return {x/a, y/a, z/a};
    }
    
    double abs() const {
        return sqrt(x*x + y*y + z*z);
    }
    
    v3<type> normalize() const {
        double len = abs();
        return v3<type>(x/len, y/len, z/len);
    }
    
    double dot(const v3<type>& a) const {
        return x*a.x + y*a.y + z*a.z;
    }
    
    double angle(const v3<type>& v_2) const {
        double a = abs();
        double b = v_2.abs();
        return (180.0/PI) * acos(dot(v_2)/(a * b));
    }
    
    v3<type> cross(const v3<type>& a) const {
        return {y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x};
    }
    
    bool operator ==(const v3<type>& v) const {
        return (x == v.x && y == v.y && z == v.z);
    }
    
    v3<type> rotateX(const float theta) const {
        double costype = cos(theta * PI/180);
        double sintype = sin(theta * PI/180);
        return v3<type>(x, y * costype - z * sintype, y * sintype + z * costype);
    }
    
    v3<type> rotateY(const float theta) const {
        double costype = cos(theta * PI/180);
        double sintype = sin(theta * PI/180);
        return v3<type>(x * costype + z * sintype, y, z * costype - x * sintype);
    }
    
    v3<type> rotateZ(const float theta) const {
        double costype = cos(theta * PI/180);
        double sintype = sin(theta * PI/180);
        return v3<type>(x * costype - y * sintype, x * sintype + y * costype, z);
    }
};

#endif /* v3_h */
