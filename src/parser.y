/***
 *** Syntaxic parser
 ***
 *** Jerome Gout -- March 2000
 ***/

%{

#ifdef WIN32
#pragma warning (disable : 4786)
#endif // WIN32
#include <stdio.h>
#include <string>
#include <time.h>  //- for localtime
#include <sys/types.h> //- for time_t
#include <vector>
#include <map>
#include <malloc.h>
  
#include "diag_header.h"
#include "date.h"
#include "step.h"
#include "vertex.h"
#include "edge.h"
#include "macro.h"
#include "read_instruction.h"
#include "global_def.h"
#include "color.h"

  //- work variables
  int line_num = 1;
  std::string file_name;

  int current_index = 1;
  int current_step = 0;
  bool is_sub_step = false;
  int nbComment = 0;

  //- global data parser fills
  diag_header info;
  std::vector<step> steps;
  bool verbose = false;
  int visible_height = CLIP_HEIGHT; 
  int visible_width = CLIP_WIDTH;
  int scale         = 100;  //- original scale factor is identity (100%)
  std::string center;
  bool clip;
  typedef std::map<std::string,macro> macromap;
  macromap macros;
  static macromap::iterator current_macro;
  
 //- Avoid error messages from yacc
 void yyerror(char *s);
 void yywarning(char* s);
 int  yylex();
 
%}

%union {
  int                      ival;
  double                   fval;
  std::string              *String;  /* any string (or symbol) */
  date                     *Date;
  diag_header              *Info;
  std::vector<std::string> *List;
  color                    *Color;
}

%token HEADER STEP TURN_VERTICAL TURN_HORIZONTAL ROTATE
%token DESIGN_DATE DIAG_DATE DESIGNER DIAGRAMER DIAGRAMMER TODAY TITLE
%token _LEFT_MARGIN _RIGHT_MARGIN _TOP_MARGIN _BOTTOM_MARGIN
%token _VSPACE _HSPACE
%token SQUARE DIAMOND MIDDLE FRACT CAPTION
%token VALLEY_FOLD MOUNTAIN_FOLD FOLD SCALE
%token POINT_TO_POINT POINT_TO_LINE LINE_TO_LINE 
%token SIMPLE_ARROW RETURN_ARROW
%token HIDE SHOW BORDER INTERSECTION SYMMETRY CUT
%token DEBUG_INST DEBUG_POINT_INST DEBUG_LINE_INST XRAY_FOLD
%token VISIBLE_HEIGHT VISIBLE_WIDTH VISIBLE_CENTER
%token COLOR_FRONT COLOR_BACK FILL UNFILL
%token V_RECTANGLE H_RECTANGLE
%token CLIP UNCLIP PERPENDICULAR PARALLEL MOVE INTER_CUT
%token SPACE_FOLD COMMENT EPS RESET SHIFT
%token MACRO MACROTEXT OPERATOR DARKER LIGHTER TEXT UNSHIFT
%token RABBIT_EAR OPEN_ARROW PUSH_ARROW REPEAT_ARROW
%token LABEL

%token DEBUG_INFO

%token <ival> INTEGER
%token <fval> FLOAT
%token <String> SYMBOL STRING
%token <String> OPERATOR
%token <ival> MACROTEXT

%type <Date>  a_date
%type <List>  symbol_list
%type <Color> a_color

%%

diagram : definitions header_statement statement
{
  std::string s;
  //- end of parsing. It's time to resolve references
  //- ref can be found in comments, captions, text associated to vertex
  info.set_comment1(expand_ref(info.get_comment1()));
  info.set_comment2(expand_ref(info.get_comment2()));
  info.set_comment3(expand_ref(info.get_comment3()));
  //- For all steps
  for(unsigned int stepId=0; stepId < steps.size(); stepId++) {
    //- Change all captions
    for(step::it_captions itCap = steps[stepId].ref_captions_begin();
	itCap != steps[stepId].ref_captions_end(); ++itCap) {
      *itCap = expand_ref(*itCap);
    }
    //- For all text vertices
    for(step::it_vertices it = steps[stepId].ref_begin_vertices();
      it != steps[stepId].ref_end_vertices(); ++it) {
      if(it->has_text()) {
	s = expand_ref(it->get_text());
	it->clear_text();
	it->add_text(s);
      }
    }
  }
}
;

definitions : /* empty rule */
| definitions pre_statement
;

statement : /* empty rule */
| statement step_statement
| statement inter_step_statement
;

/* HEARDER_STATEMENT */
pre_statement :
  macro_def
| ';'
;

/* HEARDER_STATEMENT */
header_statement : HEADER '{' header_data '}'
;

header_data : /* empty rule */
| header_data design_date
| header_data diag_date
| header_data designer
| header_data diagrammer
| header_data title
| header_data header_visible_height
| header_data header_visible_width
| header_data left_margin
| header_data right_margin
| header_data top_margin
| header_data bottom_margin
| header_data vspace
| header_data hspace
| header_data color_front
| header_data color_back
| header_data comment
;

design_date : DESIGN_DATE '(' a_date ')' ';'
{
  info.set_design_date(*$3);
}
;

diag_date : DIAG_DATE '(' a_date ')' ';' 
{
  info.set_diagram_date(*$3);
}
;

designer : DESIGNER '(' STRING ')' ';' 
{
  info.set_designer(*$3);
}
;

