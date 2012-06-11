/*  This looks like c, but it's really -*-c++-*-
 * 
 *  Copyright (C) 2000 Doodle
 *  Jerome GOUT - Tue Mar 21 2000
 */


#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <stdlib.h>
#define TEXT_MARKER_CHAR '\1'

class vertex {
public:
  vertex();
  vertex(const vertex&);
  vertex(const std::string&);
  vertex(double, double, double=0.0, double=0.0);
  vertex(const char*, double, double, double=0.0, double=0.0);
  vertex(const std::string, double, double, double=0.0, double=0.0);
  vertex& operator=(const vertex&);
  bool operator==(const vertex&) const;
  bool equivalent(const vertex& Ref) const;
  virtual ~vertex();


  //- accessors 
  std::string get_name() const;
  void set_name(const char* _name) {name = _name;}
  void set_name(const std::string _name) {name = _name;}

  bool has_text() const;
  void add_text(const std::string s);
  std::string get_text() const;
  void clear_text();
  
  double get_x() const {return x;}
  void   set_x(double _x) {x = _x;}
  
  double get_y() const {return y;}
  void   set_y(double _y) {y = _y;}

  void set_x_y(double _x, double _y) {x = _x; y = _y;} 
  
  double get_dx() const {return dx;}
  void   set_dx(double _dx) {dx = _dx;}

  double get_dy() const {return dy;}
  void   set_dy(double _dy) {dy = _dy;}

  void set_dx_dy(double _dx, double _dy) {dx = _dx; dy = _dy;} 

  bool is_debug() const {return debug;}
  void set_debug(bool b) {debug = b;}
  
  //- Vertex transformations
  vertex middle(const vertex&) const;
  vertex fraction(const vertex&, int, int) const;
  vertex symmetry(const vertex&, const vertex&, double, double) const;
  vertex intersection(const vertex&,const vertex&,const vertex&, double, double) const ;
  vertex bissector(const vertex&, const vertex&, double, double) const ;
  vertex mediator(const vertex&, const vertex&, const vertex&, double, double) const;
  vertex projection(const vertex&, const vertex&, double, double) const;
  vertex vertex_to_line(const vertex&, const vertex&, const vertex&,
			const vertex&, const vertex&, bool, double, double) const;
   
  /** This method computes the visual point according to the shift data */
  vertex apply_shift(double angle, double scale) const;

  /** This method returns the angle between line formed by vertices
      (this and the parameter) and the horizontal.
      Returned value is expressed in degrees */
  double get_angle_from_horizontal(const vertex& v, double angle, double scale) const;

  
  //- Vertex checking
  bool is_parallel(const vertex&,const vertex&,const vertex&) const;
  bool is_orthogonal(const vertex&,const vertex&,const vertex&) const;
  double distance(const vertex&, const vertex&) const;

private:
  vertex basic_symmetry(const vertex&, const vertex&) const;
  vertex basic_intersection(const vertex& A,const vertex& B,const vertex& C) const ;

private:
  std::string  name;
  double       x, y;
  double       dx, dy;
  bool         debug;
  
#ifdef DEBUG
  public:
    void gdb_info(const std::string & = std::string()) const;
#endif /* ! DEBUG */
};

#endif  /* ! VERTEX_H */


