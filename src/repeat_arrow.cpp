/*  -*-c++-*- tells to emacs that is a c++ file
 * 
 *  File     : repeat_arrow.cpp
 *  Author   : Jerome Gout
 *  Creation : Fri Sep 28 2001
 *  Revision : $Id$
 * 
 *  Copyright (C) 200, 2001 Doodle
 */

#include "repeat_arrow.h"
#include "vertex.h"
#include "step.h"

void write_ps_vertex_coordinates(std::ostream&, const vertex&);

repeat_arrow::repeat_arrow():
  arrow_symbol(), _point(), _angle(0), _distance(0),
  _number(0), _first_label(), _second_label()
{}

repeat_arrow::repeat_arrow(const std::string& p, int a, int d, unsigned int n, const std::string& lab1, const std::string& lab2):
  arrow_symbol(), _point(p), _angle(a), _distance(d),
  _number(n), _first_label(lab1), _second_label(lab2)
{}
  
repeat_arrow::repeat_arrow(const repeat_arrow& _ref):
  arrow_symbol(_ref), _point(_ref._point), _angle(_ref._angle), _distance(_ref._distance),
  _number(_ref._number), _first_label(_ref._first_label), _second_label(_ref._second_label)
{}

repeat_arrow::~repeat_arrow()
{}

void repeat_arrow::write_ps_draw(std::ostream& out, step& s, double currentRotate, double currentScale) const
{
  vertex v = s.get_vertex(_point);
  write_ps_vertex_coordinates(out, v);
  out << _number << " (";
  if(!_first_label.empty() && !_second_label.empty()) {
    out << _first_label << " - " << _second_label;
  }
  out << ") " << _angle << " " << _distance  << " " << currentRotate << " "
      << currentScale/100.0 << " repeat_arrow" << std::endl;
}

//------------------------------------------------------------

#ifdef DEBUG
#include <iostream>

void repeat_arrow::debug_info(const std::string & s) const
{
  std::cout << s << "_point        :" << _point << std::endl;
  std::cout << s << "_angle        :" << _angle << std::endl;
  std::cout << s << "_distance     :" << _distance << std::endl;
  std::cout << s << "_number       :" << _number << std::endl;
  std::cout << s << "_first_label  :" << _first_label << std::endl;
  std::cout << s << "_second_label :" << _second_label << std::endl;
}
#endif /* ! DEBUG */