diagrammer : DIAGRAMER '(' STRING ')' ';' 
{
  yywarning("Syntax changes: use \\diagrammer instead of");
  info.set_diagrammer(*$3);
}
| DIAGRAMMER '(' STRING ')' ';' 
{
  info.set_diagrammer(*$3);
}
;

title : TITLE '('  STRING ')' ';' 
{
  info.set_title(*$3);
}
|  TITLE '('  STRING  ';'
{
  yyerror("Expecting ')' before ';'");
}
/* | error ';' */

header_visible_height : VISIBLE_HEIGHT '(' INTEGER ')' ';'
{
  visible_height = $3;
}
;

header_visible_width : VISIBLE_WIDTH '(' INTEGER ')' ';'
{
  visible_width = $3;
}
;

left_margin : _LEFT_MARGIN '(' INTEGER ')' ';'
{
  info.set_left_margin($3);
}
;

right_margin : _RIGHT_MARGIN '(' INTEGER ')' ';'
{
  info.set_right_margin($3);
}
;

top_margin : _TOP_MARGIN '(' INTEGER ')' ';'
{
  info.set_top_margin($3);
}
;

bottom_margin : _BOTTOM_MARGIN '(' INTEGER ')' ';'
{
  info.set_bottom_margin($3);
}
;

vspace : _VSPACE '(' INTEGER ')' ';'
{
  info.set_vspace($3);
}
;

hspace : _HSPACE '(' INTEGER ')' ';'
{
  info.set_hspace($3);
}
;

color_front : COLOR_FRONT '(' SYMBOL ')' ';'
{
  info.set_front_color(*$3);
  delete $3;
}
|  COLOR_FRONT '(' INTEGER ',' INTEGER ',' INTEGER ')' ';'
{
  info.set_front_color($3, $5, $7)
}
;

color_back :  COLOR_BACK '(' SYMBOL ')' ';'
{
  info.set_back_color(*$3);
  delete $3;

}
|  COLOR_BACK '(' INTEGER ',' INTEGER ',' INTEGER ')' ';'
{
  info.set_back_color($3, $5, $7 )
}
;

comment : COMMENT '(' STRING ')' ';' 
{
  switch(nbComment) {
  case 0 : info.set_comment1(*$3);
    delete $3; break;
  case 1 : info.set_comment2(*$3);
    delete $3; break;
  case 2 : info.set_comment3(*$3);
    delete $3; break;
  default : yywarning("Only 3 comment lines are allowed.");
  }
  nbComment++;
}
;

/* INTER_STEP_STATEMENT */

inter_step_statement : 
  visible_height_inter
| visible_width_inter
| scale_inter
| visible_center_inter
| clip_inter
| unclip_inter
| macro_def
| rotate_inter
| turn_over_vertical_inter
| turn_over_horizontal_inter
| rotate_old_syntax_error
| ';'
;

visible_height_inter : VISIBLE_HEIGHT '(' INTEGER  ')' ';'
{
  visible_height = $3;
}
;

visible_width_inter : VISIBLE_WIDTH '(' INTEGER ')'';'
{
  visible_width = $3;
}
;

scale_inter : SCALE '(' INTEGER ')' ';'
{
  if($3 < 1) yyerror("Wrong scale factor, it should be greater than 1");
  scale = $3;
}
;

visible_center_inter : VISIBLE_CENTER '(' SYMBOL ')' ';'
{
  read_visible_center_inter(*$3);
  delete $3;
}
;

clip_inter : CLIP ';'
{
  if(verbose) {
    std::cout << "Before step #" << current_index << " : clipping state : ON" << std::endl;
  }
  clip = true;
}
;

unclip_inter : UNCLIP ';'
{
  if(verbose) {
    std::cout << "Before step #" << current_index << " : clipping state : OFF" << std::endl;
  }
  clip = false;
}
;

macro_def : MACRO macro_ope macro_param '{' macro_part '}'
{
  if(verbose) {
    std::cout << "macro : " << current_macro->first << std::endl;
  }
}
;

macro_ope : OPERATOR
{
  // *$1 const std::string& nom de la marco
  std::pair<macromap::iterator, bool> p = macros.insert(
	macromap::value_type(*$1,macro())
  );
  if (!p.second) {
	yyerror("macro already exists. Could be extended in a next version...");
	current_macro = macros.end();
  }
  else {
	current_macro  = p.first;
  }
  delete $1;
}
;

macro_param : /* empty rule */
| '(' ')'
| '(' symbol_list ')'
{
  // *$2 const std::vector<std::string>& vertices
  if (current_macro != macros.end())
	current_macro->second.set(*$2);
  delete $2;
}
;

macro_part :  /* empty rule */
| macro_part OPERATOR
{
 if (current_macro != macros.end())
	current_macro->second.add(*$2);
  delete $2;
}
| macro_part SYMBOL
{
  if (current_macro != macros.end())
	current_macro->second.addsymb(*$2);
  delete $2;
}
| macro_part MACROTEXT
{
  if (current_macro != macros.end())
	current_macro->second.add($2);
}
;

rotate_inter : ROTATE '(' FLOAT ')' ';'
{
  if(current_step != 0) read_rotate($3);
  else {
    yywarning("operator \\rotate is skipped (forbidden before first step)");
  }
}
| ROTATE '(' INTEGER ')' ';'
{
  if(current_step != 0) read_rotate((double)$3);
  else {
    yywarning("operator \\rotate is skipped (forbidden before first step)");
  }
}
;

