// This looks like c, but it's really -*-c++-*-
// 
// Copyright (c) 2000 Doodle
// Jerome GOUT - Wed Apr  5 2000
// 
#ifdef WIN32
#pragma warning (disable : 4786)
#include <iostream>
#include <fstream>
#endif // WIN32

#include <algorithm>
#include <vector>
#include <map>

#include "step.h"
#include "vertex.h"
#include "arrow.h"
#include "global_def.h"
#include "vect.h"
#include "edge.h"
#include "color.h"
#include "diag_header.h"
#include "push_arrow.h"
#include "open_arrow.h"
#include "repeat_arrow.h"
#include "ps_prologue.h" //- used in eps operator

#include <cstdio> //- for sprintf

extern diag_header info; //- defined in parser.y
extern std::vector<step> steps;   //- defined in parser.y
extern void yyerror(char*); //- defined in parser.y
extern void yywarning(char*); //- defined in parser.y
extern int current_step;      //- defined in parser.y
extern int current_index;     //- defined in parser.y
extern bool is_sub_step;      //- defined in parser.y
extern bool verbose;          //- defined in parser.y
extern int  visible_height;   //- defined in parser.y
extern int  visible_width;    //- defined in parser.y
extern int  scale;            //- defined in parser.y
extern std::string center;    //- defined in parser.y
extern bool clip;             //- defined in parser.y

std::map<std::string, unsigned int> labels;

std::string itoa(int i)
{
  char s[10];
  
  if(i < 1000000000) sprintf(s,"%d",i);
  else  yyerror("Can't handle number size greater than 10 digits");
  return std::string(s);
}

void symbol_already_defined(const std::string& s)
{
  std::string ss = s + " is already defined in the current context";
  yyerror((char *)ss.c_str());
}

void undefined_symbol(const std::string &s)
{
  std::string ss = s + " is undefined in the current context";
  yyerror((char *)ss.c_str());
}

void undefined_label(const std::string &s)
{
  std::string ss = s + " is an undefined label in the current context";
  yyerror((char *)ss.c_str());
}

void undefined_edge(const std::string &s1, const std::string &s2)
{
  std::string ss = "edge ["+s1+", "+s2+"] hasn't been yet defined";
  yyerror((char *)ss.c_str());
}

arrow_type check_arrow_type(const std::string& at)
{
  if(at == "valley") return arrowValley;
  else if(at == "mountain") return arrowMountain;
  else if(at == "unfold") return arrowUnfold;
  else if(at == "none") return arrowNone;
  else {
    std::string ss = at + " is not a valid arrow type";
    yyerror((char *)ss.c_str());
    //- never reached - just for avoid warning
    return arrowNone;
  }
}

arrow_side check_arrow_side(const std::string& as)
{
  if(as == "left") return arrowLeft;
  else if(as == "right") return arrowRight;
  else {
    std::string ss = as + " is not a valid arrow side (left/right)";
    yyerror((char *)ss.c_str());
    //- never reached - just for avoid warning
    return arrowLeft;
  }
}

bool check_first_second(const std::string& fs)
{
  if(fs == "first") return true;
  else if(fs == "second") return false;
  else {
    std::string ss = fs + " is not a valid expected keyword (first/second)";
    yyerror((char *)ss.c_str());
    //- never reached - just for avoid warning
    return true;
  }
}

int check_paper_format(const std::string& ratio) 
{
  if(ratio == "A") return (int)(sqrt(2)*100);
  if(ratio == "dollar") return 235;
  std::string ss;
  ss = ratio + " is not a known paper format";
  yyerror((char *)ss.c_str());
  //- never reached ... to avoid warning
  return 0;
}

//- looks for "\ref([^).]+)" regexpr in s
//- if some is found replace by the step index corresponding to the label
std::string expand_ref(const std::string& s)
{
  std::string expanded = s, label;
  std::string::size_type begin = s.find("\\ref("), end;

  while(begin != std::string::npos) {
    if(begin != std::string::npos) {
      expanded = std::string(s, begin,s.size()-begin);
      end = s.find(")");
      if(end != std::string::npos) {
	label = s.substr(begin+5, end-(begin+5));
	if(labels.find(label) == labels.end()) {
	  std::string error = "Label '" + label + "' referenced but never defined";
	  yyerror((char*)error.c_str());
	  return s;
	} 
	expanded = s.substr(0, begin-1) + itoa(labels[label]) + s.substr(end+1);
      } else {
	yyerror("Missing a ')' to close the \\ref operator");
	return s;
      }
    }
    begin = s.find("\\ref(", end);
  }
  return expanded;
}

void wrong_percent(int p)
{
  std::string ss;
  
  ss = itoa(p) + " is an invalid percent value ([0, 100])";
  yyerror((char *)ss.c_str());
}

void vertices_equivalent(const std::string& s1, const std::string& s2)
{
  std::string ss;

  ss = s1 + " and " + s2 + " are equivalent (norm([" +s1+", "+s2+"])<1e-6)";
  yyerror((char *)ss.c_str());
}

void error_edges_parallel(const std::string& s1, const std::string& s2,
		    const std::string& s3, const std::string& s4)
{
  std::string ss = "physical edges [";

  ss += s1 + ", " + s2 + "] and [" + s3 + ", " + s4 + "] are parallel";
  yyerror((char *)ss.c_str());
}

void error_edges_orthogonal(const std::string& s1, const std::string& s2,
		    const std::string& s3, const std::string& s4)
{
  std::string ss = "edges [";
  ss += s1 + ", " + s2 + "] and [" + s3 + ", " + s4 + "] are orthogonal";
  yyerror((char *)ss.c_str());
}

std::string gen_sym(const std::string& s)
{
  std::string ss = s;
  while(steps[current_step].symbol_exists("_"+ss)) ss = "_"+ss;
  return "_"+ss;
}

color front_or_back(const std::string& s)
{
  if(s == "front") return info.get_front_color();
  else if(s == "back") return info.get_back_color();
  else yyerror("Fill color should be defined as 'back' or 'front'");
  return color(100, 100, 100); //- White instead of
}

int search_edge(const std::string& s1, const std::string& s2)
{
  bool found = false;
  int nlines = steps[current_step].get_lines_number(), l;
  for(l=0;l<nlines && !found;l++) {
    if(steps[current_step].get_line(l).equal(s1, s2)) {
      found = true;
    }
  }
  if(found) return l-1;
  else return -1;
}

int search_edge_and_hide_equivalent(const std::string& s1, const std::string& s2)
{
  bool found = false;
  int nlines = steps[current_step].get_lines_number(), l;
  vertex v1=steps[current_step].get_vertex(s1);
  vertex v2=steps[current_step].get_vertex(s2);

  for(l=0;l<nlines && !found;l++) {
    if(steps[current_step].get_line(l).equal(s1, s2)) {
      found = true;
    } else {
      //- if is not the searched edge it can be an equivalent edge
      //- same coordinates vertices but other symbol names
      vertex vl1 = steps[current_step].get_vertex(steps[current_step].get_line(l).get_v1());
      vertex vl2= steps[current_step].get_vertex(steps[current_step].get_line(l).get_v2());
      if((vl1.equivalent(v1) && vl2.equivalent(v2)) || (vl1.equivalent(v2) && vl2.equivalent(v1))) {
	//- the current edge is equivalent to the searched one
	steps[current_step].ref_line(l).hide();
      }
    }
  }
  if(found) return l-1;
  else return -1;
}
//-----------------------------------------------------------------------

void read_new_step()
{
  double cx=0.0, cy=0.0;
  if(!center.empty()) {
    vertex v = steps[current_step-1].get_vertex(center);
    cx = v.get_x();
    cy = v.get_y();
  }
  step s(current_index, visible_height, visible_width, scale, cx, cy, clip);
  //- after first step we should build new step from last one
  if(current_step >= 1) { 
    s.fill_update(steps[current_step-1]);
    if(steps[current_step-1].get_turn() != turnNone) { //- previous has a turn
      s.turn_vertices(steps[current_step-1].get_turn());
    }
  }
  steps.push_back(s);
}

void read_diamond(const std::string& s1, const std::string& s2, const std::string& s3, const std::string& s4)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : a diamond " << s1 << " " << s2 << " " << s3
	 << " " << s4 << " is defined ..." << std::endl;
  }
  //- Define the symbol $3, $5, $7, $9 and edge $3-$5, $5-$7, $7-$9, $9-$3 as border 
  if(steps[current_step].symbol_exists(s1)) {symbol_already_defined(s1); return;}
  if(steps[current_step].symbol_exists(s2)) {symbol_already_defined(s2); return;}
  if(steps[current_step].symbol_exists(s3)) {symbol_already_defined(s3); return;}
  if(steps[current_step].symbol_exists(s4)) {symbol_already_defined(s4); return;}
  
  vertex v1(s1, 0, DIAMOND_EDGE);
  vertex v2(s2, DIAMOND_EDGE, 0);
  vertex v3(s3, 0, -DIAMOND_EDGE);
  vertex v4(s4, -DIAMOND_EDGE, 0);

  steps[current_step].add_symbol(v1);
  steps[current_step].add_symbol(v2);
  steps[current_step].add_symbol(v3);
  steps[current_step].add_symbol(v4);

  steps[current_step].add_line(edge(s1, s2));
  steps[current_step].add_line(edge(s2, s3));
  steps[current_step].add_line(edge(s3, s4));
  steps[current_step].add_line(edge(s4, s1));
}

