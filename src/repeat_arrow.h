/*  -*-c++-*- tells to emacs that is a c++ file
 * 
 *  File     : repeat_arrow.h
 *  Author   : Jerome Gout
 *  Creation : Thu Sep 27 2001
 *  Revision : $Id$
 *
 *  Copyright (C) 2000, 2001 Doodle
 */


#ifndef REPEAT_ARROW_H
#define REPEAT_ARROW_H

#include <string>

#include "arrow_symbol.h"

/** \file repeat_arrow.h */
/** Class "repeat_arrow"
    A simple arrow with little perpendicular sticks to
    show how many time one have to repeat. 
*/    
class repeat_arrow : public arrow_symbol {
public:
  /// Default constructor
  repeat_arrow();
  /// Parametrized constructor
  repeat_arrow(const std::string&, int = 0, int = 0, unsigned int = 0, const std::string& = std::string(), const std::string& = std::string());
  /// Copy constructor
  repeat_arrow(const repeat_arrow&);
  /// Deep copy (virtual constructor)
  virtual arrow_symbol * clone() const {return new repeat_arrow(*this);}
  /// Destructor
  virtual ~repeat_arrow();

  //- angle is expressed in degrees
  //- scale is expressed in %
  virtual void write_ps_draw(std::ostream& out, step&, double angle, double scale) const;
 
private:
  /// Vertex name pointed by the arrow.
  std::string  _point;
  /// Orientation angle (in degrees).
  int          _angle;
  /// Distance from the pointed point (in mm)
  int          _distance;
  /// Nb times to repeat
  unsigned int _number;
  /// Label of the first repeat step
  std::string  _first_label;
  /// Label of the second repeat step
  std::string  _second_label;
  
#ifdef DEBUG
public:
  /** Debug dump method.
      This method dump each member @c on std::cout.
      Each member is prefixed by the string parameter.
      This method can only be called if the @c DEBUG macro has been definied */
  virtual void debug_info(const std::string& = std::string()) const;
#endif /* ! DEBUG */
};

#endif  /* ! REPEAT_ARROW_H */


