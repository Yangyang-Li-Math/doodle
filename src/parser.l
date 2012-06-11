C	[A-Z_a-z]
D	[0-9\-]
A	[0-9A-Z_a-z]
WC      [ \t\n]*\:
F       [\-]?[0-9]*\.[0-9]+

  #ifdef WIN32
  #pragma warning (disable : 4786)
  #endif // WIN32
  #include <string>        //- used in parser.tab.h
  #include <vector>        //- used in parser.tab.h
  #include <map>        //- used in parser.tab.h
  #include "diag_header.h" //- used in parser.tab.h
  #include "date.h"        //- used in parser.tab.h
  #include "parser.tab.h"
  #include <stack>
  #include "context.h"
  #include "macro.h"

  #define INITIAL 0
  #ifdef __cplusplus
  extern "C" int isatty (int );
  #endif

  extern int line_num; //- defined in parser.y
  extern std::string file_name;
  extern void yyerror(char*);  //- defined in parser.y
  static std::stack<context> contexts;
  static int _nbmacro = 0;
  typedef std::map<std::string,macro> macromap;
  extern macromap macros;
  static std::string current_macro;
  static std::vector<std::string> current_params;
  static std::string current_include;

	void new_param() {
		current_params.push_back(std::string());
	}
	void add_to_params(char c) {
		if (current_params.empty()) new_param();
		current_params.back()+=c;
	}
	void switch_to_macro() {
		BEGIN(INITIAL);
		macromap::iterator it = macros.find(current_macro);
		it->second.set(current_params);
		contexts.push(
			context(file_name,line_num,YY_CURRENT_BUFFER)
		);
		// file_name="Macro"; line_num=1;
		std::string s = it->second.get();
		extern bool verbose;
		if (verbose)
			std::cout << "switch to " << it->first << " : "  << s << std::endl;
		yy_switch_to_buffer(
			yy_scan_string(s.c_str())
		);
		current_params.clear();
	}
	void switch_to_include() {
		BEGIN(INITIAL);
		// open file
		yyin = fopen(current_include.c_str(),"r");
		if (!yyin) {
			yyerror("Could not open include file");
			return;
		}
		contexts.push(
			context(file_name,line_num,YY_CURRENT_BUFFER)
		);
		file_name=current_include.c_str(); line_num=1;
		yy_switch_to_buffer(
			yy_create_buffer(yyin, YY_BUF_SIZE)
		);
	}

%option noyywrap

%x XSTRING
%x XMACRODEF
%x XMACROUSE
%x XMACROPAR
%x XINCLUDE

%%
  char string_buf[1000];
  char *string_buf_ptr = 0;

[ \t]	;
\n      line_num++;