void read_square(const std::string& s1, const std::string& s2, const std::string& s3, const std::string& s4)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : a square " << s1 << " " << s2 << " " << s3
	 << " " << s4 << " is defined ..." << std::endl;
  }
  //- Define the symbol $3, $5, $7, $9 and edge $3-$5, $5-$7, $7-$9, $9-$3 as border 
  if(steps[current_step].symbol_exists(s1)) {symbol_already_defined(s1); return;}
  if(steps[current_step].symbol_exists(s2)) {symbol_already_defined(s2); return;}
  if(steps[current_step].symbol_exists(s3)) {symbol_already_defined(s3); return;}
  if(steps[current_step].symbol_exists(s4)) {symbol_already_defined(s4); return;}
  
  vertex v1(s1, -SQUARE_EDGE, SQUARE_EDGE);
  vertex v2(s2, SQUARE_EDGE, SQUARE_EDGE);
  vertex v3(s3, SQUARE_EDGE, -SQUARE_EDGE);
  vertex v4(s4, -SQUARE_EDGE, -SQUARE_EDGE);

  steps[current_step].add_symbol(v1);
  steps[current_step].add_symbol(v2);
  steps[current_step].add_symbol(v3);
  steps[current_step].add_symbol(v4);

  steps[current_step].add_line(edge(s1, s2));
  steps[current_step].add_line(edge(s2, s3));
  steps[current_step].add_line(edge(s3, s4));
  steps[current_step].add_line(edge(s4, s1));
}

void read_fraction(const std::string& s1, const std::string& s2, const std::string& s3, int num, int den)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << s1 << " is defined as "
	 << num << "/" << den << " of edge " << s2 << " " << s3 << std::endl;  
  }
  if(!den) {
    std::string ss = itoa(den) + " is null : division by zero";
    yyerror((char*) ss.c_str());
  }
  if(steps[current_step].symbol_exists(s1)) {symbol_already_defined(s1); return;}
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v1 = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s3)) {undefined_symbol(s3); return;}
  vertex v2 = steps[current_step].get_current_vertex();

  //- Vertex building
  if(v1 == v2) vertices_equivalent(s1, s2);
  vertex v = v1.fraction(v2,num, den);
  v.set_name(s1);
  steps[current_step].add_symbol(v);
}

void read_caption(const std::string & c)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : caption :"
	      << std::endl << c << std::endl;
  }

  steps[current_step].add_caption(c);
}

void read_index(const std::string & i)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : index : " << i << std::endl;
  }

  steps[current_step].set_index(i);
}

void read_point_to_point(const std::string& r1, const std::string& r2,
			 const std::string& src, const std::string& dst,
			 const std::string& a, const std::string& b, const std::string& c, const std::string& d)
{
  //- define r1 as the instersection of the mediator of [src,dst] and [a,b]
  //- define r2 as the instersection of the mediator of [src,dst] and [c,d]
  if(verbose) {
    std::cout << "Step #" << current_index << " : [" << r1 << "," << r2 << "] is defined as ";
    std::cout << "intersection of mediator [" << src << "," << dst << "] and first [" << a
	 << "," << b << "] and second [" << c << "," << d << "]" << std::endl;
  }
  if(steps[current_step].symbol_exists(r1)) {symbol_already_defined(r1); return;}
  if(steps[current_step].symbol_exists(r2)) {symbol_already_defined(r2); return;}

  if(!steps[current_step].symbol_exists(src)) {undefined_symbol(src); return;}
  vertex vsrc = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(dst)) {undefined_symbol(dst); return;}
  vertex vdst = steps[current_step].get_current_vertex();

  if(!steps[current_step].symbol_exists(a)) {undefined_symbol(a); return;}
  vertex va = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(b)) {undefined_symbol(b); return;}
  vertex vb = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(c)) {undefined_symbol(c); return;}
  vertex vc = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(d)) {undefined_symbol(d); return;}
  vertex vd = steps[current_step].get_current_vertex();

  if(vsrc == vdst) vertices_equivalent(src, dst);
  if(va == vb) vertices_equivalent(a, b);
  if(vsrc.is_orthogonal(vdst, va, vb)) error_edges_orthogonal(src, dst, a, b);
  vertex v1 = vsrc.mediator(vdst, va, vb,
			    current_step ? steps[current_step-1].get_rotate():0,
			    steps[current_step].get_scale());
  if(vc == vd) vertices_equivalent(c, d);
  if(vsrc.is_orthogonal(vdst, vc, vd)) error_edges_orthogonal(src, dst, c, d);
  vertex v2 = vsrc.mediator(vdst, vc, vd,
			    current_step ? steps[current_step-1].get_rotate():0,
			    steps[current_step].get_scale());
  v1.set_name(r1);
  v2.set_name(r2);
  //- new points dx dy computation
  steps[current_step].add_symbol(v1);
  steps[current_step].add_symbol(v2);
}

void read_point_to_line(const std::string& r, const std::string& fs,
			const std::string& mv, const std::string& pvt,
			const std::string& a, const std::string& b,
			const std::string& c, const std::string& d)
{
  //- r is defined as intersection of [c,d] and the line bissector of 
  //- the circle arc defined around pvt from mv to line [a,b]
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << r << " is defined as ";
    std::cout << "intersection of [" << c << "," << d << "] and bissector of " << pvt
	 << "," << mv << " and [" << a << "," << b << "]" << std::endl;
  }
  bool first_second = check_first_second(fs);
  
  if(steps[current_step].symbol_exists(r)) {symbol_already_defined(r); return;}

  if(!steps[current_step].symbol_exists(pvt)) {undefined_symbol(pvt); return;}
  vertex pivot = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(mv)) {undefined_symbol(mv); return;}
  vertex move = steps[current_step].get_current_vertex();

  if(!steps[current_step].symbol_exists(a)) {undefined_symbol(a); return;}
  vertex va = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(b)) {undefined_symbol(b); return;}
  vertex vb = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(c)) {undefined_symbol(c); return;}
  vertex vc = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(d)) {undefined_symbol(d); return;}
  vertex vd = steps[current_step].get_current_vertex();

  vertex v = move.vertex_to_line(pivot, va, vb, vc, vd, first_second,
				 current_step ? steps[current_step-1].get_rotate():0,
				 steps[current_step].get_scale());
  v.set_name(r);
  
  steps[current_step].add_symbol(v);
}

void read_line_to_line(const std::string& r, const std::string& a,
		       const std::string& b, const std::string& c,
		       const std::string& d, const std::string& e)
{
  //- r is defined as the intersection of fold created by
  //- joining [ab] to [ac] (bissector) and [de]
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << r << " is defined as ";
    std::cout << "intersection of fold created by [" << a << "," << b << "] on [" << a << "," << c << "] and [" << d << "," << e << "]" << std::endl;
  }
  if(steps[current_step].symbol_exists(r)) {symbol_already_defined(r); return;}

  if(!steps[current_step].symbol_exists(a)) {undefined_symbol(a); return;}
  vertex va = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(b)) {undefined_symbol(b); return;}
  vertex vb = steps[current_step].get_current_vertex();
  if(va == vb) vertices_equivalent(a, b);
  if(!steps[current_step].symbol_exists(c)) {undefined_symbol(c); return;}
  vertex vc = steps[current_step].get_current_vertex();
  if(va == vc) vertices_equivalent(a, c);
  if(!steps[current_step].symbol_exists(d)) {undefined_symbol(d); return;}
  vertex vd = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(e)) {undefined_symbol(e); return;}
  vertex ve = steps[current_step].get_current_vertex();
  if(vd == ve) vertices_equivalent(d, e);

  if(va == vb) {vertices_equivalent(a, b); return;}
  if(va == vc) {vertices_equivalent(a, c); return;}
  vertex v1 = va.bissector(vb, vc,
			   current_step ? steps[current_step-1].get_rotate():0,
			   steps[current_step].get_scale());
  if(va.is_parallel(v1, vd, ve)) error_edges_parallel(a, b+"_"+c, d, e);
  vertex v = va.intersection(v1,vd,ve,
			     current_step ? steps[current_step-1].get_rotate():0,
			     steps[current_step].get_scale());
  v.set_name(r);
  
  steps[current_step].add_symbol(v);
}
void read_line_to_line(const std::string& r1, const std::string& r2,
		       const std::string& a, const std::string& b,
		       const std::string& c, const std::string& d,
		       const std::string& e, const std::string& f,
		       const std::string& g, const std::string& h)
{
  //- [r1 r2] is defined as the fold created by
  //- joining [ab] to [cd] (bissector or mediator)
  //- r1 : intersection with [ef], r2 : intersection with [gh]
  if(verbose) {
    std::cout << "Step #" << current_index << " : [" << r1 << "," << r2 << " ] is defined as ";
    std::cout << "intersection of fold created by [" << a << "," << b << "] on [" << a << "," << c << "] and [" << d << "," << e << "] (r1) and [" << f << "," << g << "] (r2)" <<std::endl;
  }
  
  if(steps[current_step].symbol_exists(r1)) {symbol_already_defined(r1); return;}
  if(steps[current_step].symbol_exists(r2)) {symbol_already_defined(r2); return;}

  if(!steps[current_step].symbol_exists(a)) {undefined_symbol(a); return;}
  vertex va = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(b)) {undefined_symbol(b); return;}
  vertex vb = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(c)) {undefined_symbol(c); return;}
  vertex vc = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(d)) {undefined_symbol(d); return;}
  vertex vd = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(e)) {undefined_symbol(e); return;}
  vertex ve = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(f)) {undefined_symbol(f); return;}
  vertex vf = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(g)) {undefined_symbol(g); return;}
  vertex vg = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(h)) {undefined_symbol(h); return;}
  vertex vh = steps[current_step].get_current_vertex();

  vertex v1,v2;
  if(va.is_parallel(vb,vc,vd)){
    if(vc == vd) vertices_equivalent(c,d);
    vertex pa=va.projection(vc,vd,
			    current_step ? steps[current_step-1].get_rotate():0,
			    steps[current_step].get_scale());
    if(va.is_orthogonal(pa,ve,vf)) error_edges_orthogonal(a,"proj_"+c+"_"+d, e,f);
    if(ve == vf) vertices_equivalent(e, f);
    v1=va.mediator(pa,ve,vf,
		   current_step ? steps[current_step-1].get_rotate():0,
		   steps[current_step].get_scale());
    if(va.is_orthogonal(pa,vg,vh)) error_edges_orthogonal(a,"proj_"+c+"_"+d, g,h);
    if(vg == vh) vertices_equivalent(g, h);
    v2=va.mediator(pa,vg,vh,
		   current_step ? steps[current_step-1].get_rotate():0,
		   steps[current_step].get_scale());      
  }
  else {
    vertex in=va.intersection(vb,vc,vd,
			      current_step ? steps[current_step-1].get_rotate():0,
			      steps[current_step].get_scale());
    vertex va1,vc1;
    va1 = in==va ? vb : va;
    vc1 = in==vc ? vd : vc;
    vertex v0=in.bissector(va1,vc1,
			   current_step ? steps[current_step-1].get_rotate():0,
			   steps[current_step].get_scale());
    v1=in.intersection(v0,ve,vf,
		       current_step ? steps[current_step-1].get_rotate():0,
		       steps[current_step].get_scale());
    v2=in.intersection(v0,vg,vh,
		       current_step ? steps[current_step-1].get_rotate():0,
		       steps[current_step].get_scale());
  }
  v1.set_name(r1);
  v2.set_name(r2);
  
  steps[current_step].add_symbol(v1);
  steps[current_step].add_symbol(v2);
}

