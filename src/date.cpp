// This looks like c, but it's really -*-c++-*-
// 
// Copyright (c) 2000 Doodle
// Jerome GOUT - Thu Mar 30 2000
// 

#include "date.h"

extern std::string itoa(int i); //- defined in read_instruction.cc

date::date() : Day(0), Month(0), Year(0)
{}

date::date(int d, int m, int y) : Day(d), Month(m), Year(y)
{}

date::date(const date& Ref) : Day(Ref.Day), Month(Ref.Month), Year(Ref.Year)
{}

date::~date()
{}

date& date::operator=(const date& Ref)
{
  if (this != &Ref) {
    Day = Ref.Day;
    Month = Ref.Month;
    Year = Ref.Year;
  }
  return *this;
}

bool operator == (const date& d1, const date& d2)
{
  return (d1.Day == d2.Day) && (d1.Month == d2.Month) && (d1.Year == d2.Year);
}

std::string date::format() const
{
  if(Month && Day){
    return itoa(Month) + '/' + itoa(Day) + '/' + itoa(Year);
  } else return itoa(Year);
}

//------------------------------------------------------------

#ifdef DEBUG
#include <iostream>

void date::gdb_info(const std::string & s) const
{ //- Simple version Ex. 29/1/2000
  std::cout << s << Day << "/" << Month << "/" << Year << std::endl;
}
#endif /* ! DEBUG */