turn_over_vertical_inter : TURN_VERTICAL ';'
{
  read_turn_over(true);
}
;

turn_over_horizontal_inter :  TURN_HORIZONTAL ';'
{
  read_turn_over(false);  
}
;

/* STEP_STATEMENT */
step_statement : begin_step '{' instruction_list '}'
{
  if(!steps[current_step].get_captions_number()) {
    steps[current_step].add_caption("");
  }
  //- update dx dy according to local scale factor 
  steps[current_step].update_dx_dy(scale);
  current_step++; //- index for steps array
  if(!is_sub_step) current_index++; //- index of current step in diagram
}
;

/* Used to make things in the begining of a new step ... */
begin_step : STEP   
{
  //- adding a new step filled with current symbols and lines from others steps
  read_new_step();
}
| STEP SYMBOL
{
  read_new_step();
}
;

instruction_list :
| instruction_list diamond_inst
| instruction_list square_inst
| instruction_list middle_inst
| instruction_list fract_inst
| instruction_list caption_inst
| instruction_list valley_fold_inst
| instruction_list mountain_fold_inst
| instruction_list point_to_point_inst
| instruction_list point_to_line_inst
| instruction_list line_to_line_inst
| instruction_list simple_arrow_inst
| instruction_list return_arrow_inst
| instruction_list hide_inst
| instruction_list show_inst
| instruction_list border_inst
| instruction_list fold_inst
| instruction_list intersection_inst
| instruction_list symmetry_inst
| instruction_list cut_inst
| instruction_list debug_inst
| instruction_list debug_point_inst
| instruction_list debug_line_inst
| instruction_list xray_fold_inst
| instruction_list visible_height_inst
| instruction_list visible_width_inst
| instruction_list visible_center_inst
| instruction_list scale_inst
| instruction_list fill_inst
| instruction_list unfill_inst
| instruction_list debug_info_inst
| instruction_list vertical_rectangle_inst
| instruction_list horizontal_rectangle_inst
| instruction_list clip_inst
| instruction_list unclip_inst
| instruction_list perpendicular_inst
| instruction_list parallel_inst
| instruction_list move_inst	
| instruction_list inter_cut_inst	
| instruction_list space_fold_inst
| instruction_list eps_inst
| instruction_list reset_inst
| instruction_list shift_inst
| instruction_list text_inst
| instruction_list unshift_inst
| instruction_list duplicate_inst
| instruction_list rabbit_ear_inst
| instruction_list push_arrow_inst
| instruction_list open_arrow_inst
| instruction_list repeat_arrow_inst
| instruction_list label_inst
| instruction_list rotate_in_step_error
| instruction_list turn_over_vertical_error
| instruction_list turn_over_horizontal_error
| instruction_list ';'
| error ';'
{
  yyerror("wrong instruction (perhaps a ; is missing)");
  YYABORT
}
;

diamond_inst : DIAMOND '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ')' ';'
{
  read_diamond(*$3,*$5,*$7,*$9);
  delete $3; delete $5; delete $7; delete $9;
}
;

square_inst : SQUARE '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ')' ';'
{
  read_square(*$3,*$5,*$7,*$9); 
  delete $3; delete $5; delete $7; delete $9;
} 
;

middle_inst : SYMBOL '=' MIDDLE '(' SYMBOL ',' SYMBOL ')' ';'
{
  read_fraction(*$1, *$5, *$7, 1, 2);
  delete $1; delete $5; delete $7;
}
;

fract_inst : SYMBOL '=' FRACT '(' SYMBOL ',' SYMBOL ',' INTEGER ',' INTEGER ')' ';'
{
  read_fraction(*$1, *$5, *$7, $9, $11);
  delete $1; delete $5; delete $7;
}
;

caption_inst : CAPTION '(' STRING ')' ';'
{
  read_caption(*$3);
  delete $3;
}
;

valley_fold_inst : VALLEY_FOLD '(' SYMBOL ',' SYMBOL ')' ';'
{
  read_fold_percent(*$3, *$5, edgeValley, 0, 0);
  delete $3; delete $5;
}
| VALLEY_FOLD '(' SYMBOL ',' SYMBOL ',' INTEGER ',' INTEGER ')' ';'
{
  read_fold_percent(*$3, *$5, edgeValley, $7, $9);
  delete $3; delete $5;
}
| VALLEY_FOLD '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_limit(*$3, *$5, edgeValley, *$8, *$10, *$14, *$16);
  delete $3; delete $5; delete $8; delete $10; delete $14; delete $16;
}
| VALLEY_FOLD '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_limit(*$3, *$5, edgeValley, *$8, *$10, "", "");
  delete $3; delete $5; delete $8; delete $10;
}
| VALLEY_FOLD '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' INTEGER ')' ';'
{
  read_fold_both(*$3, *$5, edgeValley, $13, *$8, *$10, true);
  delete $3; delete $5; delete $8; delete $10;
}
| VALLEY_FOLD '(' SYMBOL ',' SYMBOL ',' INTEGER ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_both(*$3, *$5, edgeValley, $7, *$10, *$12, false);
  delete $3; delete $5; delete $10; delete $12;
}
;

