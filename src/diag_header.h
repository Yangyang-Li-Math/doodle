/*  This looks like c, but it's really -*-c++-*-
 * 
 *  Copyright (C) 2000 Doodle
 *  Jerome GOUT - Thu Mar 30 2000
 */


#ifndef DIAG_HEADER_H
#define DIAG_HEADER_H

#include <string>
#include "date.h"
#include "color.h"

class diag_header {
public:
  diag_header();
  diag_header(const diag_header&);
  diag_header& operator=(const diag_header&);
  virtual ~diag_header();

  void set_design_date(const date& d){Design_Date = d;}
  date get_design_date() const {return Design_Date;} 
  void set_diagram_date(const date& d){Diagram_Date = d;}
  date get_diagram_date() const {return Diagram_Date;} 
  void set_designer(const std::string& d){Designer = d;}
  std::string get_designer() const {return Designer;} 
  void set_diagrammer(const std::string& d){Diagrammer = d;}
  std::string get_diagrammer() const {return Diagrammer;} 
  void set_title(const std::string& t){Title = t;}
  std::string get_title() const {return Title;} 
  int get_top_margin() const {return Top_Margin;}
  void set_top_margin(int t) {Top_Margin = t;}
  int get_bottom_margin() const {return Bottom_Margin;}
  void set_bottom_margin(int b) {Bottom_Margin = b;}
  int get_left_margin() const {return Left_Margin;}
  void set_left_margin(int l) {Left_Margin = l;}
  int get_right_margin() const {return Right_Margin;}
  void set_right_margin(int r) {Right_Margin = r;}
  int get_vspace() const {return V_Space;}
  void set_vspace(int v) {V_Space = v;}
  int get_hspace() const {return H_Space;}
  void set_hspace(int h) {H_Space = h;}
  void set_front_color(const std::string& c){Front_Color = color(c);}
  void set_front_color(int r, int g, int b) {Front_Color = color(r,g,b);}
  const color& get_front_color() const {return Front_Color;}
  void get_front_rgb_values(int& r, int& g, int& b) const; 
  void set_back_color(const std::string& c){Back_Color = color(c);}
  void set_back_color(int r, int g, int b){Back_Color = color(r,g,b);}
  const color& get_back_color() const {return Back_Color;}
  void get_back_rgb_values(int& r, int& g, int& b) const; 
  std::string get_comment1() const {return Comment1;}
  void set_comment1(const std::string& c) {Comment1 = c;}
  std::string get_comment2() const {return Comment2;}
  void set_comment2(const std::string& c) {Comment2 = c;}
  std::string get_comment3() const {return Comment3;}
  void set_comment3(const std::string& c) {Comment3 = c;}
  
private:
  date   Design_Date;
  date   Diagram_Date;
  std::string Designer;
  std::string Diagrammer;
  std::string Title;
  int    Top_Margin;
  int    Bottom_Margin;
  int    Left_Margin;
  int    Right_Margin;
  int    V_Space;
  int    H_Space;
  color  Front_Color;
  color  Back_Color;
  std::string Comment1;
  std::string Comment2;
  std::string Comment3;
  
#ifdef DEBUG
  public:
    void gdb_info(const std::string& s = std::string()) const ;
#endif /* ! DEBUG */
    
};

#endif  /* ! DIAG_HEADER_H */


