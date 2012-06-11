/*  This looks like c, but it's really -*-c++-*-
 * 
 *  Copyright (C) 2000 Doodle
 *  Jerome GOUT - Thu Mar 30 2000
 */


#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>

class date {
public:
  date();
  date(int d, int m, int y);
  date(const date&);
  date& operator=(const date&);
  friend bool operator == (const date&, const date&);
  virtual ~date();

  int  get_day() const {return Day;}
  void set_day(int d);
  int  get_month() const {return Month;}
  void set_month(int m);
  int  get_year() const {return Year;}
  void set_year(int y){Year = y;}
  std::string format() const;
  
private:
  int Day;
  int Month;
  int Year;
  
#ifdef DEBUG
  public:
    void gdb_info(const std::string & s = std::string()) const ;
#endif /* ! DEBUG */
};

inline void date::set_day(int d)
{
  if(d > 0 && d < 32) Day = d;
  else std::cerr << "error : Wrong day given" << std::endl;
}

inline void date::set_month(int m)
{
  if(m > 0 && m < 13) Month = m;
  else std::cerr << "error : Wrong month given" << std::endl;
}


#endif  /* ! DATE_H */