void read_simple_arrow(const std::string& s1, const std::string& s2, const std::string& v1type,
		const std::string& v2type, const std::string& left_right, int angle)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : a simple arrow is defined from ";
    std::cout << s1 << " to " << s2 << std::endl;
  }

  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}

  arrow_type v1t = check_arrow_type(v1type);
  arrow_type v2t = check_arrow_type(v2type);
  arrow_side side = check_arrow_side(left_right);
  //- add a new arrow ...
  steps[current_step].add_arrow(arrow(s1, s2, v1t, v2t, side, true, angle));
}

void read_simple_arrow(const std::string& s, const std::string& s1, const std::string& s2,
		const std::string& v1type, const std::string& v2type,
		const std::string& left_right, int angle)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : a simple arrow is defined from ";
    std::cout << s << " across [" << s1 << ", " << s2 << "]" << std::endl;
  }
  if(!steps[current_step].symbol_exists(s)) {undefined_symbol(s); return;}
  vertex v = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1 = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2 = steps[current_step].get_current_vertex();

  arrow_type v1t = check_arrow_type(v1type);
  arrow_type v2t = check_arrow_type(v2type);
  arrow_side side = check_arrow_side(left_right);
  //- add a new arrow ...
  vertex r=v.symmetry(v1, v2,
		      current_step ? steps[current_step-1].get_rotate():0,
		      steps[current_step].get_scale());
  r.set_name(gen_sym(v.get_name()));
  steps[current_step].add_symbol(r);
  
  steps[current_step].add_arrow(arrow(s,r.get_name(),v1t,v2t,side,true,angle));
}

void read_return_arrow(const std::string& s1, const std::string& s2, const std::string& s3, const std::string& s4,
		const std::string& v1type, const std::string& v2type,
		const std::string& left_right, int ratio)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : a return arrow is defined from fold line [";
    std::cout << s1 << ", " << s2 << "] up to line [" << s3 << ", " << s4 << "]" << std::endl;
  }
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1 = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2 = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s3)) {undefined_symbol(s3); return;}
  vertex v3 = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s4)) {undefined_symbol(s4); return;}
  vertex v4 = steps[current_step].get_current_vertex();
  if(v1 == v2) vertices_equivalent(s1, s2);
  if(v3 == v4) vertices_equivalent(s3, s4);
  //- TODO :check 4 points aren't aligned

  vertex mid12 = v1.middle(v2);
  vertex end = v1.mediator(v2,v3,v4,
			   current_step ? steps[current_step-1].get_rotate():0,
			   steps[current_step].get_scale());
  vertex begin = mid12.middle(end);
  arrow_type v1t = check_arrow_type(v1type);
  arrow_type v2t = check_arrow_type(v2type);
  arrow_side side = check_arrow_side(left_right);
  //- add a new arrow ...
  begin.set_name(gen_sym("bra"));
  end.set_name(gen_sym("era"));
  steps[current_step].add_symbol(begin);
  steps[current_step].add_symbol(end);
  
  steps[current_step].add_arrow(arrow(begin.get_name(),end.get_name(),v1t,v2t,side,false,ratio));
}

void read_turn_over(bool vertical)
{
  if(verbose) {
    std::cout << "Turn over ";
    if(vertical) std::cout << "vertically after step #" << current_index-1 << std::endl;
    else std::cout << "horizontally after step #" << current_index-1 << std::endl;
  }
  if(vertical) steps[current_step-1].set_turn(turnVertical);
  else steps[current_step-1].set_turn(turnHorizontal);
}

void read_rotate(double a)
{
  if(a > 180 || a < -180) yyerror("Wrong angle. Should be degrees (-180, 180).");
  if(verbose) {
    std::cout << "Rotate all internal data after step #" << current_index-1
	 << " of " << a << " degrees" << std::endl;
  }
  if(current_step) steps[current_step-1].add_rotate(a);
}

void read_hide_show(const std::string& s1, const std::string& s2, bool visible)
{
  if(verbose) {
    std::cout << "Step #" << current_index;
    if(visible) std::cout << " : show edge [";
    else std::cout << " : hide edge [";
    std::cout << s1 << ", " << s2 << "]" << std::endl;
  }
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  

  int nlines = steps[current_step].get_lines_number();
  bool found = false;

  for(int i=0;i<nlines;i++) {
    if(steps[current_step].get_line(i).equal(s1, s2)) {
      if(visible) steps[current_step].ref_line(i).show();
      else steps[current_step].ref_line(i).hide();
      found = true;
    }
  }
  if(!found) {
    std::string ss = "["+s1+", "+s2+"] hasn't been yet defined";
    yyerror((char *)ss.c_str());
  }
}

void read_hide_show(const std::vector<std::string>& vertices, bool visible)
{
  if(verbose) {
    std::cout << "Step #" << current_index;
    if(visible) std::cout << " : show all edges containing : {";
    else std::cout << " : hide all edges containing : {";
    std::cout << vertices[0];
    int nver = vertices.size();
    for(int i=1;i<nver;i++) std::cout << ", " << vertices[i];
    std::cout << "}" << std::endl;
  }

  int nv = vertices.size();
  int nlines = steps[current_step].get_lines_number();

  for(int i=0;i<nv;i++) {
    if(!steps[current_step].symbol_exists(vertices[i]))
      {undefined_symbol(vertices[i]); return;}
    else {
      bool found = false;
      for(int l=0;l<nlines;l++) {
	if(steps[current_step].get_line(l).get_v1() == vertices[i] ||
	   steps[current_step].get_line(l).get_v2() == vertices[i]) {
	  if(visible) steps[current_step].ref_line(l).show();
	  else steps[current_step].ref_line(l).hide();
	  found = true;
	}
      }
      if(!found) {
	std::string ss = "None edge containing " + vertices[i] + " previously defined";
	yyerror((char *)ss.c_str());
      }
    }
  }
}

void read_intersection(const std::string& r, const std::string& s1, const std::string& s2,
		       const std::string& s3, const std::string& s4)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << r
	 << " is defined as intersection of [" << s1 << ", " << s2 << "] and ["
	 << s3 << ", " << s4 << "]" << std::endl;
  }
  if(steps[current_step].symbol_exists(r)) {symbol_already_defined(r); return;}
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1(steps[current_step].get_current_vertex());
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2(steps[current_step].get_current_vertex());
  if(v1 == v2) vertices_equivalent(s1, s2);
  if(!steps[current_step].symbol_exists(s3)) {undefined_symbol(s3); return;}
  vertex v3(steps[current_step].get_current_vertex());
  if(!steps[current_step].symbol_exists(s4)) {undefined_symbol(s4); return;}
  vertex v4(steps[current_step].get_current_vertex());
  if(v3 == v4) vertices_equivalent(s3, s4);

  if(v1.is_parallel(v2,v3,v4)) yyerror("Intersection doesn't exist (parallel)");
  vertex v = v1.intersection(v2,v3,v4,
			     current_step ? steps[current_step-1].get_rotate():0,
			     steps[current_step].get_scale());
  v.set_name(r);

  //- new point dx dy computation
  // FIXME
  steps[current_step].add_symbol(v);
}