"\\diagram_header"    	   {yylval.ival = HEADER; return HEADER;}
"\\design\_date"      	   {yylval.ival = DESIGN_DATE; return DESIGN_DATE;}
"\\diagram\_date"     	   {yylval.ival = DIAG_DATE; return DIAG_DATE;}
"\\designer"          	   {yylval.ival = DESIGNER; return DESIGNER;}
"\\diagramer"         	   {yylval.ival = DIAGRAMER; return DIAGRAMER;}
"\\diagrammer"         	   {yylval.ival = DIAGRAMMER; return DIAGRAMMER;}
"\\title"             	   {yylval.ival = TITLE; return TITLE;}
"\\today"             	   {yylval.ival = TODAY; return TODAY;}
"\\turn\_over\_vertical"   {yylval.ival = TURN_VERTICAL; return TURN_VERTICAL;}
"\\turn\_over\_horizontal" {yylval.ival = TURN_HORIZONTAL; return TURN_HORIZONTAL;}
"\\step"              	   {yylval.ival = STEP; return STEP;}
"\\rotate"            	   {yylval.ival = ROTATE; return ROTATE;}
"\\square"            	   {yylval.ival = SQUARE; return SQUARE;}
"\\diamond"           	   {yylval.ival = DIAMOND; return DIAMOND;}
"\\middle"            	   {yylval.ival = MIDDLE; return MIDDLE;}
"\\fraction"          	   {yylval.ival = FRACT; return FRACT;}
"\\caption"           	   {yylval.ival = CAPTION; return CAPTION;}
"\\valley\_fold"      	   {yylval.ival = VALLEY_FOLD; return VALLEY_FOLD;}
"\\mountain\_fold"    	   {yylval.ival = MOUNTAIN_FOLD; return MOUNTAIN_FOLD;}
"\\point\_to\_point"  	   {yylval.ival = POINT_TO_POINT; return POINT_TO_POINT;}
"\\point\_to\_line"   	   {yylval.ival = POINT_TO_LINE; return POINT_TO_LINE;}
"\\line\_to\_line"    	   {yylval.ival = LINE_TO_LINE; return LINE_TO_LINE;}
"\\simple\_arrow"          {yylval.ival = SIMPLE_ARROW; return SIMPLE_ARROW;}
"\\return\_arrow"          {yylval.ival = RETURN_ARROW; return RETURN_ARROW;}
"\\hide"              	   {yylval.ival = HIDE; return HIDE;}
"\\show"              	   {yylval.ival = SHOW; return SHOW;}
"\\border"            	   {yylval.ival = BORDER; return BORDER;}
"\\fold"                   {yylval.ival = FOLD; return FOLD;}
"\\intersection"      	   {yylval.ival = INTERSECTION; return INTERSECTION;} 
"\\symmetry"               {yylval.ival = SYMMETRY; return SYMMETRY ;} 
"\\cut"                    {yylval.ival = CUT; return CUT ;} 
"\\debug"                  {yylval.ival = DEBUG_INST; return DEBUG_INST ;} 
"\\debug\_point"           {yylval.ival = DEBUG_POINT_INST; return DEBUG_POINT_INST ;} 
"\\debug\_line"            {yylval.ival = DEBUG_LINE_INST; return DEBUG_LINE_INST ;} 
"\\xray\_fold"             {yylval.ival = XRAY_FOLD; return XRAY_FOLD ;} 
"\\visible\_area\_height"  {yylval.ival = VISIBLE_HEIGHT; return VISIBLE_HEIGHT ;}
"\\visible\_area\_width"   {yylval.ival = VISIBLE_WIDTH; return VISIBLE_WIDTH ;}
"\\visible\_area\_center"  {yylval.ival = VISIBLE_CENTER; return VISIBLE_CENTER ;}
"\\scale"                  {yylval.ival = SCALE; return SCALE ;}
"\\left\_margin"           {yylval.ival = _LEFT_MARGIN; return _LEFT_MARGIN ;}
"\\right\_margin"          {yylval.ival = _RIGHT_MARGIN; return _RIGHT_MARGIN ;}
"\\top\_margin"            {yylval.ival = _TOP_MARGIN; return _TOP_MARGIN ;}
"\\bottom\_margin"         {yylval.ival = _BOTTOM_MARGIN; return _BOTTOM_MARGIN ;}
"\\vertical\_space"        {yylval.ival = _VSPACE; return _VSPACE ;}
"\\horizontal\_space"      {yylval.ival = _HSPACE; return _HSPACE ;}
"\\color\_front"           {yylval.ival = COLOR_FRONT; return COLOR_FRONT ;}
"\\color\_back"            {yylval.ival = COLOR_BACK; return COLOR_BACK ;}
"\\fill"                   {yylval.ival = FILL; return FILL;}
"\\unfill"                 {yylval.ival = UNFILL; return UNFILL;}
"\\debug\_info"            {yylval.ival = DEBUG_INFO; return DEBUG_INFO;}
"\\comment"                {yylval.ival = COMMENT; return COMMENT;}
"\\vertical\_rectangle"    {yylval.ival = V_RECTANGLE; return V_RECTANGLE;}
"\\horizontal\_rectangle"  {yylval.ival = H_RECTANGLE; return H_RECTANGLE;}
"\\clip"                   {yylval.ival = CLIP; return CLIP;}
"\\unclip"                 {yylval.ival = UNCLIP; return UNCLIP;}
"\\perpendicular"          {yylval.ival = PERPENDICULAR; return PERPENDICULAR;}
"\\parallel"               {yylval.ival = PARALLEL; return PARALLEL;}
"\\move"                   {yylval.ival = MOVE; return MOVE;}
"\\inter\_cut"             {yylval.ival = INTER_CUT; return INTER_CUT;}
"\\space\_fold"            {yylval.ival = SPACE_FOLD; return SPACE_FOLD;}
"\\eps"                    {yylval.ival = EPS; return EPS;}
"\\reset"                  {yylval.ival = RESET; return RESET;}
"\\shift"                  {yylval.ival = SHIFT; return SHIFT;}
"\\darker"                 {yylval.ival = DARKER; return DARKER;}
"\\lighter"                {yylval.ival = LIGHTER; return LIGHTER;}
"\\define"                 {BEGIN(XMACRODEF); _nbmacro=0; yylval.ival = MACRO; return MACRO;}
"\\include"                {BEGIN(XINCLUDE); }
"\\text"                   {yylval.ival = TEXT; return TEXT;}
"\\unshift"                {yylval.ival = UNSHIFT; return UNSHIFT;}
"\\rabbit\_ear"            {yylval.ival = RABBIT_EAR; return RABBIT_EAR;}
"\\open\_arrow"            {yylval.ival = OPEN_ARROW; return OPEN_ARROW;}
"\\push\_arrow"            {yylval.ival = PUSH_ARROW; return PUSH_ARROW;}
"\\repeat\_arrow"          {yylval.ival = REPEAT_ARROW; return REPEAT_ARROW;}
"\\label"                  {yylval.ival = LABEL; return LABEL;}

