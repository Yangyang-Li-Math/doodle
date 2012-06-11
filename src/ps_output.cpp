// This looks like c, but it's really -*-c++-*-
// 
// Copyright (c) 2000-2001 Doodle
// Jerome GOUT - Fri Apr 14 2000
// 

#ifdef WIN32
#pragma warning (disable : 4786)
#else // WIN32
#include <sys/param.h> // for MAX macro
#endif // WIN32
#include <ctime> // for time (seed of srand)
#include <fstream>
#include <vector>
#include "ps_output.h"
#include "step.h"
#include "vect.h"
#include "global_def.h"
#include "diag_header.h"
#include "vertex.h"
#include "ps_prologue.h"

#include <cmath> //- atan2, M_PI, sqrt ...
#include <cstdlib> //- random stuff

//- space between arrows and points (%)
#define ARROWSPC 5
//- arrows length (% of total length)
#define ARROWLG 10
//- arrows head angle (degrees)
#define ARROWHEADANGLE 40

//- width of the range to choice random angle to debug point
#define DEBUG_ARC 120

//- int to string conversion
std::string itoa(int); //- defined in read_instruction.cpp

extern std::vector<step> steps; //- defined in parser.y
extern diag_header info;   //- defined in parser.y
static double upperRightX=0, upperRightY=0; //- save the upper right point of the diagram
static double currentRotate = 0.0;
static double currentScale = 1;
 
static std::streampos pagesPos; //- for write the correct number of pages
//- BEGIN DEBUG
extern bool verbose; //- defined in parser.y
//- END DEBUG

//- string delimiters are parenthesis so escape it if some found
void write_ps_string(std::ostream& out, const std::string& s)
{
  out << '('; //- begin of the PS string
  for(unsigned int i=0; i < s.length(); i++) {
    if(s[i] == '(' || s[i] == ')') out << '\\';
    out << s[i];
  }
  out << ')'; //- end of the PS string
}

void write_ps_line_style(std::ostream& out, const edge& e)
{
  switch (e.get_type()) {
   case edgeBorder :
    out << PS_BORDER_WIDTH / currentScale << " setlinewidth % set border width\n"
	<< "[] 0 setdash   % set solid line for border\n";
    break;
   case edgeValley :
    out << PS_VALLEY_WIDTH / currentScale << " setlinewidth % set valley width\n"
	<< "[5 " << currentScale << " div] 0 setdash  % set dashed line for valley fold\n";
    break;
   case edgeMountain :
    out << PS_MOUNTAIN_WIDTH / currentScale << " setlinewidth % set mountain width\n"
	<< "[5 " << currentScale << " div 3 " << currentScale << " div 1 " << currentScale
	<< " div 3 " << currentScale << " div 1 " << currentScale << " div 3 " 
	<< currentScale << " div] 0 setdash  % set dotted-dashed line for mountain fold\n";
    break;
   case edgeFold :
    out << PS_FOLD_WIDTH / currentScale << " setlinewidth % set fold width\n"
	<< "[] 0 setdash  % set solid line for existing fold\n";
    break;
   case edgeXray :
    out << PS_XRAY_WIDTH / currentScale << " setlinewidth % set xray width\n"
	<< "[1 " << currentScale << " div 2 " << currentScale << " div] 0 setdash  % set dotted line for xray fold\n";
    break;
   default :
     out << PS_FOLD_WIDTH / currentScale << " setlinewidth % set fold width\n"
	<< "[] 0 setdash  % set solid line for existing fold\n";
    break;   
  }
}

void write_ps_vertex_coordinates(std::ostream& out, const vertex & v) {
  if(v.get_dx() == 0 && v.get_dy() == 0) { //- v hasn't delta so put it as it is
    out << TO_PS(v.get_x()) << " cm " << TO_PS(v.get_y()) << " cm ";
  } else {
    out << TO_PS(v.get_x()) << " " << TO_PS(v.get_y()) << " " 
	<< v.get_dx() << " " << v.get_dy() << " "
	<< currentRotate << " " << currentScale << " add_shift ";
  }
}