void read_symmetry(const std::string& r, const std::string& s,
		  const std::string& s1, const std::string& s2)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << r
	 << " is defined as the projected of " << s
	 << " through [" << s1 << ", " << s2 << "]" << std::endl;
  }

  if(steps[current_step].symbol_exists(r)) {symbol_already_defined(r); return;}
  if(!steps[current_step].symbol_exists(s)) {undefined_symbol(s); return;}
  vertex v(steps[current_step].get_current_vertex());
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1(steps[current_step].get_current_vertex());
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2(steps[current_step].get_current_vertex());

  //- Is [v1, v2] a really edge ?
  if(v1 == v2) vertices_equivalent(s1, s2);
  else {
    vertex vr = v.symmetry(v1, v2,
			   current_step ? steps[current_step-1].get_rotate():0,
			   steps[current_step].get_scale());
    vr.set_name(r);

    steps[current_step].add_symbol(vr);
  }
}

void cut_edge(const vertex& v, const vertex& v1, const vertex& v2, const std::string& s, const std::string& s1, const std::string& s2)
{
  //- existing edge is modified ...
  bool found = false;
  int nlines = steps[current_step].get_lines_number();
  int sp;
  edge_type t;
  std::string sv2;
  double norm = vect(v1, v2).norm();
  double norm1= vect(v1, v).norm();
  double norm2= vect(v, v2).norm();
  
  for(int l=0;l<nlines;l++) {
    edge& e = steps[current_step].ref_line(l);
    if(e.equal(s1, s2)) {
      if(e.get_v1() == s1) sv2 = s2;
      else { //- reverse case ... norm1 <=> norm2
	sv2 = s1;
	double d = norm2;
	norm2 = norm1;
	norm1 = d;
      }
      sp = e.get_space_v2(); //- save the other space value
      e.set_v2(s);           //- set edge extremity to cut point
      e.set_space_v2(0);     //- no space left near the cut point
      if(e.get_space_v1()*norm/norm1 > 100) {
	std::string ss = "Blank space from "+e.get_v1()+" is greater than edge length, ["+e.get_v1()+", "+e.get_v2()+"] won't be visible";
	yywarning((char *)ss.c_str());
      }
      e.set_space_v1(e.get_space_v1()*norm/norm1);//- new percent same view
      t = e.get_type();        //- save the type of original edge
      found = true;
      break;
    }
  }
  if(!found) {undefined_edge(s1, s2); return;}
  //- ... and one adds now the second edge part
  if(sp*norm/norm2 > 100) {
    std::string ss = "Blank space from "+sv2+" is greater than edge length, ["+s+", "+sv2+"] won't be visible";
    yywarning((char *)ss.c_str());
  }
  steps[current_step].add_line(edge(s,sv2,t,allPercent, 0, sp*norm/norm2));
}
  

void read_cut(const std::string& s1, const std::string& s2, const std::string& s)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : cut edge ["
	 << s1 << ", " << s2 << "] into [" << s1 << ", " << s << "] and ["
	 << s << ", " << s2 << "]" << std::endl;
  }

  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s)) {undefined_symbol(s); return;}
  vertex v=steps[current_step].get_current_vertex();

  cut_edge(v,v1,v2,s,s1,s2);
}

void read_debug_point()
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : show all points name" << std::endl;
  }
  for(int i = 0; i < steps[current_step].get_symbols_number(); i++)
    steps[current_step].ref_vertex(i).set_debug(true);
}

void read_debug_point(const std::vector<std::string>& vertices)
{
  unsigned int nver = vertices.size();
  
  if(verbose) {
    std::cout << "Step #" << current_index << " : show explicit points name (" << std::endl;
    std::cout << vertices[0];
    for(unsigned int i=1;i<nver;i++) std::cout << ", " << vertices[i];
    std::cout << ")" << std::endl;
  }
  for(unsigned int i = 0; i < nver; i++) {
    if(!steps[current_step].symbol_exists(vertices[i])) {undefined_symbol(vertices[i]); return;}
    steps[current_step].ref_current_vertex().set_debug(true);
  }
}

void read_debug_line()
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : show all lines name" << std::endl;
  }
  for(int i = 0; i<steps[current_step].get_lines_number(); i++){
    steps[current_step].ref_line(i).set_debug(true);
  }
}

void read_debug_line(const std::string& s1, const std::string& s2)
{
  if(verbose) {
    std::cout << "Step #" << current_index;
    std::cout << " : show edge [";
    std::cout << s1 << ", " << s2 << "] name" << std::endl;
  }
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  

  int nlines = steps[current_step].get_lines_number();
  bool found = false;

  for(int i=0;i<nlines;i++) {
    if(steps[current_step].get_line(i).equal(s1, s2)) {
      steps[current_step].ref_line(i).set_debug(true);
      found = true;
    }
  }
  if(!found) {
    std::string ss = "["+s1+", "+s2+"] hasn't been yet defined";
    yyerror((char *)ss.c_str());
  }
}

void read_debug_line(const std::vector<std::string>& vertices)
{
  if(verbose) {
    std::cout << "Step #" << current_index;
    std::cout << " : show all edges name containing one of those vertices (";
    std::cout << vertices[0];
    int nver = vertices.size();
    for(int i=1;i<nver;i++) std::cout << ", " << vertices[i];
    std::cout << ")" << std::endl;
  }

  int nv = vertices.size();
  int nlines = steps[current_step].get_lines_number();

  for(int i=0;i<nv;i++) {
    if(!steps[current_step].symbol_exists(vertices[i]))
      {undefined_symbol(vertices[i]); return;}
    else {
      bool found = false;
      for(int l=0;l<nlines;l++) {
	if(steps[current_step].get_line(l).get_v1() == vertices[i] ||
	   steps[current_step].get_line(l).get_v2() == vertices[i]) {
	  steps[current_step].ref_line(l).set_debug(true);
	  found = true;
	}
      }
      if(!found) {
	std::string ss = "None edge containing " + vertices[i] + " previously defined";
	yyerror((char *)ss.c_str());
      }
    }
  }
}

void read_visible_height(int h)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : visible height = " << h << std::endl;
  }

  if(h < 0) yyerror("Visible height of a step can't be negative.");
  steps[current_step].set_clip_height(h);
}

void read_visible_width(int w)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : visible width = " << w << std::endl;
  }

  if(w < 0) yyerror("Visible width of a step can't be negative.");
  steps[current_step].set_clip_width(w);
}

void read_visible_center(const std::string& s)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << s
	 << " is the center of the visible area for the current step only" << std::endl;
  }

  if(!steps[current_step].symbol_exists(s)) {undefined_symbol(s); return;}
  vertex v=steps[current_step].get_current_vertex();

  steps[current_step].set_visible_center_x(v.get_x());
  steps[current_step].set_visible_center_y(v.get_y());
}

void read_visible_center_inter(const std::string& s)
{
  if(verbose) {
    std::cout << "Before step #" << current_index << " : " << s
	 << " is the center of the visible area for the next steps" << std::endl;
  }
  //- you should find the symbol in the previous step
  if(!steps[current_step-1].symbol_exists(s)) {undefined_symbol(s); return;}
  center = s;
}

void read_scale(int s)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : scaling this step of " << s
	 << "%" << std::endl;
  }
  if(s < 1) yyerror("Wrong scaling factor, should be greater than 0");
  steps[current_step].set_scale(s);
}

color * read_color(const std::string& sym)
{
  return new color(front_or_back(sym));
}

void read_fill(const color& c, const std::vector<std::string>& vertices)
{
  if(vertices.size() < 3) yyerror("Fill should almost use 3 vertices name");
  
  if(verbose) {
    std::cout << "Step #" << current_index << " : filling area defined by {";
    std::cout << vertices[0];
    int nver = vertices.size();
    for(int i=1;i<nver;i++) std::cout << ", " << vertices[i];
    std::cout << "} with RGB color = " << c.to_string() << std::endl;
  }
  face f(c);
  int nver = vertices.size();

  for(int i=0;i<nver;i++){
    if(!steps[current_step].symbol_exists(vertices[i])) {undefined_symbol(vertices[i]); return;}
    f.add_symbol(vertices[i]);
  }
  steps[current_step].add_face(f);
}

void read_unfill(const std::vector<std::string>& vertices)
{
  if(vertices.size() < 3) yyerror("Unfill should almost use 3 vertices name");
  
  if(verbose) {
    std::cout << "Step #" << current_index << " : unfilling area defined by {";
    std::cout << vertices[0];
    int nver = vertices.size();
    for(int i=1;i<nver;i++) std::cout << ", " << vertices[i];
    std::cout << "}" << std::endl;
  }
  int nfaces = steps[current_step].get_faces_number();
  int nver = vertices.size();
  int i=0;
  bool found = false;
  while(!found && i<nfaces) {
    //- get all symbols of his face
    std::vector<std::string> symb_face(steps[current_step].get_face(i).get_symbols()); 
    bool ok = true;
    for(int v=0;v<nver && ok;v++){
      std::string symb = vertices[v];
      if(!steps[current_step].symbol_exists(symb)) {undefined_symbol(symb); return;}
      std::vector<std::string>::iterator result = std::find(symb_face.begin(), symb_face.end(), symb);
      ok = result != symb_face.end(); 
    }
    if(ok) found = true;
    else i++;
  }
  if(found) {
    //- remove face i
    steps[current_step].delete_face(i);
  } else yyerror("Area hasn't be filled before or already unfilled");
}

void read_unfill_all()
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : unfilling all areas previously defined" << std::endl;
  }
  steps[current_step].delete_all_faces();
}

