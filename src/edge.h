/*  This looks like c, but it's really -*-c++-*-
 * 
 *  Copyright (C) 2000 Doodle
 *  Jerome GOUT - Tue Mar 28 2000
 */


#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"
#include "global_def.h"
#include <string>

enum edge_type {edgeBorder, edgeValley, edgeMountain, edgeFold, edgeXray };
enum limit_type {allPercent=0, allLimit=3, limitV1=2, limitV2=1}; 

class edge {
public:
  edge(const std::string&, const std::string&, edge_type=edgeBorder,int=allPercent, int=0, int=0);
  edge(const edge&);
  edge& operator=(const edge&);
  virtual ~edge();

  //- Accessors ...
  std::string get_name() const {return Name;}
  void set_name(const char* _name) {Name = _name;}
  void set_name(const std::string _name) {Name = _name;}

  const std::string& get_v1() const {return V1;}
  void set_v1(const std::string& v) {V1 = v;}

  const std::string& get_v2() const {return V2;}
  void set_v2(const std::string& v) {V2 = v;}

  edge_type get_type() const {return Type;}
  void set_type(edge_type t) {Type = t;}
  
  int  get_space_v1() const {return Limit_V1;}
  void set_space_v1(int p) {Limit_V1 = p; Limits &= 1;} //- forces bit #1 to 0

  int  get_space_v2() const {return Limit_V2;}
  void set_space_v2(int p) {Limit_V2 = p; Limits &= 2;} //- forces bit #0 to 0

  bool is_visible() const {return Visible;}
  void show() {Visible = true;}
  void hide() {Visible = false;}

  int get_limits() const {return Limits;}
  void set_limits(limit_type l) {Limits = l;}

  int  get_limit_v1() const {return Limit_V1;}
  void set_limit_v1(int l1) {Limit_V1 = l1; Limits |= 2;} //- forces bit #1 to 1

  int  get_limit_v2() const {return Limit_V2;}
  void set_limit_v2(int l2) {Limit_V2 = l2; Limits |= 1;} //- forces bit #0 to 1

  bool has_limit_v1() const {return (Limits >> 1)!=0;}
  bool has_limit_v2() const {return Limits & 1;}
  bool has_space_v1() const {return !(Limits >> 1);}
  bool has_space_v2() const {return !(Limits & 1);}
  
  bool equal(const std::string& s1, const std::string& s2);

  void set_debug(bool b) {Debug = b;}
  bool is_debug() const {return Debug;}
  
private:
  std::string    Name;
  std::string    V1;
  std::string    V2;
  edge_type      Type;
  bool 		 Visible;  
  int            Limits;
  int   	 Limit_V1;   //- index in edge tab of the limit
  int   	 Limit_V2;
  /// Debug mode
  bool           Debug;
  
#ifdef DEBUG
  public:
    void gdb_info(const std::string & = std::string()) const;
#endif /* ! DEBUG */
};

#endif  /* ! EDGE_H */


