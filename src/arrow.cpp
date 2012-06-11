// This looks like c, but it's really -*-c++-*-
// 
// Copyright (c) 2000 Doodle
// Jerome GOUT - Wed Apr 19 2000
// 


#include "arrow.h"


arrow::arrow() :
  V1(), V2(), V1_Type(arrowNone), V2_Type(arrowNone),
  Side(arrowLeft), Simple(true), Shape(SIMPLE_ARROW_ANGLE)
{}

arrow::arrow(const arrow& Ref) :
  V1(Ref.V1), V2(Ref.V2), V1_Type(Ref.V1_Type), V2_Type(Ref.V2_Type),
  Side(Ref.Side), Simple(Ref.Simple), Shape(Ref.Shape)
{}

arrow::arrow(const std::string& v1, const std::string& v2,arrow_type v1t,
	     arrow_type v2t, arrow_side s, bool smpl, int sh) :
  V1(v1), V2(v2), V1_Type(v1t), V2_Type(v2t), Side(s), Simple(smpl)
{
  Shape = (sh == SIMPLE_ARROW_ANGLE && !Simple) ? RETURN_ARROW_RATIO : sh;
}

arrow::~arrow()
{}

arrow& arrow::operator=(const arrow& Ref)
{
  if (this != &Ref) {
    V1 = Ref.V1;
    V2 = Ref.V2;
    V1_Type = Ref.V1_Type;
    V2_Type = Ref.V2_Type;
    Side = Ref.Side;
    Simple = Ref.Simple;
    Shape = Ref.Shape;
  }
  return *this;
}

//------------------------------------------------------------

#ifdef DEBUG
#include <iostream>

void arrow::gdb_info(const std::string& s) const
{
  std::cout << s << "V1      = " << V1 << std::endl;
  std::cout << s << "V2      = " << V2 << std::endl;
  std::cout << s << "V1_Type = " << (int)V1_Type << std::endl;
  std::cout << s << "V2_Type = " << (int)V2_Type << std::endl;
  std::cout << s << "Side    = " << (int)Side << std::endl;
  if (Simple) std::cout << s << "Type    =  simple" << std::endl;
  else std::cout << s << "Type    =  return" << std::endl;
  std::cout << s << "Shape   = " << Shape << std::endl;
}
#endif /* ! DEBUG */