mountain_fold_inst : MOUNTAIN_FOLD '(' SYMBOL ',' SYMBOL ')' ';'
{
  read_fold_percent(*$3, *$5, edgeMountain, 0, 0);
  delete $3; delete $5;
}
| MOUNTAIN_FOLD '(' SYMBOL ',' SYMBOL ',' INTEGER ',' INTEGER ')' ';'
{
  read_fold_percent(*$3, *$5, edgeMountain, $7, $9);
  delete $3; delete $5;
}
| MOUNTAIN_FOLD '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_limit(*$3, *$5, edgeMountain, *$8, *$10, *$14, *$16);
  delete $3; delete $5; delete $8; delete $10; delete $14; delete $16;
}
| MOUNTAIN_FOLD '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_limit(*$3, *$5, edgeMountain, *$8, *$10, "", "");
  delete $3; delete $5; delete $8; delete $10;
}
| MOUNTAIN_FOLD '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' INTEGER ')' ';'
{
  read_fold_both(*$3, *$5, edgeMountain, $13, *$8, *$10, true);
  delete $3; delete $5; delete $8; delete $10;
}
| MOUNTAIN_FOLD '(' SYMBOL ',' SYMBOL ',' INTEGER ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_both(*$3, *$5, edgeMountain, $7, *$10, *$12, false);
  delete $3; delete $5; delete $10; delete $12;
}
;

border_inst : BORDER '(' SYMBOL ',' SYMBOL ')' ';'
{
  read_fold_percent(*$3, *$5, edgeBorder, 0, 0);
  delete $3; delete $5;
}
| BORDER '(' SYMBOL ',' SYMBOL ',' INTEGER ',' INTEGER ')' ';'
{
  read_fold_percent(*$3, *$5, edgeBorder, $7, $9);
  delete $3; delete $5;
}
| BORDER '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_limit(*$3, *$5, edgeBorder, *$8, *$10, *$14, *$16);
  delete $3; delete $5; delete $8; delete $10; delete $14; delete $16;
}
| BORDER '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_limit(*$3, *$5, edgeBorder, *$8, *$10, "", "");
  delete $3; delete $5; delete $8; delete $10;
}
| BORDER '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' INTEGER ')' ';'
{
  read_fold_both(*$3, *$5, edgeBorder, $13, *$8, *$10, true);
  delete $3; delete $5; delete $8; delete $10;
}
| BORDER '(' SYMBOL ',' SYMBOL ',' INTEGER ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_both(*$3, *$5, edgeBorder, $7, *$10, *$12, false);
  delete $3; delete $5; delete $10; delete $12;
}
;

fold_inst : FOLD '(' SYMBOL ',' SYMBOL ')' ';'
{
  read_fold_percent(*$3, *$5, edgeFold, FOLDSPC, FOLDSPC);
  delete $3; delete $5;
}
| FOLD '(' SYMBOL ',' SYMBOL ',' INTEGER ',' INTEGER ')' ';'
{
  read_fold_percent(*$3, *$5, edgeFold, $7, $9);
  delete $3; delete $5;
}
| FOLD '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_limit(*$3, *$5, edgeFold, *$8, *$10, *$14, *$16);
  delete $3; delete $5; delete $8; delete $10; delete $14; delete $16;
}
| FOLD '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_limit(*$3, *$5, edgeFold, *$8, *$10, "", "");
  delete $3; delete $5; delete $8; delete $10;
}
| FOLD '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' INTEGER ')' ';'
{
  read_fold_both(*$3, *$5, edgeFold, $13, *$8, *$10, true);
  delete $3; delete $5; delete $8; delete $10;
}
| FOLD '(' SYMBOL ',' SYMBOL ',' INTEGER ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_both(*$3, *$5, edgeFold, $7, *$10, *$12, false);
  delete $3; delete $5; delete $10; delete $12;
}
;

xray_fold_inst : XRAY_FOLD '(' SYMBOL ',' SYMBOL ')' ';'
{
  read_fold_percent(*$3, *$5, edgeXray, 0, 0);
  delete $3; delete $5;
}
| XRAY_FOLD '(' SYMBOL ',' SYMBOL ',' INTEGER ',' INTEGER ')' ';'
{
  read_fold_percent(*$3, *$5, edgeXray, $7, $9);
  delete $3; delete $5;
}
| XRAY_FOLD '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_limit(*$3, *$5, edgeXray, *$8, *$10, *$14, *$16);
  delete $3; delete $5; delete $8; delete $10; delete $14; delete $16;
}
| XRAY_FOLD '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_limit(*$3, *$5, edgeXray, *$8, *$10, "", "");
  delete $3; delete $5; delete $8; delete $10;
}
| XRAY_FOLD '(' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' INTEGER ')' ';'
{
  read_fold_both(*$3, *$5, edgeXray, $13, *$8, *$10, true);
  delete $3; delete $5; delete $8; delete $10;
}
| XRAY_FOLD '(' SYMBOL ',' SYMBOL ',' INTEGER ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_fold_both(*$3, *$5, edgeXray, $7, *$10, *$12, false);
  delete $3; delete $5; delete $10; delete $12;
}
;

point_to_point_inst : '[' SYMBOL ',' SYMBOL ']' '=' POINT_TO_POINT '(' SYMBOL ',' SYMBOL ','
                                                      '[' SYMBOL ',' SYMBOL ']' ','
                                                      '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_point_to_point(*$2, *$4, *$9, *$11, *$14, *$16, *$20, *$22);
  delete $2; delete $4; delete $9; delete $11; delete $14; delete $16;
  delete $20; delete $22;
}
;

