/*  This looks like c, but it's really -*-c++-*-
 * 
 *  Copyright (C) 2000 Doodle
 *  Jerome GOUT - Wed Apr 26 2000
 */


#ifndef FACE_H
#define FACE_H

#include <string>
#include <vector>
#include "global_def.h"
#include "color.h"

class face {
public:
  face();
  face(const color& c);
  face(const face&);
  face& operator=(const face&);
  virtual ~face();

  int  get_symbols_number() const {return Symbols.size();}
  std::string get_symbol(int i) const {return Symbols[i];}
  std::vector<std::string> get_symbols() const {return Symbols;}
  void add_symbol(const std::string& s) {Symbols.push_back(s);}
  
  
  //- Accessors
  void set_color(const color& c) {Color = c;}
  const color& get_color() const {return Color;}
  color& ref_color() {return Color;}
  
private:
  std::vector<std::string> Symbols;
  color                    Color;
  
#ifdef DEBUG
  public:
    void gdb_info(const std::string& = std::string()) const;
#endif /* ! DEBUG */
};

#endif  /* ! FACE_H */