void read_vertical_rectangle(const std::string& s1, const std::string& s2, const std::string& s3,
		       const std::string& s4, const std::string& r)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : a vertical rectangle " << s1 
	 << " " << s2 << " " << s3 << " " << s4 << " is defined" << std::endl;
  }
  //- get ratio from std::string (name of paper format : A, dollar ...)
  int ratio = check_paper_format(r);
  
  //- Define the symbol $3, $5, $7, $9 and edge $3-$5, $5-$7, $7-$9, $9-$3 as border 
  if(steps[current_step].symbol_exists(s1)) {symbol_already_defined(s1); return;}
  if(steps[current_step].symbol_exists(s2)) {symbol_already_defined(s2); return;}
  if(steps[current_step].symbol_exists(s3)) {symbol_already_defined(s3); return;}
  if(steps[current_step].symbol_exists(s4)) {symbol_already_defined(s4); return;}
  
  vertex v1(s1), v2(s2), v3(s3), v4(s4);
  if(ratio < 100) {
    v1.set_x_y(-ratio*(SQUARE_EDGE/100.0),  SQUARE_EDGE);
    v2.set_x_y( ratio*(SQUARE_EDGE/100.0),  SQUARE_EDGE);
    v3.set_x_y( ratio*(SQUARE_EDGE/100.0), -SQUARE_EDGE);
    v4.set_x_y(-ratio*(SQUARE_EDGE/100.0), -SQUARE_EDGE);
  } else {
    v1.set_x_y(-SQUARE_EDGE,  ratio*(SQUARE_EDGE/100.0));
    v2.set_x_y( SQUARE_EDGE,  ratio*(SQUARE_EDGE/100.0));
    v3.set_x_y( SQUARE_EDGE, -ratio*(SQUARE_EDGE/100.0));
    v4.set_x_y(-SQUARE_EDGE, -ratio*(SQUARE_EDGE/100.0));
  }

  steps[current_step].add_symbol(v1);
  steps[current_step].add_symbol(v2);
  steps[current_step].add_symbol(v3);
  steps[current_step].add_symbol(v4);
  
  steps[current_step].add_line(edge(s1, s2));
  steps[current_step].add_line(edge(s2, s3));
  steps[current_step].add_line(edge(s3, s4));
  steps[current_step].add_line(edge(s4, s1));
}

void read_vertical_rectangle(const std::string& s1, const std::string& s2, const std::string& s3,
		       const std::string& s4, int ratio)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : a vertical rectangle " << s1 
	 << " " << s2 << " " << s3 << " " << s4 << " is defined" << std::endl;
  }
  //- Define the symbol $3, $5, $7, $9 and edge $3-$5, $5-$7, $7-$9, $9-$3 as border 
  if(steps[current_step].symbol_exists(s1)) {symbol_already_defined(s1); return;}
  if(steps[current_step].symbol_exists(s2)) {symbol_already_defined(s2); return;}
  if(steps[current_step].symbol_exists(s3)) {symbol_already_defined(s3); return;}
  if(steps[current_step].symbol_exists(s4)) {symbol_already_defined(s4); return;}
  
  vertex v1(s1), v2(s2), v3(s3), v4(s4);
  if(ratio < 100) {
    v1.set_x_y(-ratio*(SQUARE_EDGE/100.0),  SQUARE_EDGE);
    v2.set_x_y( ratio*(SQUARE_EDGE/100.0),  SQUARE_EDGE);
    v3.set_x_y( ratio*(SQUARE_EDGE/100.0), -SQUARE_EDGE);
    v4.set_x_y(-ratio*(SQUARE_EDGE/100.0), -SQUARE_EDGE);
  }else {
    v1.set_x_y(-SQUARE_EDGE,  ratio*(SQUARE_EDGE/100.0));
    v2.set_x_y( SQUARE_EDGE,  ratio*(SQUARE_EDGE/100.0));
    v3.set_x_y( SQUARE_EDGE, -ratio*(SQUARE_EDGE/100.0));
    v4.set_x_y(-SQUARE_EDGE, -ratio*(SQUARE_EDGE/100.0));
  }

  steps[current_step].add_symbol(v1);
  steps[current_step].add_symbol(v2);
  steps[current_step].add_symbol(v3);
  steps[current_step].add_symbol(v4);
  
  steps[current_step].add_line(edge(s1, s2));
  steps[current_step].add_line(edge(s2, s3));
  steps[current_step].add_line(edge(s3, s4));
  steps[current_step].add_line(edge(s4, s1));
}
void read_horizontal_rectangle(const std::string& s1, const std::string& s2, const std::string& s3,
			       const std::string& s4, const std::string& r)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : a horizontal rectangle " << s1 
	 << " " << s2 << " " << s3 << " " << s4 << " is defined" << std::endl;
  }
  //- get ratio from std::string (name of paper format : A, dollar ...)
  int ratio = check_paper_format(r);
  
  //- Define the symbol $3, $5, $7, $9 and edge $3-$5, $5-$7, $7-$9, $9-$3 as border 
  if(steps[current_step].symbol_exists(s1)) {symbol_already_defined(s1); return;}
  if(steps[current_step].symbol_exists(s2)) {symbol_already_defined(s2); return;}
  if(steps[current_step].symbol_exists(s3)) {symbol_already_defined(s3); return;}
  if(steps[current_step].symbol_exists(s4)) {symbol_already_defined(s4); return;}
  
  vertex v1(s1), v2(s2), v3(s3), v4(s4);
  if(ratio > 100) {
    v1.set_x_y(-ratio*(SQUARE_EDGE/100.0),  SQUARE_EDGE);
    v2.set_x_y( ratio*(SQUARE_EDGE/100.0),  SQUARE_EDGE);
    v3.set_x_y( ratio*(SQUARE_EDGE/100.0), -SQUARE_EDGE);
    v4.set_x_y(-ratio*(SQUARE_EDGE/100.0), -SQUARE_EDGE);
  }else {
    v1.set_x_y(-SQUARE_EDGE,  ratio*(SQUARE_EDGE/100.0));
    v2.set_x_y( SQUARE_EDGE,  ratio*(SQUARE_EDGE/100.0));
    v3.set_x_y( SQUARE_EDGE, -ratio*(SQUARE_EDGE/100.0));
    v4.set_x_y(-SQUARE_EDGE, -ratio*(SQUARE_EDGE/100.0));
  }

  steps[current_step].add_symbol(v1);
  steps[current_step].add_symbol(v2);
  steps[current_step].add_symbol(v3);
  steps[current_step].add_symbol(v4);
  
  steps[current_step].add_line(edge(s1, s2));
  steps[current_step].add_line(edge(s2, s3));
  steps[current_step].add_line(edge(s3, s4));
  steps[current_step].add_line(edge(s4, s1));
}

void read_horizontal_rectangle(const std::string& s1, const std::string& s2, const std::string& s3,
		       const std::string& s4, int ratio)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : a horizontal rectangle " << s1 
	 << " " << s2 << " " << s3 << " " << s4 << " is defined" << std::endl;
  }
  //- Define the symbol $3, $5, $7, $9 and edge $3-$5, $5-$7, $7-$9, $9-$3 as border 
  if(steps[current_step].symbol_exists(s1)) {symbol_already_defined(s1); return;}
  if(steps[current_step].symbol_exists(s2)) {symbol_already_defined(s2); return;}
  if(steps[current_step].symbol_exists(s3)) {symbol_already_defined(s3); return;}
  if(steps[current_step].symbol_exists(s4)) {symbol_already_defined(s4); return;}
  
  vertex v1(s1), v2(s2), v3(s3), v4(s4);
  if(ratio > 100) {
    v1.set_x_y(-ratio*(SQUARE_EDGE/100.0),  SQUARE_EDGE);
    v2.set_x_y( ratio*(SQUARE_EDGE/100.0),  SQUARE_EDGE);
    v3.set_x_y( ratio*(SQUARE_EDGE/100.0), -SQUARE_EDGE);
    v4.set_x_y(-ratio*(SQUARE_EDGE/100.0), -SQUARE_EDGE);
  }else {
    v1.set_x_y(-SQUARE_EDGE,  ratio*(SQUARE_EDGE/100.0));
    v2.set_x_y( SQUARE_EDGE,  ratio*(SQUARE_EDGE/100.0));
    v3.set_x_y( SQUARE_EDGE, -ratio*(SQUARE_EDGE/100.0));
    v4.set_x_y(-SQUARE_EDGE, -ratio*(SQUARE_EDGE/100.0));
  }

  steps[current_step].add_symbol(v1);
  steps[current_step].add_symbol(v2);
  steps[current_step].add_symbol(v3);
  steps[current_step].add_symbol(v4);
  
  steps[current_step].add_line(edge(s1, s2));
  steps[current_step].add_line(edge(s2, s3));
  steps[current_step].add_line(edge(s3, s4));
  steps[current_step].add_line(edge(s4, s1));
}

void read_clip(bool c)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : local clipping state : ";
    if(c) std::cout << "ON" << std::endl;
    else std::cout << "OFF" << std::endl;
  }
  steps[current_step].set_clip(c);
}

void read_clip(bool c, int w, int h)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : local clipping state : ";
    if(c) std::cout << "ON" << std::endl;
    else std::cout << "OFF" << std::endl;
  }
  steps[current_step].set_clip_width(w);
  steps[current_step].set_clip_height(h);
  steps[current_step].set_clip(c);
}

