/*  -*-c++-*- tells to emacs that is a c++ file
 * 
 *  File     : push_arrow.h
 *  Author   : Jerome Gout
 *  Creation : Wed Sep 26 2001
 *  Revision : $Id$
 *
 *  Copyright (C) 2000, 2001 Doodle
 */


#ifndef PUSH_ARROW_H
#define PUSH_ARROW_H

#include <string>

#include "vertex.h"
#include "arrow_symbol.h"
#include "step.h"

/** \file push_arrow.h */
/** Class "push_arrow"
    Push arrow symbol (black filled head arrow).
*/    
class push_arrow : public arrow_symbol {
public:
  /// Default constructor
  push_arrow();
  /// Parametrized constructor
  push_arrow(const std::string, int=0, int=0);
  /// Copy constructor
  push_arrow(const push_arrow&);
  /// Deep copy (virtual constructor)
  virtual arrow_symbol * clone() const {return new push_arrow(*this);}
  /// Destructor
  virtual ~push_arrow();

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
  
#ifdef DEBUG
public:
  /** Debug dump method.
      This method dump each member @c on std::cout.
      Each member is prefixed by the string parameter.
      This method can only be called if the @c DEBUG macro has been definied */
  virtual void debug_info(const std::string& = std::string()) const;
#endif /* ! DEBUG */
};

#endif  /* ! PUSH_ARROW_H */


