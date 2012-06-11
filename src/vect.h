/* 
 *  Class vect
 *  Copyright (C) 2000 Doodle
 *  Xavier Fouchet - 10 mars 2000
 */


#ifndef VECT_H
#define VECT_H

#include <string>
#include <cmath>
#include "vertex.h"

class vect {
public:
  vect();
  vect(const vect&);
  vect(double, double);
  vect(vertex v1, vertex v2);
  vect(const char*, double, double);
  vect(const char* n, vertex v1, vertex v2);
  vect(const std::string, double, double);
  vect(const std::string n, vertex v1, vertex v2);
  vect& operator=(const vect&);
  bool operator==(const vect&) const;
  double operator*(const vect&);
  vect operator+(const vect&);
  friend vect operator*(double, const vect&);
  friend vect operator/(const vect&, double d);
  friend vertex operator+(const vertex&, const vect&);
  virtual ~vect();


  //- accessors 
  const std::string & get_name() const{return name;}
  void set_name(const char* _name) {name = _name;}
  void set_name(const std::string _name) {name = _name;}

  double get_x()  const {return x;}
  void   set_x(double _x) {x = _x;}
  
  double get_y() const {return y;}
  void   set_y(double _y) {y = _y;}

  void set_x_y(double _x, double _y) {x = _x; y = _y;} 
  
  //- Operations
  double norm(){return sqrt(x*x+y*y);}
  double sqr_norm(){return x*x+y*y;}
  vect ortho();
  
private:
  std::string name;
  double x, y;
  
#ifdef DEBUG
  public:
    void gdb_info(const std::string & = std::string("")) const;
#endif /* ! DEBUG */
};

#endif  /* ! VECT_H */