void write_ps_vertex(std::ostream& out, vertex& v)
{
  if(v.is_debug()) {
    out  <<"gsave 0 0 1 setrgbcolor % blue" << std::endl;
    write_ps_vertex_coordinates(out, v);
    out << std::endl << currentScale << " " << currentRotate << " ";
    write_ps_string(out, v.get_name());
    out << " " << (rand() % DEBUG_ARC) - (DEBUG_ARC/2) << " draw_symbol grestore" << std::endl;
    //- set v not to debug since it has been drawn ...
    v.set_debug(false);
  }
}

vertex compute_vertex_percent(const vertex& v1, const vertex& v2, int sp)
{
  vect v=vect(v1,v2);
  vertex vShift;	
  double norm=v.norm();

  v=(1/norm)*v;
  vShift = v1+(((double) sp)*norm/100.0)*v;
  vShift.set_dx(v1.get_dx()*(double)(100.0-sp)/100.0+v2.get_dx()*(double)sp/100.0);
  vShift.set_dy(v1.get_dy()*(double)(100.0-sp)/100.0+v2.get_dy()*(double)sp/100.0);
  return vShift;
}

void write_ps_line(std::ostream& out, step& s, const edge& e)
{
  vertex& v1=s.ref_vertex(e.get_v1());
  vertex& v2=s.ref_vertex(e.get_v2());

  if(!(v1 == v2)) {
    //- first point treatment
    if(e.has_limit_v1()) {
      //- v1 treatment : put v1.x and v1.y on stack (taking into account v1.dx and v1.dy)
      write_ps_vertex_coordinates(out, v1);
      //- v2 treatment : put v2.x and v2.y on stack (taking into account v1.dx and v1.dy)
      write_ps_vertex_coordinates(out, v2);
      write_ps_vertex_coordinates(out, s.get_vertex(s.get_line(e.get_limit_v1()).get_v1()));
      write_ps_vertex_coordinates(out, s.get_vertex(s.get_line(e.get_limit_v1()).get_v2()));
      out << "intersection ";
    } else {
      write_ps_vertex_coordinates(out, compute_vertex_percent(v1, v2, e.get_space_v1()));
    }
    out << "moveto " << std::endl;
    //- second point treatment
    if (e.has_limit_v2()) {
      //- v1 treatment : put v1.x and v1.y on stack (taking into account v1.dx and v1.dy)
      write_ps_vertex_coordinates(out, v1);
      //- v2 treatment : put v2.x and v2.y on stack (taking into account v1.dx and v1.dy)
      write_ps_vertex_coordinates(out, v2);
      write_ps_vertex_coordinates(out, s.get_vertex(s.get_line(e.get_limit_v2()).get_v1()));
      write_ps_vertex_coordinates(out, s.get_vertex(s.get_line(e.get_limit_v2()).get_v2()));
      out << "intersection ";
    } else {
      write_ps_vertex_coordinates(out, compute_vertex_percent(v2, v1, e.get_space_v2()));
    }
    out << "lineto stroke " << std::endl;
    //- write the name of each extremity if need
    write_ps_vertex(out, v1);
    write_ps_vertex(out, v2);
  }
}

void write_ps_edge(std::ostream& out, step& s, const edge& e)
{
  if (e.is_visible()) {
    vertex& v1 = s.ref_vertex(e.get_v1());
    vertex& v2 = s.ref_vertex(e.get_v2());
        
    if(e.is_debug()) {
      out << std::endl << "gsave 1 0 0 setrgbcolor % red" << std::endl;
      if(e.has_limit_v1()) {
	//- v1 treatment : put v1.x and v1.y on stack (taking into account v1.dx and v1.dy)
	write_ps_vertex_coordinates(out, v1);
	//- v2 treatment : put v2.x and v2.y on stack (taking into account v1.dx and v1.dy)
	write_ps_vertex_coordinates(out, v2);
	write_ps_vertex_coordinates(out, s.get_vertex(s.get_line(e.get_limit_v1()).get_v1()));
	write_ps_vertex_coordinates(out, s.get_vertex(s.get_line(e.get_limit_v1()).get_v2()));
	out << "intersection ";
      } else {
	write_ps_vertex_coordinates(out, compute_vertex_percent(v1, v2, e.get_space_v1()));
      }
      //write_ps_vertex_coordinates(out, v1);
      //- second point treatment
      if (e.has_limit_v2()) {
	//- v1 treatment : put v1.x and v1.y on stack (taking into account v1.dx and v1.dy)
	write_ps_vertex_coordinates(out, v1);
	//- v2 treatment : put v2.x and v2.y on stack (taking into account v1.dx and v1.dy)
	write_ps_vertex_coordinates(out, v2);
	write_ps_vertex_coordinates(out, s.get_vertex(s.get_line(e.get_limit_v2()).get_v1()));
	write_ps_vertex_coordinates(out, s.get_vertex(s.get_line(e.get_limit_v2()).get_v2()));
	out << "intersection ";
      } else {
	write_ps_vertex_coordinates(out, compute_vertex_percent(v2, v1, e.get_space_v2()));
      }
      //write_ps_vertex_coordinates(out, v2);
      out << currentScale << " " << currentRotate << " ([" << e.get_v1()
	  << ", " << e.get_v2() << "]) " << (rand() % DEBUG_ARC) - (DEBUG_ARC/2)
	  << " draw_symbol_at_middle grestore" << std::endl;
    }
    write_ps_line_style(out, e);
    write_ps_line(out, s, e);
  }
}