point_to_line_inst : SYMBOL  '=' POINT_TO_LINE '(' SYMBOL ',' SYMBOL ','
                                               '[' SYMBOL ',' SYMBOL ']' ','
                                               '[' SYMBOL ',' SYMBOL ']'')' ';'
{
  read_point_to_line(*$1, std::string("first"), *$5, *$7, *$10, *$12, *$16, *$18);
  delete $1; delete $5; delete $7; delete $10; delete $12;
  delete $16; delete $18;
}
;

| SYMBOL  '=' POINT_TO_LINE '(' SYMBOL ',' SYMBOL ','
                            '[' SYMBOL ',' SYMBOL ']' ','
                            '[' SYMBOL ',' SYMBOL ']' ','
                                SYMBOL ')' ';'
{
  read_point_to_line(*$1, *$21, *$5, *$7, *$10, *$12, *$16, *$18);
  delete $1; delete $5; delete $7; delete $10; delete $12;
  delete $16; delete $18; delete $21;
}
;


line_to_line_inst : '[' SYMBOL ',' SYMBOL ']' '=' LINE_TO_LINE '(' '[' SYMBOL ',' SYMBOL ']' ',' '[' SYMBOL ',' SYMBOL ']' ',' '[' SYMBOL ',' SYMBOL ']' ','
                                                           '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_line_to_line(*$2, *$4, *$10, *$12, *$16, *$18, *$22, *$24, *$28, *$30);
  delete $2; delete $4; delete $10; delete $12; delete $16; delete $18; delete $22; delete $24; delete $28; delete $30;
}
| SYMBOL '=' LINE_TO_LINE '(' SYMBOL ',' SYMBOL ',' SYMBOL ','
                     '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_line_to_line(*$1,*$5, *$7, *$9, *$12, *$14);
  delete $1; delete $5; delete $7; delete $9; delete $12; delete $14;
}
;

simple_arrow_inst: SIMPLE_ARROW '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ')' ';'
{ //- simple_arrow( B, C, none, valley) between two vertices (right)
  read_simple_arrow(*$3, *$5, *$7, *$9, std::string("right"), 60);//- 60 is SIMPLE_ARROW_ANGLE
  delete $3; delete $5; delete $7; delete $9; 
}
| SIMPLE_ARROW '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ')' ';'
{ //- simple_arrow( B, C, none, valley, left) between two vertices
  read_simple_arrow(*$3, *$5, *$7, *$9, *$11, 60);//- 60 is SIMPLE_ARROW_ANGLE
  delete $3; delete $5; delete $7; delete $9; delete $11;
}
| SIMPLE_ARROW '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ',' INTEGER')' ';'
{ //- simple_arrow( B, C, none, valley, left, angle) between two vertices
  read_simple_arrow(*$3, *$5, *$7, *$9, *$11, $13);
  delete $3; delete $5; delete $7; delete $9; delete $11;
}
| SIMPLE_ARROW '(' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' SYMBOL ',' SYMBOL ')' ';' 
{ //- simple_arrow( B, [C, AB2], none, valley) a vertex across a edge (right)
  read_simple_arrow(*$3, *$6, *$8, *$11, *$13,  std::string("right"), 60); //- 60 is SIMPLE_ARROW_ANGLE
  delete $3; delete $6; delete $8; delete $11; delete $13;
}
| SIMPLE_ARROW '(' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' SYMBOL ',' SYMBOL ',' SYMBOL ')' ';' 
{ //- simple_arrow( B, [C, AB2], none, valley, left) a vertex across a edge
  read_simple_arrow(*$3, *$6, *$8, *$11, *$13, *$15, 60); //- 60 is SIMPLE_ARROW_ANGLE
  delete $3; delete $6; delete $8; delete $11; delete $13; delete $15;
}
| SIMPLE_ARROW '(' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ',' SYMBOL ',' SYMBOL ',' SYMBOL ',' INTEGER ')' ';' 
{ //- simple_arrow( B, [C, AB2], none, valley, left, angle) a vertex across a edge
  read_simple_arrow(*$3, *$6, *$8, *$11, *$13, *$15, $17);
  delete $3; delete $6; delete $8; delete $11; delete $13; delete $15;
}
;

return_arrow_inst: RETURN_ARROW '(' '[' SYMBOL ',' SYMBOL ']' ',' '[' SYMBOL ',' SYMBOL ']' ','
                                         SYMBOL ',' SYMBOL ')' ';' 
{
  read_return_arrow(*$4, *$6, *$10, *$12, *$15, *$17, std::string("right"), 50); //- 50 is RETURN_ARROW_RATIO
  delete $4; delete $6; delete $10; delete $12; delete $15; delete $17;
}
| RETURN_ARROW '(' '[' SYMBOL ',' SYMBOL ']' ',' '[' SYMBOL ',' SYMBOL ']' ','
                                         SYMBOL ',' SYMBOL ',' SYMBOL ')' ';' 
{
  read_return_arrow(*$4, *$6, *$10, *$12, *$15, *$17, *$19, 50); //- 50 is RETURN_ARROW_RATIO
  delete $4; delete $6; delete $10; delete $12; delete $15; delete $17; delete $19;
}
| RETURN_ARROW '(' '[' SYMBOL ',' SYMBOL ']' ',' '[' SYMBOL ',' SYMBOL ']' ','
                       SYMBOL ',' SYMBOL ',' SYMBOL ',' INTEGER')' ';' 
{
  read_return_arrow(*$4, *$6, *$10, *$12, *$15, *$17, *$19, $21);
  delete $4; delete $6; delete $10; delete $12; delete $15; delete $17; delete $19;
}
;

