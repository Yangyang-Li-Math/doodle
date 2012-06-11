/*  -*-c++-*- tells to emacs that is a c++ file
 * 
 *  File     : open_arrow.h
 *  Author   : Jerome Gout
 *  Creation : Tue Sep 25 2001
 *  Revision : $Id$
 *
 *  Copyright (C) 2000, 2001 Doodle
 */


#ifndef OPEN_ARROW_H
#define OPEN_ARROW_H

#include <string>

#include "arrow_symbol.h"

/** \file open_arrow.h */
/** Class "open_arrow"
    Open arrow symbol (white filled arrow).
*/    
class open_arrow : public arrow_symbol {
public:
  /// Default constructor
  open_arrow();
  /// Parametrized constructor
  open_arrow(const std::string& v1, const std::string& v2, bool right=true);
  /// Copy constructor
  open_arrow(const open_arrow&);
  /// Destructor
  virtual ~open_arrow();
  /// Deep copy (virtual constructor)
  virtual arrow_symbol * clone() const {return new open_arrow(*this);}

  //- angle is expressed in degrees
  //- scale is expressed in %
  virtual void write_ps_draw(std::ostream& out, step&, double angle, double scale) const;
  
private:
  /// First edge extremity.
  std::string  _v1;
  /// Second edge extremity.
  std::string  _v2;
  /// Should the arrow be drawn on the right or left side
  bool         _right;
  
#ifdef DEBUG
public:
  /** Debug dump method.
      This method dump each member @c on std::cout.
      Each member is prefixed by the string parameter.
      This method can only be called if the @c DEBUG macro has been definied */
  virtual void debug_info(const std::string& = std::string()) const;
#endif /* ! DEBUG */
};

#endif  /* ! OPEN_ARROW_H */