void write_ps_header(std::ostream& out, const char * f)
{
   out << "%!PS-Adobe-3.0\n%%Creator: Doodle -- http://doodle.sourceforge.net\n%%Title: "
       << f  << std::endl
       << "%%DocumentPaperSizes: A4" << std::endl;
   pagesPos = out.tellp();
   out << "%%Pages: 000\n%%EndComments\n\n";
   ps_prologue(out);
}

void write_ps_declare_font(std::ostream& out, int size)
{
  out << "/Times-Roman findfont % Get basic font\n" << size
      << " scalefont          % " << size << " points font\n"
      << "setfont               % Make it the current font\n\n";
}

void write_ps_face(std::ostream& out, step& s, const face& f)
{
  int nvert = f.get_symbols_number();
  int r,g,b;

  out << "gsave % begin fill face" << std::endl;
  //- set the correct color
  f.get_color().get_rgb_values(r,g,b);
  out << r/100.0 << " " << g/100.0 << " " << b/100.0 << " setrgbcolor" << std::endl;
  //- define path and fill it
  write_ps_vertex_coordinates(out, s.get_vertex(f.get_symbol(0)));
  out << "moveto";
  for(int v=1; v<nvert; v++) {
    out << std::endl;
    write_ps_vertex_coordinates(out, s.get_vertex(f.get_symbol(v)));
    out << "lineto";
  }
  out << " fill" << std::endl;
  out << "grestore % end fill face" << std::endl;
}

void write_ps_head_simple_arrow(std::ostream& out, arrow_type type, double& angle, 
				bool reverse, vertex c, double r, double headsize) 
{
  out << "gsave" << std::endl;   
  vertex pt=c+r*vect(cos(angle*M_PI/180),sin(angle*M_PI/180));
  double angle2=angle;
  if (reverse) angle2+=180;
  out << TO_PS(pt.get_x()) << " cm " << TO_PS(pt.get_y())
      << " cm translate" << std::endl;

  vertex hr(headsize*sin(ARROWHEADANGLE*M_PI/360),
	    headsize*cos(ARROWHEADANGLE*M_PI/360));
  vertex hl(-headsize*sin(ARROWHEADANGLE*M_PI/360),
	    headsize*cos(ARROWHEADANGLE*M_PI/360));
  out << angle2 << " rotate" << std::endl;
  double dangle;
  switch(type){
  case arrowValley :
    out <<  TO_PS(hr.get_x())<< " cm "<< TO_PS(hr.get_y()) << " cm moveto" << std::endl;
    out << "0 0 lineto " <<  TO_PS(hl.get_x())<< " cm "
	<< TO_PS(hl.get_y()) << " cm lineto stroke" << std::endl;
     break;
  case arrowUnfold :
    out << "1 1 1 setrgbcolor" << std::endl;
    out << "0 0 moveto " <<  TO_PS(hr.get_x())<< " cm "
	<< TO_PS(hr.get_y()) << " cm lineto" << std::endl;
    out << TO_PS(hl.get_x())<< " cm " << TO_PS(hl.get_y()) 
	<< " cm lineto closepath fill" << std::endl;
    out << "0 0 0 setrgbcolor" << std::endl;
    out << "0 0 moveto " <<  TO_PS(hr.get_x())<< " cm "
	<< TO_PS(hr.get_y()) << " cm lineto" << std::endl;
    out << TO_PS(hl.get_x())<< " cm " << TO_PS(hl.get_y()) 
	<< " cm lineto closepath stroke" << std::endl;
    dangle=headsize*180.0/(r*M_PI);
    if (reverse) angle-=dangle;
    else angle+=dangle;
    break;
  case arrowNone :
    break;
  case arrowMountain :
    if(!reverse) {
      out << "0 0 moveto " <<  TO_PS(hr.get_x())<< " cm "
	  << TO_PS(hr.get_y()) << " cm lineto" << std::endl;
      out << TO_PS((hl.get_x()+hr.get_x())/2)<< " cm " 
	  << TO_PS((hl.get_y()+hr.get_y())/2) << " cm lineto stroke" << std::endl;
    } else {
       out << "0 0 moveto " <<  TO_PS(hl.get_x())<< " cm "
	  << TO_PS(hl.get_y()) << " cm lineto stroke" << std::endl;
      out <<  TO_PS((hl.get_x()+hr.get_x())/2)<< " cm " 
	  << TO_PS((hl.get_y()+hr.get_y())/2) << " cm lineto stroke" << std::endl;
    }
    break;
  }
  out << "grestore" << std::endl;
}

