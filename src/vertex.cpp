// This looks like c, but it's really -*-c++-*-
// 
// Copyright (c) 2000 Doodle
// Jerome GOUT - Tue Mar 21 2000
// 

#include <iostream>
#include "global_def.h"

#include "vertex.h"
#include "vect.h"
#define SQR(x) ((x)*(x))

//- Transformation between units and the given millimeter value
#define MM_TO_UNIT(x) ((x)*5.0)
#define UNIT_TO_MM(x) ((x)/5.0)

void yyerror(char *); //- defined in parser.y
void yywarning(char *); //- defined in parser.y

//- exception class used in intersection
class bad_intersection
{};

vertex::vertex() : name("null"), x(0.0), y(0.0), dx(0), dy(0), debug(false)
{}

vertex::vertex(const std::string& n) :
  name(n), x(0.0), y(0.0), dx(0), dy(0), debug(false)
{}

vertex::vertex(double _x, double _y, double _dx, double _dy):
  name("null"), x(_x), y(_y), dx(_dx), dy(_dy), debug(false)
{}

vertex::vertex(const char * n, double _x, double _y, double _dx, double _dy):
  name(n), x(_x), y(_y), dx(_dx), dy(_dy), debug(false)
{}

vertex::vertex(const std::string n, double _x, double _y, double _dx, double _dy):
  name(n), x(_x), y(_y), dx(_dx), dy(_dy), debug(false)
{}

vertex::vertex(const vertex& Ref): name(Ref.name),
  x(Ref.x), y(Ref.y), dx(Ref.dx), dy(Ref.dy), debug(Ref.debug)
{}

vertex& vertex::operator=(const vertex& Ref)
{
  if (this != &Ref) {
    name = Ref.name;
    x = Ref.x;
    y = Ref.y;
    dx = Ref.dx;
    dy = Ref.dy;
  }
  return *this;
}

bool vertex::operator==(const vertex& Ref) const
{
  if (this != &Ref) {
    double norme=sqrt((Ref.x-x)*(Ref.x-x)+(Ref.y-y)*(Ref.y-y));
    return (norme<1e-6 && norme>-1e-6);
  }
  return true;
}

vertex::~vertex()
{}

bool vertex::has_text() const
{
  return name.find(TEXT_MARKER_CHAR) != std::string::npos;
}

std::string vertex::get_name() const
{
  std::string::size_type idx;

  idx = name.find(TEXT_MARKER_CHAR);
  if(idx == std::string::npos) return name;
  else return name.substr(0, idx);
}

void vertex::add_text(const std::string s)
{
  name += TEXT_MARKER_CHAR;
  name += s;
}

std::string vertex::get_text() const
{
  std::string::size_type idx;

  idx = name.find(TEXT_MARKER_CHAR);
  if(idx == std::string::npos) return std::string();
  else return name.substr(idx+1);
}

void vertex::clear_text()
{
   std::string::size_type idx;

  idx = name.find(TEXT_MARKER_CHAR);
  if(idx != std::string::npos) name = name.substr(0, idx);
}

//- visually equivalent.
//- the test should contain dx and dy
bool vertex::equivalent(const vertex& Ref) const
{
  if (this != &Ref) {
    double norme1=sqrt((Ref.x-x)*(Ref.x-x)+(Ref.y-y)*(Ref.y-y));
    double norme2=sqrt((Ref.dx-dx)*(Ref.dx-dx)+(Ref.dy-dy)*(Ref.dy-dy));
    return (norme1<1e-6 && norme1>-1e-6) && (norme2<1e-6 && norme2>-1e-6); // A~B 
  }
  return true;
}

vertex vertex::middle(const vertex& v) const
{
    return fraction(v,1,2);
}

vertex vertex::fraction(const vertex& v, int n, int d) const
{
  vertex vRes("null", x+n*(v.x-x)/d, y+n*(v.y-y)/d);

  //- new point dx dy computation
  if(!(*this == v)) {
    vRes.dx = (d-n)/(double)d*dx+(double)n/d*v.dx;
    vRes.dy = (d-n)/(double)d*dy+(double)n/d*v.dy;
  }
  return vRes;
}

// returns symtric of current vertex (AB axis)
// warning : A and B must be different
vertex vertex::basic_symmetry(const vertex& A, const vertex& B) const
{
  vect u(A,B);
  double n=u.norm();

  u=(1/n)*u; // u: unit vector AB
  vertex Q=A+((vect(A,*this)*u)*u); //Q=proj(this)/AB : AQ=(Athis.u).u
  vertex P=*this+2*vect(*this,Q); //thisP=2thisQ
  return P;
}

