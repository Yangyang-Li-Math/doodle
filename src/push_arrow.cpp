/*  -*-c++-*- tells to emacs that is a c++ file
 * 
 *  File     : push_arrow.cpp
 *  Author   : Jerome Gout
 *  Creation : Wed Sep 26 2001
 *  Revision : $Id$
 * 
 *  Copyright (C) 2000, 2001 Doodle
 */

#include "push_arrow.h"
#include "vertex.h"

void write_ps_vertex_coordinates(std::ostream&, const vertex&);

push_arrow::push_arrow() :
  arrow_symbol(), _point(), _angle(0), _distance(0)
{}

push_arrow::push_arrow(const std::string name, int a, int d) :
  arrow_symbol(), _point(name), _angle(a), _distance(d)
{}

push_arrow::push_arrow(const push_arrow& _ref) :
  arrow_symbol(_ref), _point(_ref._point),
  _angle(_ref._angle), _distance(_ref._distance)
{}

push_arrow::~push_arrow()
{}

void push_arrow::write_ps_draw(std::ostream& out, step& s, double currentRotate, double currentScale) const
{
  vertex v = s.get_vertex(_point);
  write_ps_vertex_coordinates(out, v);
  out << _angle << " " << _distance << " " << currentRotate << " " << currentScale/100.0
      << " push_arrow" << std::endl;
}

//------------------------------------------------------------

#ifdef DEBUG
#include <iostream>

void push_arrow::debug_info(const std::string & s) const
{
  std::cout << s << "_point :" << _point << std::endl;
  std::cout << s << "_angle :" << _angle << std::endl;
  std::cout << s << "_distance :" << _distance << std::endl;
}
#endif /* ! DEBUG */
