/*  This looks like c, but it's really -*-c++-*-
 * 
 *  Copyright (C) 2000 Doodle
 *  Jerome GOUT - Thu Jul  6 2000
 */


#ifndef COLOR_H
#define COLOR_H

#include <string>

class color {
public:
  color(int=0, int=0, int=0);
  color(const std::string&);
  color(const color&, double=1.0);
  color& operator=(const color&);
  virtual ~color();
  
  int check_color(const std::string & c);
  
  void set_red(int r) {Red = r;}
  int get_red() const {return Red;}
  void set_green(int g) {Green = g;}
  int get_green() const {return Green;}
  void set_blue(int b) {Blue = b;}
  int get_blue() const {return Blue;}

  void get_rgb_values(int& r, int& g, int& b) const {r=Red; g=Green; b=Blue;}
  
  std::string to_string() const;
  
private:
  int Red;
  int Green;
  int Blue;

#ifdef DEBUG
  public:
    void gdb_info(const std::string& = std::string()) const;
#endif /* ! DEBUG */
};

#endif  /* ! COLOR_H */


