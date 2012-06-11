// This looks like c, but it's really -*-c++-*-
// 
// Copyright (c) 2000 Doodle
// Jerome GOUT - Fri Mar 31 2000
// 

#ifdef WIN32
#pragma warning (disable : 4786)
#include <iostream>
#endif // WIN32

#include <cmath>  //- for sin cos and M_PI
#include <cstdio> //- for sprintf
#include <vector>
#include "step.h"
#include "vertex.h"

step::step() : Index(), Captions(),
	       Symbols(), Internal_Symbol_Index(0), Lines(),
	       Arrows(), Turn(turnNone), Rotate(0.0),
               Clip_Width(CLIP_WIDTH), Clip_Height(CLIP_HEIGHT),
               Visible_Center_X(0.0), Visible_Center_Y(0.0), Scale(100),
               Faces(), Clip(false), Arrow_Symbols()
{}

step::step(int i,int clip_h, int clip_w, int s, double cx, double cy, bool clip) :
  Index(), Captions(),
  Symbols(), Internal_Symbol_Index(0), Lines(),
  Arrows(), Turn(turnNone), Rotate(0.0),
  Clip_Width(clip_w), Clip_Height(clip_h),
  Visible_Center_X(cx), Visible_Center_Y(cy), Scale(s),
  Faces(), Clip(clip), Arrow_Symbols()
{
  set_index(i);
}

step::step(const step& Ref) :
  Index(Ref.Index), Captions(Ref.Captions),
  Symbols(Ref.Symbols),
  Internal_Symbol_Index(Ref.Internal_Symbol_Index),
  Lines(Ref.Lines), Arrows(Ref.Arrows),
  Turn(Ref.Turn), Rotate(Ref.Rotate),
  Clip_Width(Ref.Clip_Width), Clip_Height(Ref.Clip_Height),
  Visible_Center_X(Ref.Visible_Center_X),
  Visible_Center_Y(Ref.Visible_Center_Y), Scale(Ref.Scale),
  Faces(Ref.Faces), Clip(Ref.Clip)
{
  for(cit_arrow_symbols it = Ref.Arrow_Symbols.begin(); it != Ref.Arrow_Symbols.end(); ++it) {
    if(*it == NULL) {
      std::cerr << "A NULL pointer has been added to the arrow symbols list (copy)" << std::endl;
    } else Arrow_Symbols.push_back((*it)->clone());
  }

}

step::~step()
{
  for(cit_arrow_symbols it = Arrow_Symbols.begin(); it != Arrow_Symbols.end(); ++it) {
    if(*it == NULL) {
      std::cerr << "A NULL pointer has been added to the arrow symbols list" << std::endl;
    } else delete *it;
  }
}

void step::set_index(int i)
{
  char s[10];

  if(i > 999999999) std::cerr << "Index too long to be stored in char[10]" <<std::endl;
  else {
    sprintf(s,"%d", i);
    Index = std::string(s);
  }
}

bool step::symbol_exists(const std::string& s)
{
  //- returns true if `s' is allready define in `Symbols', false otherwise
  int nsym = Symbols.size();
  for(int i=0;i < nsym; i++) {
    if(Symbols[i].get_name() == s) {
      Internal_Symbol_Index = i;
      return true;
    }
  }
  return false;
}

const vertex& step::get_current_vertex() const
{
  if (Internal_Symbol_Index < 0 || Internal_Symbol_Index > int(Symbols.size())){
    std::cerr << "Wrong Internal_Symbol_Index access - return first vertex" << std::endl;
    return Symbols[0];
  } else return Symbols[Internal_Symbol_Index];
}

vertex& step::ref_current_vertex()
{
  if (Internal_Symbol_Index < 0 || Internal_Symbol_Index > int(Symbols.size())){
    std::cerr << "Wrong Internal_Symbol_Index access - return first vertex" << std::endl;
    return Symbols[0];
  } else return Symbols[Internal_Symbol_Index];
}

const vertex& step::get_vertex(const std::string & n)
{
  int nsym = Symbols.size();
  for(int i=0;i < nsym; i++) {
    if(Symbols[i].get_name() == n) {
      Internal_Symbol_Index = i;
      return Symbols[i];
    }
  }
  std::cerr << "Wrong vertex access by name (" << n
       << ") - return first vertex" << std::endl;
  return Symbols[0]; 
}

vertex& step::ref_vertex(const std::string & n)
{
  int nsym = Symbols.size();
  for(int i=0;i < nsym; i++) {
    if(Symbols[i].get_name() == n) {
      Internal_Symbol_Index = i;
      return Symbols[i];
    }
  }
  std::cerr << "Wrong vertex access by name (" << n
       << ") - return first vertex" << std::endl;
  return Symbols[0]; 
}

void step::delete_symbol(int i)
{
  int ns = Symbols.size();
  if (i < ns-1) {             //- swap only if not the last one
    Symbols[i] = Symbols[ns]; //- swap with last one
  }
  Symbols.pop_back();         //- delete last
}

edge step::get_line(int i) const
{
  if(i < (int)Lines.size() && i >= 0)
    return Lines[i];
  else {
    std::cerr << "Wrong edge index (" << i << "  this could be a space %) - return first" << std::endl;
    return Lines[0]; 
  }
}