void write_ps_head_return_arrow(std::ostream& out, const vertex& v, const vect& u,
				double headSize, arrow_type type, bool rightSide)
{
  out << "gsave" << std::endl;
  out << TO_PS(v.get_x()) << " cm " << TO_PS(v.get_y()) << " cm translate" <<std::endl;
  out << atan2(-u.get_x(), u.get_y())*180/M_PI << " rotate" << std::endl;
  double headLength = headSize/4;
  vertex pr(headLength*sin(ARROWHEADANGLE*M_PI/360), headLength*cos(ARROWHEADANGLE*M_PI/360));
  vertex pl(-headLength*sin(ARROWHEADANGLE*M_PI/360), headLength*cos(ARROWHEADANGLE*M_PI/360));
  vertex pt = rightSide ? pr : pl;
  switch (type) {
  case arrowValley : 
    out << TO_PS(pr.get_x()) << " cm " << TO_PS(pr.get_y()) << " cm moveto" << std::endl;
    out << "0 0 lineto " << TO_PS(pl.get_x()) << " cm " 
	<< TO_PS(pl.get_y()) << " cm lineto stroke" << std::endl; break;
  case arrowUnfold :  
    out << "1 1 1 setrgbcolor" << std::endl;
    out << "0 0 moveto " << TO_PS(pr.get_x()) << " cm " 
	<< TO_PS(pr.get_y()) << " cm lineto " << std::endl
	<< TO_PS(pl.get_x()) << " cm " 
	<< TO_PS(pl.get_y()) << " cm lineto closepath fill" << std::endl;
    out << "0 0 0 setrgbcolor" << std::endl;
    out << "0 0 moveto " << TO_PS(pr.get_x()) << " cm " 
	<< TO_PS(pr.get_y()) << " cm lineto " << std::endl
	<< TO_PS(pl.get_x()) << " cm " 
	<< TO_PS(pl.get_y()) << " cm lineto closepath stroke" << std::endl;  
    break;
  case arrowMountain :
    out << "0 0 moveto " << TO_PS(pt.get_x()) << " cm " 
	<< TO_PS(pt.get_y()) << " cm lineto " << std::endl
 	<< TO_PS((pr.get_x()+pl.get_x())/2) << " cm "
 	<< TO_PS((pr.get_y()+pl.get_y())/2) << " cm lineto stroke" << std::endl;
    break;
    
  case arrowNone : break;
  }
  out << "grestore" << std::endl;
}