//- returns symtric of current vertex (AB axis)
//- warning : A and B must be different 
//- dx and dy of *this A and B are considered
vertex vertex::symmetry(const vertex& A, const vertex& B,
			double angle, double scale) const
{
  vertex p;

  if(A == B) {
    std::string ss = A.get_name() + " equivalent to " + B.get_name()+ ".";
    yyerror((char*)ss.c_str());
  } else {
    p = basic_symmetry(A,B);
    vertex s = apply_shift(angle, scale);
    vertex aBis = A.apply_shift(angle, scale);
    vertex bBis = B.apply_shift(angle, scale);
    vertex pBis;
    if(!(aBis == bBis)) pBis = s.basic_symmetry(aBis, bBis);
    else {
      std::string ss = A.get_name() + " equivalent to " + B.get_name() +
	" due to shift operations.";
      yywarning((char*)ss.c_str());
      pBis = p;
    }

    vect v(p, pBis);
    
    p.dx = scale/100.0*UNIT_TO_MM(cos(angle*M_PI/180)*v.get_x()-sin(angle*M_PI/180)*v.get_y());
    p.dy = scale/100.0*UNIT_TO_MM(sin(angle*M_PI/180)*v.get_x()+cos(angle*M_PI/180)*v.get_y());

  }
  return p;
}

// intersection : (thisA) and (BC)
// warning : (thisA) and (BC) must be not parallel, A!=this, B!=C
// this method computes only x and y fields of instance.
vertex vertex::basic_intersection(const vertex& A,const vertex& B,const vertex& C) const
{
  // line equations ax+by=c 1:thisA 2:BC
  double a1=y-A.y;
  double b1=A.x-x;
  double c1=a1*x+b1*y;
  double a2=B.y-C.y;
  double b2=C.x-B.x;
  double c2=a2*C.x+b2*C.y;
  
  // lines intersection :
  double det=a1*b2-a2*b1;
  if (det==0){
    std::string ss = "no intersection of visual parallel lines [";
    ss +=  name + ", " + A.name + "] and [" + B.name + ", " + C.name + "]\n";
    yywarning((char*)ss.c_str());
    throw bad_intersection();
  }
  double _x=(c1*b2-b1*c2)/det;
  double _y=(a1*c2-a2*c1)/det;
  
  return vertex("null",_x,_y);
}

vertex vertex::intersection(const vertex& A,const vertex& B,const vertex& C,
			    double angle, double scale) const
{
  //- computes its x y only
  vertex i = basic_intersection(A,B, C);
  //- computes its dx and dy now
  vertex v1 = apply_shift(angle, scale);
  vertex v2 = A.apply_shift(angle, scale);
  vertex v3 = B.apply_shift(angle, scale);
  vertex v4 = C.apply_shift(angle, scale);

  //- visual intersection
  vertex iBis;
  try {
    iBis = v1.basic_intersection(v2, v3, v4);
  } 
  catch (bad_intersection) {
    //- in case of intesection error
    iBis = i;
  }
  vect v(i, iBis);
  i.dx = scale/100.0*UNIT_TO_MM(cos(angle*M_PI/180)*v.get_x()-sin(angle*M_PI/180)*v.get_y());
  i.dy = scale/100.0*UNIT_TO_MM(sin(angle*M_PI/180)*v.get_x()+cos(angle*M_PI/180)*v.get_y());
  return i;
}

// bissector : on angle (this A, this B)
// returns P : intersection of bissector and (AB)
// caution : this!=A, this!=B
vertex vertex::bissector(const vertex& A, const vertex& B,
			 double angle, double scale) const
{
    vect u=vect(*this,A);
    vect v=vect(*this,B);
    double nu=u.norm();
    double nv=v.norm();
    if (nu<1e-6 || nv<1e-6){
	std::cerr << "assert error : undefined bissector\n";
	exit(0);
    }
    u=(1/nu)*u; // u,v : unit vector of thisA, thisB
    v=(1/nv)*v;
    vect w=u+v; // u+v dir vector
    if (w.sqr_norm()<1e-6) // case u,v colinear
	w=u.ortho();
    vertex Q=*this+w; // thisQ is the bissector
    vertex P=intersection(Q,A,B, angle, scale);
    return P;
}

// mediator of [this A]
// returns intersection of mediator and (BC)
// caution : (this A) and (BC) not orthogonal, A!=this, B!=C
vertex vertex::mediator(const vertex& A, const vertex& B, const vertex& C,
			double angle, double scale) const
{
    vertex M=middle(A);
    vect u=vect(*this,A);
    vect v=u.ortho(); // dir vector of mediator
    vertex P=M.intersection(M+v,B,C, angle, scale);
    return P;
}

// orthogonal projection of *this on (AB)
// caution : A!=B
vertex  vertex::projection(const vertex& A, const vertex& B,
			   double, double) const
{
    vect u(A,B);
    u=(1/u.norm())*u;
    return A+((vect(A,*this)*u)*u);
}