void read_perpendicular(const std::string& sr, const std::string& s1, const std::string& s2,
			const std::string& so)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << sr << " is defined such as [" 
	 << sr << ", " << so << "] is perpendicular to [" << s1 << ", " << s2 << "]"
	 << std::endl;
  }
  if(steps[current_step].symbol_exists(sr)) {symbol_already_defined(sr); return;}
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(so)) {undefined_symbol(so); return;}
  vertex vo=steps[current_step].get_current_vertex();

  if(v1 == v2) vertices_equivalent(s1, s2);
  vertex vr = vo.projection(v1, v2,
			    current_step ? steps[current_step-1].get_rotate():0,
			    steps[current_step].get_scale());
  vr.set_name(sr);
  steps[current_step].add_symbol(vr);
}

void read_perpendicular(const std::string& sr, const std::string& s1, const std::string& s2,
			const std::string& so, const std::string& s3, const std::string& s4)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << sr << " is defined such as [" 
	 << sr << ", " << so << "] is perpendicular to [" << s1 << ", " << s2
	 << "], " << sr << " belongs to [" << s3 << ", " << s4 << "]" << std::endl;
  }
  if(steps[current_step].symbol_exists(sr)) {symbol_already_defined(sr); return;}
  if(!steps[current_step].symbol_exists(so)) {undefined_symbol(so); return;}
  vertex vo=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s3)) {undefined_symbol(s3); return;}
  vertex v3=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s4)) {undefined_symbol(s4); return;}
  vertex v4=steps[current_step].get_current_vertex();

  if(v1 == v2) vertices_equivalent(s1, s2);
  if(v3 == v4) vertices_equivalent(s3, s4);
  if(v1.is_orthogonal(v2, v3, v4)) error_edges_orthogonal(s1, s2, s3, s4);
  
  vertex p = vo.projection(v1, v2,
			   current_step ? steps[current_step-1].get_rotate():0,
			   steps[current_step].get_scale());
  vertex vr;
  if(p == vo) {
    vect u = vect(v1, v2).ortho();
    vertex auxi = vo+u;
    vr = p.intersection(auxi, v3, v4,
			current_step ? steps[current_step-1].get_rotate():0,
			steps[current_step].get_scale());
  } else vr = p.intersection(vo, v3, v4,
			     current_step ? steps[current_step-1].get_rotate():0,
			     steps[current_step].get_scale());
  vr.set_name(sr);
  steps[current_step].add_symbol(vr);
}

void read_parallel(const std::string& sr, const std::string& s1, const std::string& s2,
		   const std::string& so, const std::string& s3, const std::string& s4)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << sr << " is defined such as [" 
	 << sr << ", " << so << "] parallel to [" << s1 << ", " << s2 << "], "
	 << sr << " belongs to [" << s3 << ", " << s4 << "]" << std::endl;
  }
  if(steps[current_step].symbol_exists(sr)) {symbol_already_defined(sr); return;}
  if(!steps[current_step].symbol_exists(so)) {undefined_symbol(so); return;}
  vertex vo=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s3)) {undefined_symbol(s3); return;}
  vertex v3=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s4)) {undefined_symbol(s4); return;}
  vertex v4=steps[current_step].get_current_vertex();

  if(v1 == v2) vertices_equivalent(s1, s2);
  if(v3 == v4) vertices_equivalent(s3, s4);
  if(v1.is_parallel(v2, v3, v4)) error_edges_parallel(s1, s2, s3, s4);

  vertex p = vo + vect(v1, v2);
  vertex vr = p.intersection(vo, v3, v4,
			     current_step ? steps[current_step-1].get_rotate():0,
			     steps[current_step].get_scale());
  vr.set_name(sr);
  steps[current_step].add_symbol(vr);
}

void read_move(const std::string& src, const std::string& dest) {
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << src << " moves to meet " 
	 << dest << std::endl;
  }
  if(!steps[current_step].symbol_exists(src)) {undefined_symbol(src); return;}
  vertex& vsrc=steps[current_step].ref_current_vertex();
  if(!steps[current_step].symbol_exists(dest)) {undefined_symbol(dest); return;}
  vertex vdest=steps[current_step].get_current_vertex();

  //- change internal data of vsrc to meet vdest
  vsrc.set_x_y(vdest.get_x(), vdest.get_y());
  //- reset possible dx dy on moving point
  vsrc.set_dx_dy(0,0);
}

void read_move(const std::string& src, const std::string& s1, const std::string& s2) {
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << src
	 << " moves symmetrically through [" << s1 << ", "
	 << s2 << "]" << std::endl;
  }
  if(!steps[current_step].symbol_exists(src)) {undefined_symbol(src); return;}
  vertex& vsrc=steps[current_step].ref_current_vertex();
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2=steps[current_step].get_current_vertex();

  //- change internal data of vsrc to meet its symmetry by [s1, s2]
  if(v1 == v2) vertices_equivalent(s1, s2);
  vertex sym = vsrc.symmetry(v1, v2,
			     current_step ? steps[current_step-1].get_rotate():0,
			     steps[current_step].get_scale());
  vsrc.set_x_y(sym.get_x(), sym.get_y());
  //- reset possible dx dy on moving point
  vsrc.set_dx_dy(0,0);
}

void read_inter_cut(const std::string& s, const std::string& s1, const std::string& s2,
		    const std::string& s3, const std::string& s4)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << s
	 << " is defined as the intersection of [" << s1 << ", "
	 << s2 << "] and [" << s3 << ", " << s4 << "], [" << s1 << ", "
	 << s2 << "] is cut at this point" << std::endl;
  }
  
  if(steps[current_step].symbol_exists(s)) {symbol_already_defined(s); return;}
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s3)) {undefined_symbol(s3); return;}
  vertex v3=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s4)) {undefined_symbol(s4); return;}
  vertex v4=steps[current_step].get_current_vertex();
  if(v1.is_parallel(v2,v3,v4)) yyerror("Intersection doesn't exist (parallel edges)");

  //- adding intersection vertex
  vertex v = v1.intersection(v2,v3,v4,
			     current_step ? steps[current_step-1].get_rotate():0,
			     steps[current_step].get_scale());
  v.set_name(s);
  steps[current_step].add_symbol(v);
  //- \cut([s1, s2], s);
  cut_edge(v,v1,v2,s, s1,s2);
}

void read_space_fold(const std::string& s1, const std::string& s2, int sp1, int sp2)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << " spaces of ["
	 << s1 << ", " << s2 << "] are respectively set to "
	 << sp1 << "% and " <<  sp2 << "%" << std::endl;
  }

  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1=steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2=steps[current_step].get_current_vertex();

  bool found = false;
  int nlines = steps[current_step].get_lines_number();
  for(int l=0;l<nlines;l++) {
    edge& e = steps[current_step].ref_line(l);
    if(e.equal(s1, s2)) {
      if(e.get_v1() == s1) { //- normal case
	e.set_space_v1(sp1);
	e.set_space_v2(sp2);
      } else { //- reverse case
	e.set_space_v1(sp2);
	e.set_space_v2(sp1);
      }
      found = true;
      break;
    }
  }
  if(!found)  {
    std::string ss = "edge ["+s1+", "+s2+"] hasn't been yet defined";
    yyerror((char *)ss.c_str());
  }
}


//- Hack to produce eps manual figure from doodle output
#include <fstream>
extern void write_ps_step(std::ostream& out, step& s, bool);

void read_eps(const std::string& eps_fn, int l, int b, int r, int t)
{
  std::ofstream out(eps_fn.c_str());

  if(verbose) {
    std::cout << "Step #" << current_index << " : " << "make a EPS file ( "
	 << eps_fn << ") corresponding to the current step." << std::endl;
  }
  out << "%!PS-Adobe-2.0 EPSF-1.2\n%%Creator: Doodle -- http://doodle.sf.net\n%%Title: "
      << eps_fn  << std::endl
      << "%%BoundingBox: 0 0 " << r-l << " " << t-b << "\n"
      << "%%EndComments\n\n"
      << " gsave " << -l << " " << -b << " translate\n";
  ps_prologue(out);
  out << "105 mm 200 mm translate\n";
  write_ps_step(out, steps[current_step], false);
  out << "grestore\n";
}

void read_reset() 
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : "
	 << "all previously defined data (vertices and edges) are reseted." << std::endl;
  }
  steps[current_step].reset_data();
}

void read_fold_percent(const std::string& s1, const std::string& s2,
		       edge_type t, int sp1, int sp2)
{
  int lim = -1;
  if(verbose) {
    std::cout << "Step #" << current_index << " : a ";
    switch (t) {
     case edgeValley : std::cout << "valley"; break;
     case edgeMountain : std::cout << "mountain"; break;
     case edgeFold : std::cout << "existent"; break;
     case edgeBorder : std::cout << "border"; break;
     case edgeXray : std::cout << "Xray"; break;
     default : std::cerr << "Wrong edge type in read_fold_percent" << std::endl;
    }
    std::cout << " fold is defined between " << s1 << " and " << s2 << std::endl;
  }

  if(sp1+sp2 > 100) {
    yyerror("Sum of blank spaces given > 100%!");
  }
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1 = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2 = steps[current_step].get_current_vertex();
  if(v1 == v2) vertices_equivalent(s1, s2);

  lim = search_edge_and_hide_equivalent(s1, s2);
  if(lim != -1) {
    edge& e = steps[current_step].ref_line(lim);
    e.set_type(t);
    if(e.get_v1() == s1) e.set_space_v1(sp1);
    else e.set_space_v2(sp1);
    if(e.get_v2() == s2) e.set_space_v2(sp2);
    else e.set_space_v1(sp2);
    e.show();
  } else {
    steps[current_step].add_line(edge(s1, s2, t, allPercent, sp1, sp2));
  }
}

