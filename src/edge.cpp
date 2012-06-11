// This looks like c, but it's really -*-c++-*-
// 
// Copyright (c) 2000 Doodle
// Jerome GOUT - Tue Mar 28 2000
// 


#include "edge.h"

edge::edge(const std::string& _v1, const std::string& _v2, edge_type _t, int _lt, int _l1, int _l2) :
  Name("null"), V1(_v1), V2(_v2), Type(_t), Visible(true), Limits(_lt), Limit_V1(_l1), Limit_V2(_l2), Debug(false)
{}

edge::edge(const edge& Ref) :
  Name(Ref.Name), V1(Ref.V1), V2(Ref.V2), Type(Ref.Type),
  Visible(Ref.Visible), Limits(Ref.Limits), Limit_V1(Ref.Limit_V1),
  Limit_V2(Ref.Limit_V2), Debug(Ref.Debug)
{}

edge::~edge()
{}

edge& edge::operator=(const edge& Ref)
{
  if (this != &Ref) {
    Name = Ref.Name;
    V1 = Ref.V1;
    V2 = Ref.V2;
    Type = Ref.Type;
    Limits = Ref.Limits;
    Limit_V1 = Ref.Limit_V1;
    Limit_V2 = Ref.Limit_V2;
    Debug = Ref.Debug;
  }
  return *this;
}

bool edge::equal(const std::string& s1, const std::string& s2)
{
  return (V1 == s1 && V2 == s2) || (V1 == s2 && V2 == s1);
}

//------------------------------------------------------------

#ifdef DEBUG
#include <iostream>

void edge::gdb_info(const std::string & s) const
{
  std::cout << s << "Name     = " << Name << std::endl;
  std::cout << s << "Type     = " << (int)Type << std::endl;
  std::cout << s << "V1       = " << V1 << std::endl;
  std::cout << s << "V2       = " << V2 << std::endl;
  if(Visible) std::cout << s << "Visible  = true" << std::endl;
  else std::cout << s << "Visible  = false" << std::endl;
  std::cout << s << "Limits   = " << (int)Limits << std::endl;
  if(has_limit_v1()) std::cout << s << "Limit_V1 = ";
  else std::cout << s << "Space_V1 = ";
  std::cout << Limit_V1 << std::endl;
  if(has_limit_v2()) std::cout << s << "Limit_V2 = ";
  else std::cout << s << "Space_V2 = ";
  std::cout << Limit_V2 << std::endl;
  std::cout << s << "Debug    = " << Debug << std::endl;
}
#endif /* ! DEBUG */
