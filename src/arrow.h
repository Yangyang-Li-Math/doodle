/*  This looks like c, but it's really -*-c++-*-
 * 
 *  Copyright (C) 2000 Doodle
 *  Jerome GOUT - Wed Apr 19 2000
 */


#ifndef ARROW_H
#define ARROW_H

#include <string>
#include "global_def.h"

enum arrow_type {arrowValley, arrowMountain, arrowUnfold, arrowNone};
enum arrow_side {arrowLeft, arrowRight};

class arrow {
public:
  arrow();
  arrow(const std::string&, const std::string&,	arrow_type=arrowNone,
	arrow_type=arrowNone, arrow_side=arrowLeft, bool=true,
	int = SIMPLE_ARROW_ANGLE);
  arrow(const arrow&);
  arrow& operator=(const arrow&);
  virtual ~arrow();

  void set_v1(const std::string& v1) {V1 = v1;}
  std::string get_v1() const {return V1;}
  void set_v2(const std::string& v2) {V2 = v2;}
  std::string get_v2() const {return V2;}
  void set_v1_type(arrow_type v1_type) {V1_Type=v1_type;}
  arrow_type get_v1_type() const {return V1_Type;}
  void set_v2_type(arrow_type v2_type) {V2_Type=v2_type;}
  arrow_type get_v2_type() const {return V2_Type;}
  void set_side(arrow_side side) {Side=side;}
  arrow_side get_side() const {return Side;}
  bool is_simple() const {return Simple;}
  void set_simple(bool s) {Simple = s;}
  int get_shape() const {return Shape;}
  void set_shape(int s) {Shape = s;}

private:
  std::string     V1;
  std::string     V2;
  arrow_type V1_Type;
  arrow_type V2_Type;
  arrow_side Side;
  bool       Simple;    
  int        Shape;
  
#ifdef DEBUG
  public:
    void gdb_info(const std::string& s=std::string("")) const;
#endif /* ! DEBUG */
};

#endif  /* ! ARROW_H */


