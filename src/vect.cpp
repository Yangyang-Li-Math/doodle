// class vect
// 
// Copyright (c) 2000 Doodle
// Xavier Fouchet - 10 avril 2000
// 


#include "vect.h"
#include "global_def.h"

vect::vect() : name("null"), x(0.0), y(0.0)
{}

vect::vect(double _x, double _y):
  name("null"), x(_x), y(_y)
{}

vect::vect(vertex v1, vertex v2):
  name("null"), x(v2.get_x()-v1.get_x()), y(v2.get_y()-v1.get_y())
{}

vect::vect(const char * n, double _x, double _y):
  name(n), x(_x), y(_y)
{}

vect::vect(const char * n, vertex v1, vertex v2):
  name(n), x(v2.get_x()-v1.get_x()), y(v2.get_y()-v1.get_y())
{}

vect::vect(const std::string n, double _x, double _y):
  name(n), x(_x), y(_y)
{}

vect::vect(const std::string n, vertex v1, vertex v2):
  name(n), x(v2.get_x()-v1.get_x()), y(v2.get_y()-v1.get_y())
{}

vect::vect(const vect& Ref): name(Ref.name),
  x(Ref.x), y(Ref.y)
{}

vect::~vect()
{}

vect& vect::operator=(const vect& Ref)
{
  if (this != &Ref) {
    name = Ref.name;
    x = Ref.x;
    y = Ref.y;
  }
  return *this;
}

bool vect::operator==(const vect& Ref) const
{
  if (this != &Ref) {
    double norme=sqrt((Ref.x-x)*(Ref.x-x)+(Ref.y-y)*(Ref.y-y));
    return (norme < EPSILON && norme>-EPSILON); // A~B 
  }
  return true;
}

vect vect::operator+(const vect& u)
{
    return vect(x+u.x,y+u.y);
}

double vect::operator*(const vect& v)
{
    return x*v.x+y*v.y;
}

vect operator/(const vect& v, double d)
{
  return vect(v.x/d,v.y/d);
}

vect operator*(double a, const vect& v)
{
    return vect(a*v.x,a*v.y);
}

vertex operator+(const vertex& A, const vect& v)
{
    return vertex(A.get_name(),A.get_x()+v.x,A.get_y()+v.y,A.get_dx(),A.get_dy());
}

vect vect::ortho()
{
    return vect(-y,x);
}

//------------------------------------------------------------

#ifdef DEBUG
#include <iostream>

void vect::gdb_info(const std::string & s) const
{
  std::cout << s << "name = " << name << std::endl;
  std::cout << s << "x    = " << x << std::endl;
  std::cout << s << "y    = " << y << std::endl;
}
#endif /* ! DEBUG */