void write_ps_simple_arrow(std::ostream& out, step& s, const arrow& a)
{
  vertex v1(s.get_vertex(a.get_v1()).apply_shift(currentRotate, currentScale*100.0)); //- get source arrow
  vertex v2(s.get_vertex(a.get_v2()).apply_shift(currentRotate, currentScale*100.0)); //- get destination arrow

  out << PS_BORDER_WIDTH / currentScale << " setlinewidth % set arrow width\n"
      << "[] 0 setdash   % set solid line for arrow\n";

  vertex p = v1.middle(v2);
  vertex c;
  vect u = vect(v1, v2);
  double alpha_r = a.get_shape()*M_PI/180;
  double d = u.norm();
  u=(1/d)*u;
  double r = d/(2*sin(alpha_r/2));
  
  vect v = u.ortho();
  if(a.get_side() == arrowRight) c = p + (r*cos(alpha_r/2))*v;
  else  c = p + (-r*cos(alpha_r/2))*v;
  vect w = vect(c, v1);
  double a1 = atan2(w.get_y(), w.get_x())*180/M_PI;
  double a2;
  if(a.get_side() == arrowRight) {
    a2 = a1+a.get_shape();
  } else {
    a2 = a1;
    a1 = a2-a.get_shape();
  }
  // adjustments (arrows don't reach points)
  a1 += a.get_shape()*(ARROWSPC/100.0);
  a2 -= a.get_shape()*(ARROWSPC/100.0);
  // arrows heads
  double headsize=((double)ARROWLG)*(a2-a1)*M_PI*r/(180*100);
  if ((a.get_v1_type()!=arrowNone && a.get_side() == arrowRight)
      ||(a.get_v2_type()!=arrowNone && a.get_side() == arrowLeft)){
    arrow_type type = (a.get_side()==arrowRight) ? a.get_v1_type():
		                                   a.get_v2_type();
    write_ps_head_simple_arrow(out,type,a1,false,c,r,headsize);
  }
  if ((a.get_v2_type()!=arrowNone && a.get_side() == arrowRight)
      ||(a.get_v1_type()!=arrowNone && a.get_side() == arrowLeft)){
    arrow_type type = (a.get_side()==arrowRight) ? a.get_v2_type():
		                                   a.get_v1_type();
    write_ps_head_simple_arrow(out,type,a2,true,c,r,headsize);
  }

  // arrow body
  out << TO_PS(c.get_x()) << " cm " << TO_PS(c.get_y()) << " cm " 
      << TO_PS(r) << " cm " << a1 << " " << a2 << " arc stroke" << std::endl;
}

void write_ps_return_arrow(std::ostream& out, step& s, const arrow& a)
{

  vertex begin = s.get_vertex(a.get_v1()).apply_shift(currentRotate, currentScale*100.0);
  vertex end = s.get_vertex(a.get_v2()).apply_shift(currentRotate, currentScale*100.0);
  vect be = vect(begin,end);
  vect u = be/10;
  vect v = u.ortho();
  if(a.get_side() == arrowRight) v = -1*v;
  vect uProp1 = 50.0/(100.0 - a.get_shape())*u;
  vect uProp2 = a.get_shape()/(100.0 - a.get_shape())*u;
  vect vProp2 = a.get_shape()/(100.0 - a.get_shape())*v;
  vertex ctrlpts1 = begin+8*uProp1+5*v;
  vertex ctrlpts2 = begin+20*uProp1+3*v;
  vertex ctrlpts3 = begin+20*uProp1+v;
  vertex ctrlpts4 = begin+20*uProp1+(-1*v);
  vertex ctrlpts5 = begin+10*u+(15-10)*uProp2+(-2)*vProp2;
  vertex ctrlpts6 = begin+10*u+(11-10)*uProp2+(-1)*vProp2;

  out << PS_BORDER_WIDTH / currentScale << " setlinewidth % set arrow width\n"
      << "[] 0 setdash   % set solid line for arrow\n";
  //- arrow body
  out << TO_PS(begin.get_x()) << " cm " << TO_PS(begin.get_y())
      << " cm moveto" << std::endl
      << TO_PS(ctrlpts1.get_x()) << " cm " << TO_PS(ctrlpts1.get_y())
      << " cm" << std::endl
      << TO_PS(ctrlpts2.get_x()) << " cm " << TO_PS(ctrlpts2.get_y())
      << " cm"  << std::endl
      << TO_PS(ctrlpts3.get_x()) << " cm " << TO_PS(ctrlpts3.get_y())
      << " cm curveto " << std::endl
      << TO_PS(ctrlpts4.get_x()) << " cm " << TO_PS(ctrlpts4.get_y())
      << " cm"  << std::endl
      << TO_PS(ctrlpts5.get_x()) << " cm " << TO_PS(ctrlpts5.get_y())
      << " cm"  << std::endl
      << TO_PS(ctrlpts6.get_x()) << " cm " << TO_PS(ctrlpts6.get_y())
      << " cm" << " curveto stroke % return arrow" << std::endl;
  //- arrow heads
  bool rightSide = a.get_side() == arrowRight ? true : false;
  write_ps_head_return_arrow(out, begin, vect(begin, ctrlpts1), be.norm(), a.get_v1_type(), rightSide);
  write_ps_head_return_arrow(out, ctrlpts6, vect(ctrlpts6, ctrlpts5), be.norm(), a.get_v2_type(), !rightSide);
}