void read_fold_limit(const std::string& s1, const std::string& s2, edge_type t,
		     const std::string& e11, const std::string& e12,
		     const std::string& e21, const std::string& e22)
{
  int eIdx = -1, lim1 =-1, lim2=-1;
  if(verbose) {
    std::cout << "Step #" << current_index << " : a ";
    switch (t) {
     case edgeValley : std::cout << "valley"; break;
     case edgeMountain : std::cout << "mountain"; break;
     case edgeFold : std::cout << "existent"; break;
     case edgeBorder : std::cout << "border"; break;
     case edgeXray : std::cout << "Xray"; break;
     default : yyerror("Wrong edge type in read_fold_limit");
    }
    std::cout << " fold is defined between " << s1 << " and " << s2 << std::endl;
  }
  //- check all input edges ...
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1 = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2 = steps[current_step].get_current_vertex();
  if(v1 == v2) vertices_equivalent(s1, s2);
  if(!steps[current_step].symbol_exists(e11)) {undefined_symbol(e11); return;}
  v1 = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(e12)) {undefined_symbol(e12); return;}
  v2 = steps[current_step].get_current_vertex();
  if(v1 == v2) vertices_equivalent(s1, s2);
  //- last edge can be null, in case of only one limit set
  if(!e21.empty() && !e22.empty()) {
    if(!steps[current_step].symbol_exists(e21)) {undefined_symbol(e21); return;}
    v1 = steps[current_step].get_current_vertex();
    if(!steps[current_step].symbol_exists(e22)) {undefined_symbol(e22); return;}
    v2 = steps[current_step].get_current_vertex();
    if(v1 == v2) vertices_equivalent(s1, s2);
  }
  lim1 = search_edge(e11, e12);
  if(lim1 == -1) {
	std::string s = "edge [" + e11 + "," + e12 + "] is undefined";
    yyerror((char *)s.c_str());
    return;
  }
  if(!e21.empty() && !e22.empty()) {
    lim2 = search_edge(e21, e22);
    if(lim2 == -1) {
      std::string s = "edge [" + e21 + "," + e22 + "] is undefined";
      yyerror((char *)s.c_str());
      return;
    }
  }
  eIdx = search_edge_and_hide_equivalent(s1, s2);
  if(eIdx != -1) {
    edge& e = steps[current_step].ref_line(eIdx);
    e.set_type(t);
    if(s1 == e.get_v1()) {
      e.set_limit_v1(lim1);
      if(lim2 != -1) {
	e.set_limit_v2(lim2);
//	e.set_limits(allLimit);
      }
    } else {
      e.set_limit_v2(lim1);
      if(lim2 != -1) {
	e.set_limit_v1(lim2);
//	e.set_limits(allLimit);
      }
    }
    e.show();
  } else {
    if(lim2 != -1) {
      steps[current_step].add_line(edge(s1, s2, t, allLimit, lim1,lim2));
    } else {
      steps[current_step].add_line(edge(s1, s2, t, limitV1, lim1));
    }
  }
}

void read_fold_both(const std::string& s1, const std::string& s2, edge_type t,
		    int sp, const std::string& e1, const std::string& e2, bool isLimitV1)
{
  int lim = -1, eIdx = -1;
  if(verbose) {
    std::cout << "Step #" << current_index << " : a ";
    switch (t) {
     case edgeValley : std::cout << "valley"; break;
     case edgeMountain : std::cout << "mountain"; break;
     case edgeFold : std::cout << "existent"; break;
     case edgeBorder : std::cout << "border"; break;
     case edgeXray : std::cout << "Xray"; break;
     default : yyerror("Wrong edge type in read_fold_both");
    }
    std::cout << " fold is defined between " << s1 << " and " << s2 << std::endl;
  }
  //- check all input edges ...
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  vertex v1 = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2 = steps[current_step].get_current_vertex();
  if(v1 == v2) vertices_equivalent(s1, s2);
  if(!steps[current_step].symbol_exists(e1)) {undefined_symbol(e1); return;}
  v1 = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(e2)) {undefined_symbol(e2); return;}
  v2 = steps[current_step].get_current_vertex();
  if(v1 == v2) vertices_equivalent(s1, s2);

  if(sp > 100) {
    yyerror("Blank space given > 100%!");
  }
  
  lim = search_edge(e1, e2);
  if(lim == -1) {
    std::string s = "edge [" + e1 + "," + e2 + "] is undefined";
    yyerror((char *)s.c_str());
    return;
  }
  eIdx = search_edge_and_hide_equivalent(s1, s2);
  if(eIdx != -1) {
    edge& e = steps[current_step].ref_line(eIdx);
    if((s1 == e.get_v1() && isLimitV1) || (s1 != e.get_v1() && !isLimitV1)) {
      e.set_limit_v1(lim);
      e.set_space_v2(sp);
    } else {
      e.set_space_v1(sp);
      e.set_limit_v2(lim);
    }
    e.set_type(t);
    e.show();
  } else {
    if(isLimitV1)
      steps[current_step].add_line(edge(s1, s2, t, limitV1, lim, sp));
    else 
      steps[current_step].add_line(edge(s1, s2, t, limitV2, sp, lim));
  }
}

void read_shift(const std::string& s, double dx, double dy)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : " << s;
    std::cout << " is shifted dx=" << dx << "mm dy=" << dy << "mm" << std::endl;
  }
  if(!steps[current_step].symbol_exists(s)) {undefined_symbol(s); return;}
  steps[current_step].ref_current_vertex().set_dx_dy(dx,dy);
}

void read_text(const std::string& s,const std::string& text)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : "
	      << "string \"" << text << "\" is added to point "
	      << s << std::endl;
  }
  if(!steps[current_step].symbol_exists(s)) {undefined_symbol(s); return;}
  vertex& v = steps[current_step].ref_current_vertex();

  v.add_text(text);
}

void read_unshift_all() 
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : "
	      << "unshift all vertices" << std::endl;
  }
  for(step::it_vertices it = steps[current_step].ref_begin_vertices(); 
      it != steps[current_step].ref_end_vertices(); ++it) {
    it->set_dx_dy(0.0, 0.0);
  }
}

void read_unshift(std::vector<std::string>& vertices)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : "
	      << "unshift vertices {";
    std::cout << vertices[0];
    int nver = vertices.size();
    for(int i=1;i<nver;i++) std::cout << ", " << vertices[i];
    std::cout << "}" << std::endl;
  }
  std::vector<std::string>::iterator it = vertices.begin();
  for(;it != vertices.end(); ++it) {
    if(!steps[current_step].symbol_exists(*it)) {undefined_symbol(*it); return;}
    vertex& v = steps[current_step].ref_current_vertex();
    v.set_dx_dy(0.0, 0.0);
  }
}

void read_duplicate(const std::string& s1, const std::string& s2)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : new vertex " << s1
	      << " is defined as a clone of of vertex " << s2 << std::endl;
  }
  if(steps[current_step].symbol_exists(s1)) {symbol_already_defined(s1); return;}
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  vertex v2 = steps[current_step].get_current_vertex();

  vertex v1 = v2;
  v1.set_name(s1);

  steps[current_step].add_symbol(v1);
}