// fold involved when a vertex this is set on a line (BC) around
// a pivot A. two image points are possible ; the first point met
// in a direct rotation is chosen if first=true, the second is
// chosen if first=false
// return intersection of fold with line (DE)
// caution : B!=C, D!=E, Athis > dist(A,(BC))
vertex vertex::vertex_to_line(const vertex& A, const vertex& B, const vertex& C,
			      const vertex& D, const vertex& E, bool first,
			      double angle, double scale) const
{
    // looking for intersection of (BC) and circle(A,Athis)
    // line (BC) : ax+by=c
    double a=-vect(B,C).get_y();
    double b=vect(B,C).get_x();
    double c=a*B.x+b*B.y;
    double r2=vect(A,*this).sqr_norm(); // sqr(radius of circle)
    vertex P,P1,P2;
    if (a==0){
	if (b==0){
	    std::cerr << "assert error : equal points in vertextoline\n";
	    exit(0);
	}
	double y1=c/b;
	double s=r2-SQR(y1-A.y);
	if (s<0){
	    std::cerr << "assert error : no intersections in vertextoline\n";
	    exit(0);
	}
	//- 2 solutions : x=A.x +/- srqt(s)
	P1 = vertex(A.x+s,y1);
	P2 = vertex(A.x-s,y1);
    }
    else{ // a!=0
	// replacing x by (c-by)/a,
	// we get a 2nd order equation : a0y^2+b0y+c0=0
	double a0=SQR(b/a)+1;
	double b0=2*((A.x-c/a)*b/a-A.y);
	double c0=SQR(-A.x+c/a)+SQR(A.y)-r2;
	// resolution
	double delta=SQR(b0)-4*a0*c0;
	if (delta<0){
	    std::cerr << "assert error : no intersections in vertextoline\n";
	    exit(0);
	}
	double y1=(-b0-sqrt(delta))/(2*a0);
	double y2=(-b0+sqrt(delta))/(2*a0);
	double x1=(c-b*y1)/a;
	double x2=(c-b*y2)/a;
	P1=vertex(x1,y1);
	P2=vertex(x2,y2);
    }
    // choice between P1 and P2
    vect v(A,*this);
    vect v1(A,P1);
    vect v2(A,P2);
    double angle1=atan2(v1.get_y(),v1.get_x())-atan2(v.get_y(),v.get_x()); 
    double angle2=atan2(v2.get_y(),v2.get_x())-atan2(v.get_y(),v.get_x()); 
    // angle1 and angle2 between 0 and 2PI
    angle1 = angle1 < 0 ? angle1 + 2*M_PI : angle1;
    angle2 = angle2 < 0 ? angle2 + 2*M_PI : angle2;
    if (first)
	P = angle1 < angle2 ? P1:P2;
    else
	P = angle1 < angle2 ? P2:P1;
    vertex Q=A.bissector(*this,P, angle, scale); // Q belongs to fold
    return A.intersection(Q,D,E, angle, scale);
}

//- angle is expressed in degres and converted into the method
//- scale is expressed in %
vertex vertex::apply_shift(double angle, double scale) const
{
  double newX = x+100.0/scale*(MM_TO_UNIT(dx*cos(angle*M_PI/180)+dy*sin(angle*M_PI/180)));
  double newY = y+100.0/scale*(MM_TO_UNIT(-dx*sin(angle*M_PI/180)+dy*cos(angle*M_PI/180)));
  return vertex(newX, newY);
}

//------------------------------------------------------------

// returns true if (*this A) and (BC) parallel
bool vertex::is_parallel(const vertex& A, const vertex& B, const vertex& C) const
{
    vect v1=vect(*this,A);
    vect v2=vect(B,C);
    double ps=v1*v2;
    double n=v1.norm()*v2.norm();
    ps=ps>0?ps:-ps;
    if (ps>0.99*n && ps <1.01*n) return true;
    else return false;
}

// returns true if (*this A) and (BC) orthogonal
bool vertex::is_orthogonal(const vertex& A, const vertex& B, const vertex& C) const
{
    vect v1=vect(*this,A);
    vect v2=vect(B,C);
    double ps=v1*v2;
    double n=v1.norm()*v2.norm();
    if (ps>-0.01*n && ps <0.01*n) return true;
    else return false;
}

// distance between vertex this and line (AB)
// caution : A!=B
double vertex::distance(const vertex& A, const vertex& B) const
{
  //- note: dummies parameters for angle and scale
  return vect(*this,projection(A,B, 0.0, 0.0)).norm();
}

double vertex::get_angle_from_horizontal(const vertex& v, double angle, double scale) const
{
  double res;

  apply_shift(angle, scale);
  v.apply_shift(angle, scale);
  
  if(*this == v) return 0.0;
  else res = atan2(v.get_y()-this->get_y(),v.get_x()-this->get_x());
  if(res < 0) res += 2*M_PI;
  return 180*res/M_PI;
}

//------------------------------------------------------------

#ifdef DEBUG
#include <iostream>

void vertex::gdb_info(const std::string & s) const
{
  std::cout << s << "name  = " << name << std::endl;
  std::cout << s << "x     = " << x << std::endl;
  std::cout << s << "y     = " << y << std::endl;
  std::cout << s << "dx    = " << dx << std::endl;
  std::cout << s << "dy    = " << dy << std::endl;
  std::cout << s << "debug = " << debug << std::endl;
}
#endif /* ! DEBUG */

