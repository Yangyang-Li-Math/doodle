/*  -*-c++-*- tells to emacs that is a c++ file
 * 
 *  File     : ps_prologue.cpp
 *  Author   : Jerome Gout
 *  Creation : Fri Sep 28 2001
 *  Revision : $Id$
 * 
 *  Copyright (C) 2000, 2001 Doodle
 */

#include <iostream>
#include "global_def.h"

void ps_prologue(std::ostream & out)
{
  out << "\
/cm {72 mul 2.54 div} bind def\n\
/mm {72 mul 25.4 div} bind def\n\
/interline {1 mm} def\n\n\
/width_height {  % r string width_height w h\n\
  /s exch def\n\
  /r exch def\n\
  gsave \n\
  r neg rotate newpath 0 0 moveto\n\
  s false charpath pathbbox\n\
  4 2 roll pop pop grestore\n\
} def\n\
\n\
/turn_over { % x y turn_over -\n\
  /y exch def\n\
  /x exch def\n\
  1 setlinewidth\n\
  newpath \n\
  x y 0.3 cm 0 360 arc stroke\n\
  x y 1.5 cm sub 1.8 cm 65 115 arc stroke\n\
  gsave\n\
    x y 1.5 cm sub translate\n\
    -25 rotate\n\
    0 1.8 cm translate newpath\n\
    0 0 moveto 0 0.15 cm rlineto 0.15 cm -0.15 cm rlineto\n\
    -0.15 cm -0.15 cm rlineto closepath fill\n\
 grestore % end turn\n\
} def\n\n"
      << //- We cut this const string because Microsoft Visual can handle longer than 2048 chars
"\
/rotate_step { % x y s bool rotate_step -\n\
  /positive exch def\n\
  /s exch def\n\
  /y exch def\n\
  /x exch def\n\
  1 setlinewidth\n\
  newpath \n\
  x y 0.5 cm 0 360 arc stroke\n\
  newpath\n\
  positive {\n\
    x y 5 mm sub moveto\n\
  } { \n\
    x y 5 mm add moveto\n\
  } ifelse\n\
  0 0.15 cm rlineto 0.15 cm -0.15 cm rlineto\n\
  -0.15 cm -0.15 cm rlineto closepath fill\n\
  positive {\n\
    x y 5 mm add moveto\n\
  } { \n\
    x y 5 mm sub moveto\n\
  } ifelse\n\
  0 0.15 cm rlineto -0.15 cm -0.15 cm rlineto\n\
  0.15 cm -0.15 cm rlineto closepath fill\n\
  /Helvetica findfont 11 scalefont setfont\n\
  0 s width_height /H exch def /W exch def\n\
  x W 2 div sub y H 2 div sub moveto s show\n\
} def\n\n\
/draw_symbol { % x y s r string angle draw_symbol -\n\
  /angle exch def\n\
  /s exch def\n\
  /r exch def\n\
  /sc exch def\n\
  /y exch def\n\
  /x exch def\n\
  gsave\n\
    r matrix rotate /M exch def % M is the current rotation matrix\n\
    r s width_height /H exch def /W exch def % store string width and height\n\
    x y M transform /y1 exch def /x1 exch def  % new point ...\n\
    0 setlinewidth % set line thickness \n\
    [] 0 setdash\n\
    r neg rotate\n\
    x1 y1 translate 0 0 moveto\n\
    x 0 ne y 0 ne or { % x != 0 OR y != 0\n\
      y x atan angle add /tetha exch def\n\
    } { \n\
      angle /tetha exch def\n\
    } ifelse\n\
    tetha rotate\n\
    1 sc div 1 sc div scale\n\
    5 mm 0 rlineto \n\
    currentpoint /y2 exch def /x2 exch def stroke\n\
    x2 y2 translate 0 0 moveto\n\
    tetha neg rotate\n\
    tetha 270 ge tetha 360 lt and  { % 270 <= a < 360\n\
      0 H neg moveto\n\
    } {\n\
      tetha 90 ge tetha 180 lt and  { % 90 <= a < 180\n\
	W neg 0 moveto\n\
      } { % 180 <= a < 270\n\
	tetha 180 ge tetha 270 lt and {\n\
	  W neg H neg moveto\n\
	} if\n\
      } ifelse\n\
    } ifelse\n\
    s show  \n\
  grestore\n\
} def\n\n"
      << //- We cut this const string because Microsoft Visual can handle longer than 2048 chars
"\
/draw_symbol_at_middle { % x1 y1 x2 y2 s r string angle draw_symbol_at_middle -\n\
  /a exch def\n\
  /s exch def\n\
  /r exch def\n\
  /sc exch def\n\
  /y2 exch def\n\
  /x2 exch def\n\
  /y1 exch def\n\
  /x1 exch def\n\
  x1 x2 add 2 div  % midX\n\
  y1 y2 add 2 div  % midY\n\
  sc r s a draw_symbol\n\
} def\n\n\
/add_shift { % x y dx dy rot scale add_shift x' y'\n  \
   /s exch def\n\
   /r exch def\n\
   mm /dy exch def\n\
   mm /dx exch def\n\
   cm /y exch def\n\
   cm /x exch def\n\
   dx dy r matrix rotate itransform exch s div exch s div exch x add exch y add\n\
} def\n\n\
/intersection { % v1x v1y v2x v2y e1x e1y e2x e2y intersection x y\n  \
  /e2y exch def\n  \
  /e2x exch def\n  \
  /e1y exch def\n  \
  /e1x exch def\n  \
  /v2y exch def\n  \
  /v2x exch def\n  \
  /v1y exch def\n  \
  /v1x exch def\n  \
  /a1 v1y v2y sub def\n  \
  /b1 v2x v1x sub def\n  \
  /c1 a1 v1x mul b1 v1y mul add def\n  \
  /a2 e1y e2y sub def\n  \
  /b2 e2x e1x sub def\n  \
  /c2 a2 e2x mul b2 e2y mul add def\n  \
  /det a1 b2 mul a2 b1 mul sub def\n  \
  0 det ne {\n  \
    c1 b2 mul b1 c2 mul sub det div % put x on stack\n  \
    a1 c2 mul a2 c1 mul sub det div % put y on stack\n  \
  } {\n  \
    0 0 % det == 0 x=0, y=0 to avoid stack underflow\n  \
  } ifelse\n  \
} def\n\n\
/make_first_page_header {\n\
  /r_margin exch def\n\
  /l_margin exch def\n\
  /comment3 exch def\n\
  /comment2 exch def\n\
  /comment1 exch def\n\
  /diagram exch def\n\
  /design exch def\n\
  /title exch def\n\
  gsave % begin first page header\n\
  /Helvetica-Bold findfont 24 scalefont setfont\n\
  0 title width_height /title_h exch def /title_w exch def\n\
  /y " << PAGE_HEIGHT << " mm title_h sub 15 mm sub def\n\
  " << PAGE_WIDTH << " mm title_w sub 2 div y moveto\n\
  title show\n\
  /Times-Roman findfont 12 scalefont setfont\n\
  0 diagram width_height /diagram_h exch def /diagram_w exch def\n\
  /y y 1 cm sub diagram_h sub def\n\
  l_margin y moveto design show\n\
  " << PAGE_WIDTH << " mm r_margin sub diagram_w sub y moveto\n\
  diagram show\n\
  /Times-Roman findfont 9 scalefont setfont\n\
  /comment_hspace 0 (B) width_height exch pop def\n\
  /y y 4 mm sub comment_hspace sub def\n\
  /l_margin l_margin 5 mm add def\n\
  l_margin y moveto comment1 show\n\
  /y y 1 mm sub comment_hspace sub def\n\
  l_margin y moveto comment2 show\n\
  /y y 1 mm sub comment_hspace sub def\n\
  l_margin y moveto comment3 show\n\
  grestore % end first page header\n\
} def\n\n\
/push_arrow { % x y alpha distance angle scale push_arrow - \n\
  /sc exch def\n\
  /angle exch def\n\
  /d exch def\n\
  /alpha exch def\n\
  /y exch def\n\
  /x exch def\n\
  gsave\n\
    0 0 0 setrgbcolor\n\
    0 setlinewidth % set line thickness \n\
    [] 0 setdash\n\
    x y translate\n\
    alpha angle sub rotate\n\
    1 sc div 1 sc div scale\n\
    d mm 0 translate 0 0 moveto\n\
    6 0.8660254 mul mm 6 0.5 mul mm lineto\n\
    6 0.6 mul mm 0 lineto\n\
    6 0.8660254 mul mm 6 0.5 mul neg mm lineto\n\
    closepath fill\n\
  grestore\n\
} def\n\n"
      << //- We cut this const string because Microsoft Visual can handle longer than 2048 chars
"\
/open_arrow { % x y alpha scale open_arrow - \n\
  /sc exch def\n\
  /alpha exch def\n\
  /y exch def\n\
  /x exch def\n\
  gsave\n\
    1 setlinewidth % set line thickness \n\
    [] 0 setdash\n\
    x y translate\n\
    alpha rotate\n\
    1 sc div 1 sc div scale\n\
    0 2 mm moveto\n\
    2.5 mm 1.75 mm rlineto 0 -2 mm rlineto \n\
    6 mm 1 mm rlineto -1.5 mm -2.75 mm rlineto\n\
    1.5 mm -2.75 mm rlineto -6 mm 1 mm rlineto\n\
    0 -2 mm rlineto -2.5 mm 1.75 mm rlineto \n\
    closepath\n\
    gsave\n\
      1 1 1 setrgbcolor\n\
      fill\n\
    grestore\n\
    0 0 0 setrgbcolor\n\
    stroke\n\
  grestore\n\
} def\n\n\
/repeat_arrow { % x y nb s alpha distance angle scale push_arrow - \n\
  /sc exch def\n\
  /angle exch def\n\
  /d exch def\n\
  /alpha exch def\n\
  /s exch def\n\
  /nb exch def\n\
  /y exch def\n\
  /x exch def\n\
  gsave\n\
    /Helvetica-Bold findfont 12 scalefont setfont\n\
    angle s width_height 2 mm add /H exch def 2 mm add /W exch def\n\
    0 0 0 setrgbcolor\n\
    1 setlinewidth % set line thickness \n\
    [] 0 setdash\n\
    x y translate\n\
    alpha angle sub rotate\n\
    1 sc div 1 sc div scale\n\
    gsave\n\
      d mm 0 translate 0 0 moveto\n\
      2.5 mm 2 mm lineto 0 0 moveto\n\
      2.5 mm -2 mm lineto 0 0 moveto\n\
      nb 1 sub 1.5 mul 8 add mm 0 lineto\n\
      5 1.5 nb 1 sub 1.5 mul 5 add {\n\
	mm 2.5 mm moveto 0 -5 mm rlineto\n\
      } for stroke\n\
    grestore\n\
    s () ne {\n\
      nb 1 sub 1.5 mul 8 add mm 0 translate\n\
      alpha neg rotate  0 0  moveto\n\
      alpha 270 ge alpha 360 lt and  { % 270 <= a < 360\n\
	0 H neg moveto \n\
      } {\n\
	alpha 90 ge alpha 180 lt and  { % 90 <= a < 180\n\
	  W neg 0 moveto\n\
	} { % 180 <= a < 270\n\
	  alpha 180 ge alpha 270 lt and {\n\
	    W neg H neg moveto\n\
	  } if\n\
	} ifelse\n\
      } ifelse  \n\
      gsave \n\
	1 mm 1 mm rmoveto\n\
	s show\n\
      grestore\n\
      0 H rlineto W 0 rlineto 0 H neg rlineto closepath stroke\n\
    } if\n\
  grestore\n\
} def\n\
";
}