void read_rabbit_ear(const std::string& s, const std::string& scenter,
		     const std::string& s1, const std::string& s2, const std::string& s3,
		     const std::string& se1, const std::string& se2, bool is_center_given)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : new vertex " << s;
    if(!is_center_given) std::cout << " and " << center << " are";
    else  std::cout << " is";
    std::cout << " defined by rabbit ear (" << s1 <<  ", " << s2 <<  ", " << s3
	      << ") on edge [" << se1 << ", " << se2 << "]" << std::endl;
  }
  //- Collecting data 
  if(!steps[current_step].symbol_exists(se1)) {undefined_symbol(se1); return;}
  const vertex& e1 = steps[current_step].ref_current_vertex();
  if(!steps[current_step].symbol_exists(se2)) {undefined_symbol(se2); return;}
  const vertex& e2 = steps[current_step].get_current_vertex();
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  const vertex& v1 = steps[current_step].ref_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  const vertex& v2 = steps[current_step].ref_current_vertex();
  if(!steps[current_step].symbol_exists(s3)) {undefined_symbol(s3); return;}
  const vertex& v3 = steps[current_step].ref_current_vertex();
  if(IS_NULL(v1.distance(v2, v3))) {
    std::string s = "vertices " + s1 + ", " + s2 + ", " + s3 + " are aligned";
    yyerror((char *)s.c_str());
    return;
  }
  if(search_edge(se1, se2) == -1) {undefined_edge(se1, se2); return; }
  
  vertex center;
  if(is_center_given) {
    if(!steps[current_step].symbol_exists(scenter)) {undefined_symbol(scenter); return;}
    center = steps[current_step].get_current_vertex();
  } else {
    //- in case of the center isn't given, compute it position as the intersection
    //- of the three bissectors (two ones are enough).
    vertex p, q;
    if(v1 == v2) vertices_equivalent(s1, s2);
    else if(v1 == v3) vertices_equivalent(s1, s3);
    else {
      p = v1.bissector(v2, v3, current_step ? steps[current_step-1].get_rotate():0,
		       steps[current_step].get_scale());
    }
    if(v2 == v3) vertices_equivalent(s2, s3);
    else {    
      q = v2.bissector(v1, v3, current_step ? steps[current_step-1].get_rotate():0,
		       steps[current_step].get_scale());
      center = v1.intersection(p, v2, q,
			       current_step ? steps[current_step-1].get_rotate():0,
			       steps[current_step].get_scale());
    }
  }
  vertex v;
  if(!(e1 == e2)) {
    vertex temp = center.projection(v1, v2,
				    current_step ? steps[current_step-1].get_rotate():0,
				    steps[current_step].get_scale());
    v = center.intersection(temp, e1, e2,
			    current_step ? steps[current_step-1].get_rotate():0,
			    steps[current_step].get_scale());
    if(!is_center_given) {
      center.set_name(scenter);
      steps[current_step].add_symbol(center);
    } else {
      //- case in which the center is given we have to compute v.

      //- MOVING_VERTEX => v1
      //- VERTEX_DEST   => v2
      //- a1=symmetry(MOVING_VERTEX,[VERTEX_DEST,CENTER])
      //- b1=intersection([VERTEX_DEST, CENTER],[MOVING_VERTEX, V3])
      //- a2=symmetry(a1,[V3, CENTER])
      //- b2=symmetry(b1,[V3, CENTER])
      //- a3=symmetry(a2,[b2, CENTER]) // a3 : point d'arriv‚e...
      //- a4=mediator([a1,a3])intersect‚ avec [VERTEX_DEST, V3]
      //- a5=symmetry(a4,[VERTEX_DEST, CENTER])
      //- VERTEX = intersection([a5, CENTER], EDGE)
      vertex a1 = v1.symmetry(v2, center,
			      current_step ? steps[current_step-1].get_rotate():0,
			      steps[current_step].get_scale());
      vertex b1 = v2.intersection(center, v1, v3,
				  current_step ? steps[current_step-1].get_rotate():0,
				  steps[current_step].get_scale());
      vertex a2 = a1.symmetry(v3, center,
			      current_step ? steps[current_step-1].get_rotate():0,
			      steps[current_step].get_scale());
      vertex b2 = b1.symmetry(v3, center,
			      current_step ? steps[current_step-1].get_rotate():0,
			      steps[current_step].get_scale());
      vertex a3 = a2.symmetry(b2, center,
			      current_step ? steps[current_step-1].get_rotate():0,
			      steps[current_step].get_scale());
      vertex a4 = a1.mediator(a3, v2, v3,
			      current_step ? steps[current_step-1].get_rotate():0,
			      steps[current_step].get_scale());
      vertex a5 = a4.symmetry(v2, center,
			      current_step ? steps[current_step-1].get_rotate():0,
			      steps[current_step].get_scale());
      v = a5.intersection(center, e1, e2,
			  current_step ? steps[current_step-1].get_rotate():0,
			  steps[current_step].get_scale());
    }
    v.set_name(s);
    steps[current_step].add_symbol(v);
  } else vertices_equivalent(se1, se2);
}

void read_push_arrow(const std::string& s, int angle, int distance)
{
  if(angle > 180 || angle < -180) yyerror("Wrong angle. Should be degrees (-180, 180).");
  if(verbose) {
    std::cout << "Step #" << current_index << " : a push arrow is drawn close to point "
	      << s << " (" << distance << " mm) with a orientation of " << angle << " degree";
    if(angle > 1) std::cout << 's';
    std::cout << std::endl;
  }

  if(!steps[current_step].symbol_exists(s)) {undefined_symbol(s); return;}
  steps[current_step].add_arrow_symbol(new push_arrow(s, angle, distance));
}

void read_push_arrow(const std::string& s)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : a push arrow is drawn close to point "
	      << s << std::endl;
  }

  if(!steps[current_step].symbol_exists(s)) {undefined_symbol(s); return;}
  const vertex& v = steps[current_step].ref_current_vertex();
  
  vertex o(0.0, 0.0); //- center of the step
  double angle = o.get_angle_from_horizontal(v,
					     current_step ? steps[current_step-1].get_rotate():0,
					     steps[current_step].get_scale());
  angle += current_step ? steps[current_step-1].get_rotate():0;
  steps[current_step].add_arrow_symbol(new push_arrow(s, angle, 0));
}

void read_push_arrow(const std::string& s, const std::string& s1, const std::string& s2, int distance)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : a push arrow is drawn close to point "
	      << s << " (" << distance << " mm)" << std::endl;
  }

  if(!steps[current_step].symbol_exists(s)) {undefined_symbol(s1); return;}
  const vertex& v = steps[current_step].ref_current_vertex();
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  const vertex& v1 = steps[current_step].ref_current_vertex();
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  const vertex& v2 = steps[current_step].ref_current_vertex();
  if(search_edge(s, s1) == -1) {undefined_edge(s, s1); return;}
  if(search_edge(s, s2) == -1) {undefined_edge(s, s2); return;}
  if(v == v1) {vertices_equivalent(s, s1); return;}
  if(v == v2) {vertices_equivalent(s, s2); return;}

  vertex vTemp = v.bissector(v1, v2,
			     current_step ? steps[current_step-1].get_rotate():0,
			     steps[current_step].get_scale());
  double angle = v.get_angle_from_horizontal(vTemp,
					     current_step ? steps[current_step-1].get_rotate():0,
					     steps[current_step].get_scale());

  angle += current_step ? steps[current_step-1].get_rotate():0;

  steps[current_step].add_arrow_symbol(new push_arrow(s, angle+180, distance));
}

void read_open_arrow(const std::string& s1, const std::string& s2, const std::string& side)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : an open arrow is drawn on the "
	      << side << " of edge [" << s1 << " ," << s2 << "]." << std::endl;
  }

  bool right_side;
  
  if(side == "left") right_side = false;
  else if(side == "right") right_side = true;
  else {
    std::string ss = side + " is not a valid edge side (left/right)";
    yyerror((char *)ss.c_str());
    return;
  }
  if(!steps[current_step].symbol_exists(s1)) {undefined_symbol(s1); return;}
  if(!steps[current_step].symbol_exists(s2)) {undefined_symbol(s2); return;}
  if(search_edge(s1, s2) == -1) {undefined_edge(s1, s2); return; }
  steps[current_step].add_arrow_symbol(new open_arrow(s1, s2, right_side));
}

void read_repeat_arrow(const std::string& s, unsigned int nb, const std::string& label1, const std::string& label2, int angle, int distance)
{
  if(angle > 180 || angle < -180) yyerror("Wrong angle. Should be degrees (-180, 180).");
  if(verbose) {
    std::cout << "Step #" << current_index << " : a repeat arrow (" << nb << " time";
    if(nb > 1) std::cout << 's';
    std::cout << ") is drawn close to point " << s << " (" << distance
	      << " mm) with a orientation of " << angle << " degree";
    if(angle > 1) std::cout << 's';
    std::cout << std::endl;
  }

  if(!steps[current_step].symbol_exists(s)) {undefined_symbol(s); return;}
  if (nb > 10) {
    std::string ss = "Repeat number should be lesser than 10 (" + itoa(nb) + ")";
    yyerror((char *)ss.c_str());
    return;
  }
  std::string labExp1=label1, labExp2=label2;
  if(!label1.empty()) {
    if(labels.find(label1) == labels.end()) {undefined_label(label1); return;}
    else labExp1 = itoa(labels[label1]);
  }
  if(!label2.empty()) {
    if(labels.find(label2) == labels.end()) {undefined_label(label2); return;}
    else labExp2 = itoa(labels[label2]);
  }
  steps[current_step].add_arrow_symbol(new repeat_arrow(s, angle, 0, nb, labExp1, labExp2));
}

void read_repeat_arrow(const std::string& s, unsigned int nb, const std::string& label1, const std::string& label2)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : a repeat arrow (" << nb << " time";
    if(nb > 1) std::cout << 's';
    std::cout << ") is drawn close to point " << s << std::endl;
  }

  if(!steps[current_step].symbol_exists(s)) {undefined_symbol(s); return;}
  const vertex& v = steps[current_step].ref_current_vertex();
  if (nb > 10) {
    std::string ss = "Repeat number should be lesser than 10 (" + itoa(nb) + ")";
    yyerror((char *)ss.c_str());
    return;
  }
  vertex o(0.0, 0.0); //- center of the step
  double angle = o.get_angle_from_horizontal(v,
					     current_step ? steps[current_step-1].get_rotate():0,
					     steps[current_step].get_scale());
  angle += current_step ? steps[current_step-1].get_rotate():0;

  std::string labExp1=label1, labExp2=label2;
  if(!label1.empty()) {
    if(labels.find(label1) == labels.end()) {undefined_label(label1); return;}
    else labExp1 = itoa(labels[label1]);
  }
  if(!label2.empty()) {
    if(labels.find(label2) == labels.end()) {undefined_label(label2); return;}
    else labExp2 = itoa(labels[label2]);
  }
  steps[current_step].add_arrow_symbol(new repeat_arrow(s, angle, 0, nb, labExp1, labExp2));
}

void read_label(const std::string& s)
{
  if(verbose) {
    std::cout << "Step #" << current_index << " : the label '"
	      << s << "' is defined to reference this step" << std::endl;
  }
  if(labels.find(s) != labels.end()) {
    std::string ss = s+" is already used to reference step "+
		     steps[labels[s]].get_index();
    yyerror((char*) ss.c_str());
  }
  labels[s] = current_index;
}