hide_inst : HIDE '(' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_hide_show(*$4, *$6, false);
  delete $4; delete $6;
}
| HIDE '(' symbol_list ')' ';'
{
  read_hide_show(*$3, false);
  delete $3;
}
;

show_inst : SHOW '(' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_hide_show(*$4, *$6, true);
  delete $4; delete $6;
}
| SHOW '(' symbol_list ')' ';'
{
  read_hide_show(*$3, true);
  delete $3;
}
;

intersection_inst : SYMBOL '=' INTERSECTION '(' '[' SYMBOL ',' SYMBOL ']' ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_intersection(*$1, *$6, *$8, *$12, *$14);
  delete $1; delete $6; delete $8; delete $12; delete $14;
}
| SYMBOL '=' INTERSECTION '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ')' ';'
{
  read_intersection(*$1, *$5, *$7, *$9, *$11);
  delete $1; delete $5; delete $7; delete $9; delete $11;
}
;

symmetry_inst : SYMBOL '=' SYMMETRY '(' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_symmetry(*$1,*$5,*$8, *$10);
  delete $1; delete $5; delete $8; delete $10;
}
;

cut_inst : CUT '(' '[' SYMBOL ',' SYMBOL ']' ',' SYMBOL ')' ';'
{
  read_cut(*$4, *$6,*$9);
  delete $4; delete $6; delete $9;
}
;

debug_inst : DEBUG_INST ';'
{
  read_debug_point();
  read_debug_line();
}
;

debug_point_inst : DEBUG_POINT_INST ';'
{
  read_debug_point();
}
| DEBUG_POINT_INST '(' symbol_list ')' ';'
{
  read_debug_point(*$3);
  delete $3;
}
;

debug_line_inst : DEBUG_LINE_INST ';'
{
  read_debug_line();  
}
| DEBUG_LINE_INST '(' symbol_list ')' ';'
{
  read_debug_line(*$3);
  delete $3;
}
| DEBUG_LINE_INST '(' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_debug_line(*$4, *$6);
  delete $4; delete $6;
}
;

visible_height_inst : VISIBLE_HEIGHT '(' INTEGER ')' ';'
{
  read_visible_height($3);
}
;

visible_width_inst : VISIBLE_WIDTH '(' INTEGER ')' ';'
{
  read_visible_width($3);
}
;

visible_center_inst : VISIBLE_CENTER '(' SYMBOL ')' ';'
{
  read_visible_center(*$3);
  delete $3;
}
;

scale_inst : SCALE '('  INTEGER ')' ';'
{
  read_scale($3);
}
;

fill_inst : FILL '(' a_color ',' symbol_list ')' ';'
{
  read_fill(*$3, *$5);
  delete $3; delete $5;
}
;

unfill_inst : UNFILL '(' symbol_list ')' ';'
{
  read_unfill(*$3);
  delete $3;
}
| UNFILL ';'
{
  read_unfill_all();
}
;

debug_info_inst : DEBUG_INFO '(' STRING ')' ';'
{
#ifdef DEBUG
  steps[current_step].gdb_info(*$3);
#endif /* ! DEBUG */
  delete $3;
}
;

vertical_rectangle_inst : V_RECTANGLE '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ',' 
     INTEGER ')' ';'
{
  read_vertical_rectangle(*$3, *$5, *$7, *$9, $11);
  delete $3; delete $5; delete $7; delete $9;
}
| V_RECTANGLE '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ',' 
     SYMBOL ')' ';'
{
  read_vertical_rectangle(*$3, *$5, *$7, *$9, *$11);
  delete $3; delete $5; delete $7; delete $9; delete $11;
}
;

horizontal_rectangle_inst : H_RECTANGLE '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL 
 ',' INTEGER ')' ';'
{
  read_horizontal_rectangle(*$3, *$5, *$7, *$9, $11);
  delete $3; delete $5; delete $7; delete $9;
}
| H_RECTANGLE '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ',' 
     SYMBOL ')' ';'
{
  read_horizontal_rectangle(*$3, *$5, *$7, *$9, *$11);
  delete $3; delete $5; delete $7; delete $9; delete $11;
}
;

clip_inst : CLIP ';'
{
  read_clip(true);
}
| CLIP '(' INTEGER ',' INTEGER ')' ';'
{
  read_clip(true, $3, $5);
}
;

unclip_inst : UNCLIP ';'
{
  read_clip(false);
}
;

perpendicular_inst : SYMBOL '=' PERPENDICULAR '(' '[' SYMBOL ',' SYMBOL ']' ',' SYMBOL ')' ';'
{
  read_perpendicular(*$1, *$6, *$8, *$11);
  delete $1; delete $6; delete $8; delete $11;
}
| SYMBOL '=' PERPENDICULAR '(' '[' SYMBOL ',' SYMBOL ']' ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_perpendicular(*$1, *$6, *$8, *$11, *$14, *$16);
  delete $1; delete $6; delete $8; delete $11; delete $14; delete $16;
}
;

parallel_inst : SYMBOL '=' PARALLEL '(' '[' SYMBOL ',' SYMBOL ']' ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_parallel(*$1, *$6, *$8, *$11, *$14, *$16);
  delete $1; delete $6; delete $8; delete $11; delete $14; delete $16;
}
;

move_inst : MOVE '(' SYMBOL ',' SYMBOL ')' ';'
{
  read_move(*$3, *$5);
  delete $3; delete $5;
}
| MOVE '(' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_move(*$3, *$6, *$8);
  delete $3; delete $6; delete $8;
}
;

