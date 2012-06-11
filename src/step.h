/*  This looks like c, but it's really -*-c++-*-
 * 
 *  Copyright (C) 2000 Doodle
 *  Jerome GOUT - Fri Mar 31 2000
 */


#ifndef STEP_H
#define STEP_H

#include <string>
#include <vector>
#include <list>

#include "global_def.h"
#include "vertex.h"
#include "edge.h"
#include "arrow.h"
#include "face.h"
#include "arrow_symbol.h"

class step {
public:
  typedef std::vector<vertex>::iterator it_vertices;
  typedef std::vector<vertex>::const_iterator cit_vertices;   
  typedef std::list<arrow_symbol*>::iterator it_arrow_symbols;
  typedef std::list<arrow_symbol*>::const_iterator cit_arrow_symbols;   
  typedef std::list<std::string>::const_iterator cit_captions;
  typedef std::list<std::string>::iterator it_captions;
  
  step();
  step(int, int = CLIP_HEIGHT, int = CLIP_WIDTH, int = 100, double = 0.0, double = 0.0,
       bool=false);
  step(const step&);
  virtual ~step();

  //- slots accessors

  cit_vertices get_begin_vertices() const {return Symbols.begin();}
  it_vertices ref_begin_vertices() {return Symbols.begin();}
  cit_vertices get_end_vertices() const {return Symbols.end();}
  it_vertices ref_end_vertices() {return Symbols.end();}
  
  void set_index(const std::string & i) {Index = i;}
  void set_index(int i);
  std::string get_index() const {return Index;}

  void add_caption(const std::string & c) {Captions.push_back(c);}
  int get_captions_number() const {return Captions.size();}
  cit_captions get_captions_begin() const {return Captions.begin();}
  cit_captions get_captions_end() const {return Captions.end();}
  it_captions ref_captions_begin() {return Captions.begin();}
  it_captions ref_captions_end() {return Captions.end();}
  
  std::vector<vertex> get_symbols() const {return Symbols;}
  vertex& ref_last_vertex() {return Symbols[Symbols.size()-1];}
  void set_symbols(const std::vector<vertex>& s){Symbols = s;}
  void add_symbol(const vertex & v) {Symbols.push_back(v);}
  int get_symbols_number() const {return Symbols.size();}
  void delete_symbol(int i);
  std::string get_symbol_name(int i) const {return Symbols[i].get_name();}
  const vertex& get_vertex(const std::string&);
  vertex& ref_vertex(const std::string&);
  const vertex& get_current_vertex() const;
  vertex& ref_current_vertex();
  vertex& ref_vertex(int i) {return Symbols[i];}
  int get_arrows_number() const {return Arrows.size();}
  arrow get_arrow(int i) const {return Arrows[i];}
  std::vector<arrow> get_arrows() const {return Arrows;}
  void set_arrows(const std::vector<arrow>& a) {Arrows=a;}
  void add_arrow(const arrow& a) {Arrows.push_back(a);}
  int get_lines_number() const {return Lines.size();}
  edge get_line(int i) const;
  edge& ref_line(int i);
  std::vector<edge> get_lines() const {return Lines;}
  void set_lines(const std::vector<edge>& l) {Lines=l;}
  void add_line(const edge& l) {Lines.push_back(l);}
  turn_type get_turn() const {return Turn;}
  void set_turn(turn_type t) {Turn = t;}
  double  get_rotate() const {return Rotate;}
  void add_rotate(double a) {Rotate +=a;}
  void set_rotate(double a) {Rotate = a;}
  int  get_clip_width() const {return Clip_Width;}
  void set_clip_width(int w) {Clip_Width = w;}
  int  get_clip_height() const {return Clip_Height;}
  void set_clip_height(int h) {Clip_Height = h;}
  double get_visible_center_x() const {return Visible_Center_X;}
  void   set_visible_center_x(double x) {Visible_Center_X = x;}
  double get_visible_center_y() const {return Visible_Center_Y;}
  void   set_visible_center_y(double y) {Visible_Center_Y = y;}
  int    get_scale() const {return Scale;}
  void   set_scale(int s) {Scale = s;}
  int get_faces_number() const {return Faces.size();}
  face get_face(int i) const {return Faces[i];}
  std::vector<face> get_faces() const {return Faces;}
  void set_faces(const std::vector<face>& f) {Faces=f;}
  void add_face(const face& f) {Faces.push_back(f);}
  void delete_face(int i);
  void delete_all_faces() {Faces.clear();}
  void set_clip(bool c) {Clip = c;}
  bool get_clip() const {return Clip;}
  void reset_data();

  void add_arrow_symbol(arrow_symbol * as) {Arrow_Symbols.push_back(as);}
  cit_arrow_symbols get_arrow_symbols_begin() const {return Arrow_Symbols.begin();}
  cit_arrow_symbols get_arrow_symbols_end() const {return Arrow_Symbols.end();}
  it_arrow_symbols ref_arrow_symbols_begin() {return Arrow_Symbols.begin();}
  it_arrow_symbols ref_arrow_symbols_end() {return Arrow_Symbols.end();}
  
  void fill_update(const step& s);
  void turn_vertices(turn_type);

  void update_dx_dy(int);
  
  //- search methods
  bool symbol_exists(const std::string& s);
  
private:
  std::string Index;
  std::list<std::string> Captions;
  std::vector<vertex> Symbols;
  int Internal_Symbol_Index; //- set by symbol search methods (e.g. symbol_exists)
  std::vector<edge>  Lines; //- edges defined by folds but not integrated in faces
  std::vector<arrow> Arrows;
  turn_type Turn; //- turn over the model after this step ?
  double  Rotate;   //- cummulative angle to rotate after this step
  int  Clip_Width;
  int  Clip_Height;
  double Visible_Center_X;
  double Visible_Center_Y;
  int    Scale;
  std::vector <face> Faces;
  bool   Clip;
  std::list<arrow_symbol*> Arrow_Symbols;
  
#ifdef DEBUG
  public:
    void gdb_info(const std::string & s = std::string()) const;
#endif /* ! DEBUG */
};

#endif  /* ! STEP_H */


