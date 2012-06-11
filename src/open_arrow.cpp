/*  -*-c++-*- tells to emacs that is a c++ file
 * 
 *  File     : open_arrow.cpp
 *  Author   : Jerome Gout
 *  Creation : Tue Sep 25 2001
 *  Revision : $Id$
 * 
 *  Copyright (C) 2000, 2001 Doodle
 */

#include <iostream>
#include <cmath>

#include "open_arrow.h"
#include "vertex.h"
#include "step.h"

extern void write_ps_vertex_coordinates(std::ostream& out, const vertex&);

open_arrow::open_arrow() :
  arrow_symbol(),
  _v1(), _v2(), _right(true)
{}

open_arrow::open_arrow(const std::string& v1, const std::string& v2, bool right):
  arrow_symbol(),
   _v1(v1), _v2(v2), _right(right)
{}

open_arrow::open_arrow(const open_arrow& _ref):
  arrow_symbol(_ref),
   _v1(_ref._v1), _v2(_ref._v2), _right(_ref._right)
{}

open_arrow::~open_arrow()
{} 

void open_arrow::write_ps_draw(std::ostream& out, step& s, double currentRotate, double currentScale) const
{
  vertex v1 = s.get_vertex(_v1);
  vertex v2 = s.get_vertex(_v2);
  vertex mid = v1.middle(v2);
  double angle = v1.get_angle_from_horizontal(v2, currentRotate, currentScale);
  
  if(_right) angle -= 90; //- right perpendicular
  else angle += 90; //- left perpendicular
  
  write_ps_vertex_coordinates(out, mid);
  out << angle << " " << currentScale/100.0 << " open_arrow" << std::endl;
}

//------------------------------------------------------------

#ifdef DEBUG
#include <iostream>

void open_arrow::debug_info(const std::string & s) const
{
  std::cout << s << "_v1 :" << _v1 << std::endl;
  std::cout << s << "_v2 :" << _v2 << std::endl;
  std::cout << s << "_right :" << _right << std::endl;
}
#endif /* ! DEBUG */
