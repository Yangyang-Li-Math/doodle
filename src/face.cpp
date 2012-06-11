// This looks like c, but it's really -*-c++-*-
// 
// Copyright (c) 2000 Doodle 
// Jerome GOUT - Wed Apr 26 2000
// 

#ifdef WIN32
#pragma warning (disable : 4786)
#endif // WIN32

#include "face.h"

face::face() : Symbols(), Color()
{}

face::face(const color& c) : Symbols(), Color(c)
{}

face::face(const face& Ref) : Symbols(Ref.Symbols), Color(Ref.Color)
{}

face::~face()
{}

face& face::operator=(const face& Ref)
{
  if (this != &Ref) {
    Symbols = Ref.Symbols;
    Color = Ref.Color;
  }
  return *this;
}

//------------------------------------------------------------

#ifdef DEBUG
#include <iostream>

void face::gdb_info(const std::string& s) const
{
  std::cout << s << "Color used : ";
  Color.gdb_info();
  std::cout << s << "Symbols    : ";
  int ns = Symbols.size();
  for(int i=0; i<ns; i++) std::cout << Symbols[i] << " ";
  std::cout << std::endl;
}
#endif /* ! DEBUG */