inter_cut_inst : SYMBOL '=' INTER_CUT '(' '[' SYMBOL ',' SYMBOL ']' ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_inter_cut(*$1, *$6,*$8,*$12,*$14);
  delete $1; delete $6; delete $8; delete $12; delete $14;
}
;

space_fold_inst: SPACE_FOLD '('  '[' SYMBOL ',' SYMBOL ']' ',' INTEGER ',' INTEGER')' ';'
{
  read_space_fold(*$4, *$6, $9, $11);
  delete $4; delete $6;
}
;

eps_inst: EPS '(' STRING ')' ';'
{
  read_eps(*$3, 210, 480, 380, 655);
  delete $3;
}
| EPS '(' STRING ',' INTEGER ',' INTEGER',' INTEGER ',' INTEGER ')' ';'
{
  read_eps(*$3, $5, $7, $9, $11);
  delete $3;
}
;

reset_inst: RESET ';'
{
  read_reset();
}
;

shift_inst: SHIFT '(' SYMBOL ',' INTEGER ',' INTEGER ')' ';'
{
  read_shift(*$3, $5,$7);
  delete $3;
}
| SHIFT '(' SYMBOL ',' FLOAT ',' FLOAT ')' ';'
{
  read_shift(*$3, $5,$7);
  delete $3;
}
| SHIFT '(' SYMBOL ',' INTEGER ',' FLOAT ')' ';'
{
  read_shift(*$3, $5,$7);
  delete $3;
}
| SHIFT '(' SYMBOL ',' FLOAT ',' INTEGER ')' ';'
{
  read_shift(*$3, $5,$7);
  delete $3;
}
;

unshift_inst: UNSHIFT ';'
{
  read_unshift_all();
}
| UNSHIFT '(' symbol_list ')' ';'
{
  read_unshift(*$3);
  delete $3;
}
;

text_inst: TEXT '(' SYMBOL ',' STRING ')' ';'
{
  read_text(*$3, *$5);
  delete $3; delete $5;
}
;

duplicate_inst: SYMBOL '=' SYMBOL ';'
{
  read_duplicate(*$1, *$3);
  delete $1; delete $3;
}
;

rabbit_ear_inst: '[' SYMBOL ',' SYMBOL ']' '=' RABBIT_EAR '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  //- [VERTEX, CENTER] = \rabbit_ear(MOVING_VERTEX, VERTEX_DEST, V3, EDGE);
  read_rabbit_ear(*$2, *$4, *$9, *$11, *$13, *$16, *$18, false);
  delete $2; delete $4; delete $9; delete $11; delete $13; delete $16; delete $18;
}
| '[' SYMBOL ',' SYMBOL ']' '=' RABBIT_EAR '(' SYMBOL ',' SYMBOL ',' SYMBOL ')' ';'
{
  //- [VERTEX, CENTER] = \rabbit_ear(MOVING_VERTEX, VERTEX_DEST, V3);
  read_rabbit_ear(*$2, *$4, *$9, *$11, *$13, *$9, *$11, false);
  delete $2; delete $4; delete $9; delete $11; delete $13;

}
| SYMBOL '=' RABBIT_EAR '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ',' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  //- VERTEX = \rabbit_ear(MOVING_VERTEX, VERTEX_DEST, V3, CENTER, EDGE);
  read_rabbit_ear(*$1, *$11, *$5, *$7, *$9, *$14, *$16, true);
  delete $1; delete $11; delete $5; delete $7; delete $9; delete $14; delete $16;
}
| SYMBOL '=' RABBIT_EAR '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' SYMBOL ')' ';'
{
  //- VERTEX = \rabbit_ear(MOVING_VERTEX, VERTEX_DEST, V3, CENTER);
  read_rabbit_ear(*$1, *$11, *$5, *$7, *$9, *$5, *$7, true);
  delete $1; delete $11; delete $5; delete $7; delete $9;
}
;

push_arrow_inst: PUSH_ARROW '(' SYMBOL ',' INTEGER ',' INTEGER ')' ';'
{
  read_push_arrow(*$3, $5, $7);
  delete $3;
}
| PUSH_ARROW '(' SYMBOL ',' INTEGER ')' ';'
{
  read_push_arrow(*$3, $5, 0);
  delete $3;
}
| PUSH_ARROW '(' SYMBOL ')' ';'
{
  read_push_arrow(*$3);
  delete $3;
}
| PUSH_ARROW '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' INTEGER ')' ';'
{
  read_push_arrow(*$3, *$5, *$7, $9);
  delete $3; delete $5; delete $7;
}
| PUSH_ARROW '(' SYMBOL ',' SYMBOL ',' SYMBOL ')' ';'
{
  read_push_arrow(*$3, *$5, *$7, 0);
  delete $3; delete $5; delete $7;
}
;

open_arrow_inst: OPEN_ARROW '(' '[' SYMBOL ',' SYMBOL ']' ',' SYMBOL ')' ';'
{
  read_open_arrow(*$4, *$6, *$9);
  delete $4; delete $6; delete $9;
}
| OPEN_ARROW '(' '[' SYMBOL ',' SYMBOL ']' ')' ';'
{
  read_open_arrow(*$4, *$6, std::string("right"));
  delete $4; delete $6;
}
;