void write_ps_turn(std::ostream& out)
{
  out << upperRightX << " mm " << upperRightY << " mm turn_over" << std::endl;
}

void write_ps_rotate(std::ostream& out, double a)
{
  out << "% begin rotate" << std::endl;
  out << upperRightX << " mm " << upperRightY << " mm (" << abs(a) << ") ";
  if(a > 0) out << "true rotate_step" << std::endl;
  else out << "false rotate_step" << std::endl;
  //- apply the rotation to the next steps ...
  currentRotate += a;
}

void write_ps_caption(std::ostream& out, step& s)
{
  write_ps_declare_font(out, 11);
  //- computes height of chosen font
  out << "/hline {0 (Mj) width_height exch pop} def" << std::endl;
  out << upperRightX*2 << " mm -0.4 mul "
      << -upperRightY << " mm interline hline add sub moveto" << std::endl
      << "(" << s.get_index() << "-) show" << std::endl;
  //- write text. It always contains almost one caption (maybe empty)
  int i = 1;
  for(step::cit_captions c=s.get_captions_begin();c!=s.get_captions_end();c++, i++) {
    out << upperRightX*2 << " mm -0.3 mul "
	<< -upperRightY << " mm interline hline add "
	<< i << " mul sub moveto" << std::endl;
    write_ps_string(out, *c);
    out << " show" << std::endl;
  }
}

void write_ps_step(std::ostream& out, step& s, bool cap=true)
{
  int nlines = s.get_lines_number();
  int narrows = s.get_arrows_number();
  int nfaces = s.get_faces_number();

  out << "gsave % begin figure" << std::endl; 

  if (currentRotate) out << currentRotate << " rotate" << std::endl;
  //- declare font for debug info of this step
  write_ps_declare_font(out, 12);
  //- scaling if it's needed
   currentScale = s.get_scale()/100.0;
  if(currentScale != 1.0) {
    out << currentScale << " " << currentScale
	<< " scale % scaling the step" << std::endl;
  }
  //- recentering the figure in the clipping area
  out << TO_PS(-s.get_visible_center_x()) << " cm "
      << TO_PS(-s.get_visible_center_y())
      << " cm translate % recentering the step" << std::endl;

  //- draw faces
  for(int f=0;f<nfaces;f++){
    write_ps_face(out, s,s.get_face(f));
  }
  //- draw each edge
  for(int i=0;i<nlines;i++) {
    write_ps_edge(out, s, s.get_line(i));
  }

  //- draw each arrow
  for(int j=0;j<narrows;j++) {
    arrow a = s.get_arrow(j);
    if(a.is_simple()) write_ps_simple_arrow(out, s, a);
    else write_ps_return_arrow(out, s, a);
  }

  //- write text of vertices if needed
  for(step::it_vertices it = s.ref_begin_vertices();
      it != s.ref_end_vertices(); ++it) {
    if(it->has_text()) { 
      write_ps_vertex_coordinates(out, *it);
      out << std::endl << "gsave 0 0 0 setrgbcolor % black" << std::endl;
      out << currentScale << " " << currentRotate << " ";
      write_ps_string(out, it->get_text());
      out << " " << (rand() % DEBUG_ARC) - (DEBUG_ARC/2) <<  " draw_symbol grestore" << std::endl;
    }
  }

  //- draw all arrow symbols store in this step
  for(step::cit_arrow_symbols itA = s.get_arrow_symbols_begin();
      itA != s.get_arrow_symbols_end(); ++itA) {
    (*itA)->write_ps_draw(out,s, currentRotate, currentScale*100.0);
  }
  
  out << "grestore % end figure" << std::endl;
  if(s.get_clip()) out << "grestore % end clip" << std::endl;

  //- draw caption
  if(cap) write_ps_caption(out, s);
  //- is there a turn symbol to draw ??
  if(s.get_turn() != turnNone) {
    write_ps_turn(out);
  }
  //- is there a rotate symbol to draw ??
  double r = s.get_rotate();
  if(r) {
    if(s.get_turn() != turnNone) { // there is already a symbol drawn
      upperRightY -= 15;           // decrease of 15 mm
    }
    write_ps_rotate(out, r);
  }
}  

