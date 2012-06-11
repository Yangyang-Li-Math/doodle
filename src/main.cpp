// This looks like c, but it's really -*-c++-*-
// 
// Copyright (c) 2000 Doodle
// Jerome GOUT - Tue Mar 21 2000
// 


#ifdef WIN32
#pragma warning (disable : 4786)
#endif // WIN32

#include <cstdio>
#include "vertex.h"
#include "edge.h"
#include "diag_header.h"
#include "step.h"
#include "ps_output.h"

//- parsing main function
int yyparse();

enum format_type {PS, TEXT, GRAPH};

//- defined in parser.y
extern int yydebug;
extern FILE* yyin;
extern std::string file_name;
extern diag_header  info;  //- defined in parser.y
extern std::vector<step> steps; //- defined in parser.y
extern bool verbose; //- defined in parser.y
static format_type format = PS;
static std::string output;

static void usage(){
	std::cerr << "Doodle -- J. Gout, X. Fouchet and V. Osele -- Copyright 2000-2001\n\n"
       << "Usage: doodle [-h] [-v] [-f format] [-o file] file\n"
       << "   -h        : this short usage help\n"
       << "   -v        : verbose mode\n"     
       << "   -f format : specifies output format (default ps)\n"
       << "               known formats are : ps, text, graph\n"
       << "   -o file   : uses `file' as the output file\n"
       << "               by default produces extension .ps\n"
       << "   file      : input diagram file (.doo)\n";
  exit(1);
}

void check_format(char * f)
{
  std::string sf(f);
  if(sf == std::string("ps")) format = PS;
  else if (sf == std::string("text")) format = TEXT;
  else if (sf == std::string("graph")) format = GRAPH;
  else {
    std::cerr << f << " : unsupported output format" << std::endl;
    usage();
  }
}

std::string extension_format()
{
  switch (format) {
   case PS : return std::string(".ps");
   case TEXT : return std::string(".txt");
   case GRAPH : return std::string(".gr");
   default : std::cerr << "Unknown format : this should never appear ..." << std::endl;
    return std::string(".ps");
  }
}

std::string get_correct_output_name(const std::string& fn)
{
  unsigned int extLength;
  
  if (fn.compare(fn.length()-4, 4, ".DOO") == 0) extLength = 4;
  else if (fn.compare(fn.length()-4, 4, ".doo") == 0) extLength = 4;
  else if(fn.compare(fn.length()-2, 2, ".d") == 0) extLength = 2;
  else {
    std::cerr << "Doodle input filename error: " << fn << std::endl
	      << "A Doodle input file should have .doo or .d extension" << std::endl;
    return std::string();
  }
  return fn.substr(0, fn.length()-extLength)+std::string(".ps");
}

bool arg_managment(int argc, char** argv)
{
  int ind=1;

  if(argc < 2) {
    std::cerr << "Doodle expects at least an input file ... '"
	      << argv[0] << " -h' for more details" << std::endl;
    return false;
  }
  
  while(ind < argc) {
    if (argv[ind][0] == '-'){
      switch (argv[ind][1]) {
       case 'h' : usage(); break;
       case 'v' : verbose = true; break;
       case 'f' : check_format(argv[++ind]); break;
       case 'o' : output = argv[++ind]; break;
       default: 
        std::cerr << "Unknown option: " << argv[ind] << std::endl;
	usage();
	break;
      }
    } else {
      file_name = std::string(argv[ind]);
      if(output.empty()) {
	output = get_correct_output_name(file_name);
      }
      if (output.empty()) return false;
    }
    ind++;
  }
  return true;
}

int main(int argc, char * argv[]){

  //yydebug = 1;
  if(!arg_managment(argc, argv)) return 1;
  if ((yyin = fopen(file_name.c_str(), "r")) == NULL) {
    perror(file_name.c_str());
    return  2;
  }
  if(yyparse()) {
    std::cerr << "An error occured during parsing" << std::endl;
  } else {
    fclose(yyin);
    //- make the output
    switch (format) {
     case PS : ps_output(output.c_str()); break;
     case TEXT : std::cerr << "Sorry, not yet implemented ..." << std::endl; break;
     case GRAPH : std::cerr << "Sorry, not yet implemented ..." << std::endl; break;
    }
  }
  return 0;
}