repeat_arrow_inst: REPEAT_ARROW '(' SYMBOL ',' INTEGER ',' INTEGER ',' INTEGER ')' ';'
{
  if($5 < 1) yyerror("Repeat number shouldn't be negative or null.");
  read_repeat_arrow(*$3, (unsigned int) $5, std::string(), std::string(), $7, $9);
  delete $3;
}
| REPEAT_ARROW '(' SYMBOL ',' INTEGER ',' INTEGER ')' ';'
{
  if($5 < 1) yyerror("Repeat number shouldn't be negative or null.");
  read_repeat_arrow(*$3, (unsigned int) $5, std::string(), std::string(), $7, 0);
  delete $3;
}
| REPEAT_ARROW '(' SYMBOL ',' INTEGER ')' ';'
{
  if($5 < 1) yyerror("Repeat number shouldn't be negative or null.");
  read_repeat_arrow(*$3, (unsigned int) $5, std::string(), std::string());
  delete $3;
}
| REPEAT_ARROW '(' SYMBOL ')' ';'
{
  read_repeat_arrow(*$3, (unsigned int) 1, std::string(), std::string());
  delete $3;
}
| REPEAT_ARROW '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' INTEGER ',' INTEGER ',' INTEGER ')' ';'
{
  if($9 < 1) yyerror("Repeat number shouldn't be negative or null.");
  read_repeat_arrow(*$3, (unsigned int) $9, *$5, *$7, $11, $13);
  delete $3; delete $5; delete $7;  
}
| REPEAT_ARROW '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' INTEGER ',' INTEGER ')' ';'
{
  if($9 < 1) yyerror("Repeat number shouldn't be negative or null.");
  read_repeat_arrow(*$3, (unsigned int) $9, *$5, *$7, $11, 0);
  delete $3; delete $5; delete $7;  
}
| REPEAT_ARROW '(' SYMBOL ',' SYMBOL ',' SYMBOL ',' INTEGER')' ';'
{
  if($9 < 1) yyerror("Repeat number shouldn't be negative or null.");
  read_repeat_arrow(*$3, (unsigned int) $9, *$5, *$7);
  delete $3; delete $5; delete $7;  
}
| REPEAT_ARROW '(' SYMBOL ',' SYMBOL ',' SYMBOL ')' ';'
{
  read_repeat_arrow(*$3, 1, *$5, *$7);
  delete $3; delete $5; delete $7;  
}
;

label_inst: LABEL '(' SYMBOL ')' ';'
{
  read_label(*$3);
  delete $3;
}
;

//-------------------------------------------------------------

symbol_list: SYMBOL
{
  $$ = new std::vector<std::string>;
  $$->push_back(*$1);
  delete $1;
}
| symbol_list ',' SYMBOL
  {
    $$ = $1;
    $$->push_back(*$3);
    delete $3;
}
;

a_date : INTEGER ',' INTEGER ',' INTEGER
{ /* Ex. 03, 29, 2000 */
  $$ = new date($3, $1, $5);
}
| TODAY
{
  time_t c = time(NULL);
  struct tm * t = localtime(&c);
  
  $$ = new date(t->tm_mday, t->tm_mon+1, t->tm_year+1900);
}
| INTEGER
{
  $$ = new date(0 ,0 , $1);
}
;

a_color: SYMBOL
{
  $$ = read_color(*$1);
  delete $1;
}
| DARKER '(' a_color ')'
{
  $$ = new color(*$3, 0.9);
  delete $3;
}
| LIGHTER '(' a_color ')'
{
  $$ = new color(*$3, 1.1);
  delete $3;
}
;

/*--------------------------------------------------------------------*/
/* Those rules are rules that have been modified rules		      */
/* to make old doodle file compile we continue to accept those rules  */
/* but one day in a near future those rules can be removed            */

rotate_old_syntax_error: ROTATE INTEGER ';'
{
  char msg[100];

  sprintf(msg, "Syntax has changed\n\\rotate syntax has changed. Use now \\rotate(%d); instead of.", $2);
  yywarning(msg);
}
rotate_in_step_error: ROTATE '(' INTEGER ')' ';'
{
  yywarning("Syntax has changed\n\\rotate can no longer be found inside a step block");
}
;

turn_over_vertical_error: TURN_VERTICAL ';'
{
  yywarning("Syntax has changed\n\\turn_over_vertical can no longer be found inside a step block");
  read_turn_over(true);
}
;

turn_over_horizontal_error: TURN_HORIZONTAL ';'
{
  yywarning("Syntax has changed\n\\turn_over_horizontal can no longer be found inside a step block");
  read_turn_over(false);
}
;


%%
/*--------------------------------------------------------------------*/
/* Maintenant c'est du C 					      */

/**
 ** Traitement des erreurs lors du parsing
 **/
void yyerror(char* s)
{
#ifdef WIN32
  // Message d'erreur style visual
  fprintf(stderr, "%s(%d): error %s\n", file_name.c_str(), line_num, s);
#else // WIN32
  // Message d'erreur style unix
  fprintf(stderr, "%s:%d: error %s\n", file_name.c_str(), line_num, s);
#endif // WIN32
  /* exit(2); */
}

void yywarning(char* s)
{
#ifdef WIN32
  // Message d'erreur style visual
  fprintf(stderr, "%s(%d): warning %s\n", file_name.c_str(), line_num, s);
#else // WIN32
  // Message d'erreur style unix
  fprintf(stderr, "%s:%d: warning %s\n", file_name.c_str(), line_num, s);
#endif // WIN32
}