\\{C}+{A}*  {
	macromap::iterator it = macros.find(yytext);
	if (it==macros.end()) {
		yylval.ival = OPERATOR;
		return OPERATOR;
	}
	BEGIN(XMACROUSE);
	_nbmacro=0;
	current_macro=yytext;
  }

{C}+{A}* {yylval.String = new std::string(yytext);
           return(SYMBOL); }

{D}+ { yylval.ival = atoi(yytext); return(INTEGER); }

{F}  { yylval.fval = atof(yytext); return(FLOAT); }


\"      string_buf_ptr = string_buf; BEGIN(XSTRING);

<XSTRING>{
\" { /* saw closing quote - all done */
    BEGIN(INITIAL);
    *string_buf_ptr = '\0';
    yylval.String = new std::string(string_buf); return(STRING);
 }
\n { yyerror("Unterminated string."); }

[^\n\"]+ {
    char *yptr = yytext;
    while(*yptr) {
      if(*yptr == '\\') *string_buf_ptr++ = '\\';
      *string_buf_ptr++ = *yptr++;
    }
 }
}

\% { // comments
	register int c;
	while ((c=yyinput())!='\n' && c!=EOF); // eat comments
	unput(c);
}

<XMACRODEF>{
\\{C}+{A}*		{yylval.String = new std::string(yytext); return OPERATOR;}

{C}+{A}*		{yylval.String = new std::string(yytext); return(SYMBOL); }

\{				{yylval.ival=yytext[0]; _nbmacro++; if (_nbmacro==1) return yytext[0]; else return MACROTEXT; }

\}				{yylval.ival=yytext[0]; _nbmacro--; if (_nbmacro<=0) { BEGIN(INITIAL); return yytext[0]; } return MACROTEXT; }

[\,\(\)]		{yylval.ival=yytext[0]; if (_nbmacro==0) return yytext[0]; else return MACROTEXT; }
[ \t]			{yylval.ival=yytext[0]; if (_nbmacro>0) return MACROTEXT; }
\n				{line_num++;
				 // if (_nbmacro>0) return MACROTEXT; on compte les lignes mais on n'ajoute pas à la macro
				}

.				{yylval.ival=yytext[0]; return MACROTEXT;}

}

<XMACROUSE>{
\(				{_nbmacro++; BEGIN(XMACROPAR); }

[ \t]			;
\n				line_num++;

.				{unput(yytext[0]);switch_to_macro();}

}

<XMACROPAR>{
\)				{_nbmacro--; if (_nbmacro==0) switch_to_macro(); }

\(				{_nbmacro++; add_to_params(yytext[0]); }

\,				{if (_nbmacro==1) new_param(); else add_to_params(yytext[0]); }

[ \t]			;
\n				line_num++;

.				{add_to_params(yytext[0]);}

}

<XINCLUDE>{
\)				{ switch_to_include(); }

\(				{ current_include.erase(); }

[ \t\"]			;
\n				line_num++;

[^\t\n\"\(\)]+	{current_include = yytext; }

}

[\,\{\:\}\;\(\)\[\]\=] {return yytext[0];}

<<EOF>> {
	if (contexts.empty()) {
		yyterminate();
	}
	else {
		yy_delete_buffer(YY_CURRENT_BUFFER);
		context c = contexts.top();
		contexts.pop();
		yy_switch_to_buffer(
			c.getBuffer()
		);
		file_name=c.getName();
		line_num=c.getLine();
	}
  }

%%