void write_ps_first_page_header(std::ostream& out)
{
  write_ps_string(out, info.get_title());  out << std::endl;
  std::string ss = std::string("Design: ") + info.get_designer(); 
  if(info.get_design_date().get_year()) {
    ss += std::string(" - Copyright ");
    ss += itoa(info.get_design_date().get_year());
  }
  write_ps_string(out, ss);  out << std::endl;
  ss = std::string("Diagrams: ") + info.get_diagrammer();
  if(info.get_diagram_date().get_year()) {
    ss += std::string(" - Copyright ");
    ss += itoa(info.get_diagram_date().get_year());
  }
  write_ps_string(out, ss); out << std::endl;
  write_ps_string(out, info.get_comment1()); out << std::endl;
  write_ps_string(out, info.get_comment2()); out << std::endl;
  write_ps_string(out, info.get_comment3());
  out << info.get_left_margin() << " mm " << info.get_right_margin() 
      << " mm make_first_page_header" << std::endl;
}

void write_ps_page_header(std::ostream& out)
{
  out << "gsave % begin page header" << std::endl;
  out << "/Helvetica-Bold findfont 16 scalefont setfont" << std::endl;
  out << info.get_left_margin() << " mm " << PAGE_HEIGHT-info.get_top_margin()
      << " mm 3 mm add moveto ";
  write_ps_string(out, info.get_title());
  out << " show" << std::endl;
  out << "/Helvetica findfont 12 scalefont setfont" << std::endl;
  out << PAGE_WIDTH-info.get_right_margin()<< " mm 0 (" << info.get_designer() 
      << ") width_height pop sub " << PAGE_HEIGHT-info.get_top_margin()
      << " mm 3 mm add moveto ";
  write_ps_string(out, info.get_designer());
  out << " show stroke" << std::endl;
  out << "3 setlinewidth [] 0 setdash" << std::endl;
  out << info.get_left_margin() << " mm " << PAGE_HEIGHT-info.get_top_margin()
      << " mm moveto " << PAGE_WIDTH-info.get_right_margin() << " mm " 
      << PAGE_HEIGHT-info.get_top_margin() << " mm lineto stroke" << std::endl;
  out << "grestore % end page header" << std::endl;
}

void write_ps_page_footer(std::ostream& out, int pageNb)
{
  out << "gsave % begin page footer" << std::endl;
  out << "3 setlinewidth [] 0 setdash" << std::endl;
  out << info.get_left_margin() << " mm " << info.get_bottom_margin()
      << " mm moveto " << PAGE_WIDTH-info.get_right_margin() << " mm " 
      << info.get_bottom_margin() << " mm lineto stroke" << std::endl;
  out << "/Times-Roman findfont 12 scalefont setfont" << std::endl;
  out << PAGE_WIDTH << " mm 0 (" << pageNb << ") width_height" << std::endl
      << "3 1 roll sub 2 div exch " << info.get_bottom_margin() 
      << " mm exch sub 3 mm sub moveto" << std::endl << "(" << pageNb << ") show"
      << std::endl;
  out << "grestore % end page footer" << std::endl;
  out << "showpage\n";
}

