/*  -*-c++-*- tells to emacs that is a c++ file
 * 
 *  File     : arrow_symbol.h
 *  Author   : Jerome Gout
 *  Creation : Tue Sep 25 2001
 *  Revision : $Id$
 *
 *  Copyright (C) 2000, 2001 Doodle
 */


#ifndef ARROW_SYMBOL_H
#define ARROW_SYMBOL_H

#ifdef DEBUG
#include <string>
#endif /* ! DEBUG */

#include <iostream>

class step;

/** \file arrow_symbol.h */
/** Class "arrow_symbol"
    Abstract class wrapping all symbols of arrow (for instance open_arrow ..).
*/    
class arrow_symbol {
public:
  /// Default constructor
  arrow_symbol() {}
  /// Copy constructor
  arrow_symbol(const arrow_symbol&) {}
  /// Destructor
  virtual ~arrow_symbol() {}
  /// Deep copy (virtual constructor)
  virtual arrow_symbol * clone() const = 0;

  //- angle is expressed in degrees
  //- scale is expressed in %
  virtual void write_ps_draw(std::ostream& out, step&, double angle, double scale) const = 0;
  
private:
  
#ifdef DEBUG
public:
  /** Debug dump method.
      This method dump each member @c on std::cout.
      Each member is prefixed by the string parameter.
      This method can only be called if the @c DEBUG macro has been definied */
  virtual void debug_info(const std::string& = std::string()) const {}
#endif /* ! DEBUG */
};

#endif  /* ! ARROW_SYMBOL_H */


