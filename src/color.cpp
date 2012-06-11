// This looks like c, but it's really -*-c++-*-
// 
// Copyright (c) 2000 Doodle
// Jerome GOUT - Thu Jul  6 2000
// 

#include <cstdio>
#include "color.h"

#define MAX_COLORS        11

extern void yyerror(char*); //- defined in parser.y

color White(100,100,100);
static char * color_names[MAX_COLORS] = {"black", "gray10", "gray20", "gray30", "gray40",
					 "gray50", "gray60", "gray70", "gray80", "gray90",
					 "white"};

color::color(int r, int g, int b)
{
  if(r<0 || r>100 || g<0 || g>100 || b<0 || b>100) yyerror("Bad color values [0-100]");
  else {
    Red = r;
    Green = g;
    Blue = b;
  }
}

color::color(const std::string &c)
{
  int color_idx = 0;
  if((color_idx = check_color(c)) == -1) yyerror("Unknown color.");
  else {
    switch (color_idx) {
     case 0 : //- Black
      Red = 0; Green = 0; Blue = 0; break;
     case 1 : //- gray10
      Red = 10; Green = 10; Blue = 10; break;
     case 2 : //- gray20
      Red = 20; Green = 20; Blue = 20; break;
     case 3 : //- gray30
      Red = 30; Green = 30; Blue = 30; break;
     case 4 : //- gray40
      Red = 40; Green = 40; Blue = 40; break;
     case 5 : //- gray50
      Red = 50; Green = 50; Blue = 50; break;
     case 6 : //- gray60
      Red = 60; Green = 60; Blue = 60; break;
     case 7 : //- gray70
      Red = 70; Green = 70; Blue = 70; break;
     case 8 : //- gray80
      Red = 80; Green = 80; Blue = 80; break;
     case 9 : //- gray90
      Red = 90; Green = 90; Blue = 90; break;
     case 10 : //- White
      Red = 100; Green = 100; Blue = 100; break;
    }
  }
}

color::color(const color& Ref, double p)
{
  Red = (int)(Ref.Red*p);
  if(Red > 100) Red = 100;
  Green = (int)(Ref.Green*p);
  if(Green > 100) Green = 100;
  Blue = (int)(Ref.Blue*p);
  if(Blue > 100) Blue = 100;
}

color::~color()
{}

color& color::operator=(const color& Ref)
{
  if (this != &Ref) {
    Red = Ref.Red;
    Green = Ref.Green;
    Blue = Ref.Blue;
  }
  return *this;
}

int color::check_color(const std::string &c)
{
  bool found = false;
  int i = 0;
  while(!found && i < MAX_COLORS) {
    found = c == std::string(color_names[i]);
    i++;
  }
  if(found) return i-1;
  else return -1;
}

std::string color::to_string() const {
  char temp[4]="";
  std::string ret = std::string("(");
  sprintf(temp, "%d", Red);
  ret += std::string(temp) + std::string(", ");
  sprintf(temp, "%d", Green);
  ret += std::string(temp) + std::string(", ");
  sprintf(temp, "%d", Blue);
  ret += std::string(temp) + std::string(")");
  return ret;
}

//------------------------------------------------------------

#ifdef DEBUG
#include <iostream>

void color::gdb_info(const std::string& s) const
{
  std::cout << s << '(' << Red << ", " << Green << ", " << Blue << ')' << std::endl;
}
#endif /* ! DEBUG */