void write_ps_steps(std::ostream& out, const char * f) 
{
  int nbSteps = steps.size();
  int stepId=0;
  int topMarginMin = FIRST_TOP_MARGIN;
  int bottomMarginMin = info.get_bottom_margin();
  int leftMarginMin = info.get_left_margin();
  int rightMarginMin = info.get_right_margin();
  int vSpace = info.get_vspace();
  int hSpace = info.get_hspace();
  int pageId = 1;

  //- Special case of empty first page
  if(nbSteps == 0) {
    write_ps_first_page_header(out);
    write_ps_page_footer(out, pageId);
    pageId++; //- to print the right number of drawn pages (1)
  }
  //- Other case nbSteps > 0
  while(stepId < nbSteps) {
    int sumHeight=0;
    int numLine = 0;
    int maxHeights[100];
    int nbStepsByLine[100];
    int sumWidth[100];
    bool nextPage = false; 
    int stepId2Write = stepId;

    out << "\n%%Page: " << pageId << " " << pageId << std::endl;
    while(!nextPage) {
      sumWidth[numLine] = 0;
      bool nextLine = false;
      maxHeights[numLine] = 0;
      nbStepsByLine[numLine] = 0;
      
      while(!nextLine) {
	if(sumWidth[numLine]+steps[stepId].get_clip_width() < 
	   (PAGE_WIDTH-leftMarginMin-rightMarginMin-
	    (nbStepsByLine[numLine]-1)*hSpace)) {
	  sumWidth[numLine] += steps[stepId].get_clip_width();
	  maxHeights[numLine] = MAX(maxHeights[numLine],
				    steps[stepId].get_clip_height()+
				    steps[stepId].get_captions_number()*
				    CAPTION_HEIGHT_MM);
	  stepId++;
	  if(stepId >= nbSteps) {
	    nextLine = true;
	    nextPage = true;
	  }
	  nbStepsByLine[numLine]++;
	} else {
	  nextLine = true;
	}
      }
      if(pageId > 1) topMarginMin = info.get_top_margin();
      if(sumHeight + maxHeights[numLine] + topMarginMin +
	 bottomMarginMin + (numLine+1)*vSpace < PAGE_HEIGHT) {
	sumHeight += maxHeights[numLine];
	numLine++;
      } else {
	stepId -= nbStepsByLine[numLine];
	nextPage = true;
      }
    }
    // Now we can write the whole page ...
    int x, y; // axis origin for each step
    int currentWidth;
    int currentHeight=0;
    int interHSpace;
    int interVSpace;

    if(pageId > 1) write_ps_page_header(out);
    else write_ps_first_page_header(out);
    for(int line=0;line<numLine; line++) {
      currentWidth = 0;
      //- y :
      if(stepId != nbSteps) {
	interVSpace = (PAGE_HEIGHT-topMarginMin-bottomMarginMin-sumHeight)/numLine;
      } else {
	interVSpace = vSpace;
      }
      y = PAGE_HEIGHT - (int)(topMarginMin + maxHeights[line]/2 + currentHeight + (line+0.5)*interVSpace);
      currentHeight += maxHeights[line];
      for(int col=0;col<nbStepsByLine[line]; col++) {
	//- x :
	if(nbStepsByLine[line] == 1) { //- only one step on this line, center the step
	  x = PAGE_WIDTH / 2;
	} else {
	  interHSpace=(PAGE_WIDTH-leftMarginMin-rightMarginMin-sumWidth[line])/(nbStepsByLine[line]-1);
	  x = leftMarginMin + currentWidth + col*interHSpace + steps[stepId2Write].get_clip_width()/2;
	  currentWidth += steps[stepId2Write].get_clip_width();
	}
	out << "gsave % begin step" << std::endl;
	out << x << " mm " << y << " mm translate\n";
	//- defining the step clipping zone
	int w = steps[stepId2Write].get_clip_width();
	int h = steps[stepId2Write].get_clip_height();
	//- export clipping zone values in globals
	upperRightX = w/2; upperRightY = h/2;
	if(steps[stepId2Write].get_clip()) {
	  out << "gsave % begin clip" << std::endl;
	  out << "newpath % begin of clipping zone" << std::endl;
	  out << -w/2 << " mm " << h/2 << " mm moveto" << std::endl
	      << w/2 << " mm " << h/2 << " mm lineto" << std::endl
	      << w/2 << " mm " << -h/2 << " mm lineto" << std::endl
	      << -w/2 << " mm " << -h/2 << " mm lineto closepath clip";
	  out << " newpath % end of clipping zone" << std::endl;
	}
	write_ps_step(out, steps[stepId2Write]);
	out << "grestore % end step" << std::endl;
	stepId2Write++;
      }
    }
    write_ps_page_footer(out, pageId);
    pageId++;
  }
  //- Now we have the total number of pages .. we can write it inside PS
  out.seekp(pagesPos);
  out << "%%Pages: ";
  out.width(3);
  out << pageId-1 << std::endl;
  std::cout << "Output file     : " << f << std::endl;
  std::cout << "Number of steps : " << nbSteps << std::endl;
  std::cout << "Number of pages : " << pageId-1 << std::endl;
}

void ps_output(const char * f)
{
  std::ofstream out(f);

  srand((unsigned int)time(NULL));
  write_ps_header(out,f);
  write_ps_steps(out,f);
  out.close();
}
 