edge& step::ref_line(int i)
{
  if(i < (int)Lines.size() && i >= 0)
    return Lines[i];
  else {
    std::cerr << "Wrong edge index (" << i << "  this could be a space %) - return first" << std::endl;
    return Lines[0]; 
  }
}

void step::delete_face(int i)
{
  //- faces order should respected
  int nf = Faces.size();
  for(int f=i;f<nf-1;f++) {
    Faces[f] = Faces[f+1];  //- translate all elements
  }
  Faces.pop_back(); //- delete last redundant
}

void step::fill_update(const step& s)
{
  //- fill all faces of s into this
  int nfaces = s.Faces.size();
  for(int f=0;f<nfaces;f++) Faces.push_back(s.Faces[f]);
  //- fill all symbols s into this
  int nsym= s.Symbols.size();
  for(int i=0;i<nsym;i++) {
    Symbols.push_back(s.Symbols[i]);
    //- remove eventual debug in previous step
    Symbols[i].set_debug(false);
    //- remove eventual text contained in previous  step
    Symbols[i].clear_text();
  }
  //- copy and update lines from s to this
  int nlines = s.Lines.size();
  for(int l=0;l<nlines; l++){
    Lines.push_back(s.Lines[l]); //- copy
    switch (s.Lines[l].get_type()) {
     case edgeXray: Lines[l].hide(); break; //- xray are ephemerals
     case edgeBorder:
     case edgeFold: break; //- nothing to do they stay unchanged 
     case edgeValley:
     case edgeMountain: {
       //- update lines style
       Lines[l].set_type(edgeFold);
       Lines[l].set_space_v1(FOLDSPC);
       Lines[l].set_space_v2(FOLDSPC);
       break;
     }
    }
    //- Remove eventual debug in previous step
    Lines[l].set_debug(false);
  }
  if(!IS_NULL(s.get_rotate()) || s.get_scale() != Scale) {
    int prevScale = s.get_scale(); //- previous step scale factor
    double resizeFactor = 1.0;
    
    if(prevScale != Scale) resizeFactor = Scale/(double)prevScale;
    for(it_vertices it = Symbols.begin(); it != Symbols.end(); ++it) {
      double dx = it->get_dx();
      double dy = it->get_dy();
      double alpha = s.get_rotate()*M_PI/180.0;
      it->set_dx(resizeFactor * (cos(alpha)*dx-sin(alpha)*dy));
      it->set_dy(resizeFactor * (sin(alpha)*dx+cos(alpha)*dy));
    }
  }
}

void step::turn_vertices(turn_type t)
{
  if(t == turnVertical) { //- vertical mirror
    int nv = Symbols.size();
    for(int v=0;v<nv;v++) {
      Symbols[v].set_x(-Symbols[v].get_x());
      Symbols[v].set_dx(-Symbols[v].get_dx());
    }
  } else { //- horizontal mirror
    int nv = Symbols.size();
    for(int v=0;v<nv;v++) {
      Symbols[v].set_y(-Symbols[v].get_y());
      Symbols[v].set_dy(-Symbols[v].get_dy());
    }
  }
}

void step::reset_data()
{
  Symbols.erase(Symbols.begin(), Symbols.end());
  Lines.erase(Lines.begin(), Lines.end());
  Faces.erase(Faces.begin(), Faces.end());
}

//- update dx dy of all vertices
void step::update_dx_dy(int globalScale)
{
  if(globalScale != Scale) {
    double ratio = ((double)Scale)/globalScale;
    for(it_vertices it=Symbols.begin();	it != Symbols.end();++it) { 
      it->set_dx_dy(it->get_dx()*ratio, it->get_dy()*ratio);
    }
  }
}

//------------------------------------------------------------

#ifdef DEBUG
#include <iostream>

void step::gdb_info(const std::string & s) const 
{
  std::cout << s << "Index       : " << Index << std::endl;
  std::cout << s << "Turn        : " << (int)Turn << std::endl;;
  std::cout << s << "Rotate      : " << Rotate << std::endl;
  std::cout << s << "Clip width  : " << Clip_Width << std::endl;
  std::cout << s << "Clip height : " << Clip_Height << std::endl;
  std::cout << s << "X center    : " << Visible_Center_X << std::endl;
  std::cout << s << "Y center    : " << Visible_Center_Y << std::endl;
  std::cout << s << "Scale       : " << Scale << std::endl;
  std::cout << s << "Clip        : ";
  if(Clip) std::cout << "On" << std::endl;
  else std::cout << "Off" << std::endl;
  //- print caption
  for(cit_captions c=Captions.begin();c!=Captions.end();c++)
    std::cout << s << "Caption     : " << *c << std::endl;
  //- print symbols list ...
  int nsym=Symbols.size();
  for(int i=0; i<nsym; i++) Symbols[i].gdb_info(s+"symbols_");
  //- ... and edges list
  int nlines = Lines.size();
  for(int l = 0; l<nlines; l++) Lines[l].gdb_info(s+"lines_");
  //- ... and arrows list
  int narrows = Arrows.size();
  for(int a = 0; a<narrows; a++) Arrows[a].gdb_info(s+"arrows_");
  //- ... and faces list
  int nfaces = Faces.size();
  for(int f=0;f<nfaces;f++) Faces[f].gdb_info(s+"faces_");
}
#endif /* ! DEBUG */
