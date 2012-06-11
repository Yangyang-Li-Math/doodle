%!PS-Adobe-3.0
%%Creator: Doodle -- http://doodle.sourceforge.net
%%Title: test.ps
%%DocumentPaperSizes: A4
%%Pages:   1
%%EndComments

/cm {72 mul 2.54 div} bind def
/mm {72 mul 25.4 div} bind def
/interline {1 mm} def

/width_height {  % r string width_height w h
  /s exch def
  /r exch def
  gsave 
  r neg rotate newpath 0 0 moveto
  s false charpath pathbbox
  4 2 roll pop pop grestore
} def

/turn_over { % x y turn_over -
  /y exch def
  /x exch def
  1 setlinewidth
  newpath 
  x y 0.3 cm 0 360 arc stroke
  x y 1.5 cm sub 1.8 cm 65 115 arc stroke
  gsave
    x y 1.5 cm sub translate
    -25 rotate
    0 1.8 cm translate newpath
    0 0 moveto 0 0.15 cm rlineto 0.15 cm -0.15 cm rlineto
    -0.15 cm -0.15 cm rlineto closepath fill
 grestore % end turn
} def

/rotate_step { % x y s bool rotate_step -
  /positive exch def
  /s exch def
  /y exch def
  /x exch def
  1 setlinewidth
  newpath 
  x y 0.5 cm 0 360 arc stroke
  newpath
  positive {
    x y 5 mm sub moveto
  } { 
    x y 5 mm add moveto
  } ifelse
  0 0.15 cm rlineto 0.15 cm -0.15 cm rlineto
  -0.15 cm -0.15 cm rlineto closepath fill
  positive {
    x y 5 mm add moveto
  } { 
    x y 5 mm sub moveto
  } ifelse
  0 0.15 cm rlineto -0.15 cm -0.15 cm rlineto
  0.15 cm -0.15 cm rlineto closepath fill
  /Helvetica findfont 11 scalefont setfont
  0 s width_height /H exch def /W exch def
  x W 2 div sub y H 2 div sub moveto s show
} def

/draw_symbol { % x y s r string angle draw_symbol -
  /angle exch def
  /s exch def
  /r exch def
  /sc exch def
  /y exch def
  /x exch def
  gsave
    r s width_height /H exch def /W exch def % store string width and height
    r matrix rotate /M exch def % M is the current rotation matrix
    x y M transform /y1 exch def /x1 exch def  % new point ...
    0 setlinewidth % set line thickness 
    [] 0 setdash
    r neg rotate
    x1 y1 translate 0 0 moveto
    x 0 ne y 0 ne or { % x != 0 OR y != 0
      y x atan angle add /tetha exch def
    } { 
      angle /tetha exch def
    } ifelse
    tetha rotate
    1 sc div 1 sc div scale
    5 mm 0 rlineto 
    currentpoint /y2 exch def /x2 exch def stroke
    x2 y2 translate 0 0 moveto
    tetha neg rotate
    tetha 270 ge tetha 360 lt and  { % 270 <= a < 360
      0 H neg moveto
    } {
      tetha 90 ge tetha 180 lt and  { % 90 <= a < 180
	W neg 0 moveto
      } { % 180 <= a < 270
	tetha 180 ge tetha 270 lt and {
	  W neg H neg moveto
	} if
      } ifelse
    } ifelse
    s show  
  grestore
} def

/draw_symbol_at_middle { % x1 y1 x2 y2 s r string angle draw_symbol_at_middle -
  /a exch def
  /s exch def
  /r exch def
  /sc exch def
  /y2 exch def
  /x2 exch def
  /y1 exch def
  /x1 exch def
  x1 x2 add 2 div  % midX
  y1 y2 add 2 div  % midY
  sc r s a draw_symbol
} def

/add_shift { % x y dx dy rot scale add_shift x' y'
     /s exch def
   /r exch def
   mm /dy exch def
   mm /dx exch def
   cm /y exch def
   cm /x exch def
   dx dy r matrix rotate itransform exch s div exch s div exch x add exch y add
} def

/intersection { % v1x v1y v2x v2y e1x e1y e2x e2y intersection x y
    /e2y exch def
    /e2x exch def
    /e1y exch def
    /e1x exch def
    /v2y exch def
    /v2x exch def
    /v1y exch def
    /v1x exch def
    /a1 v1y v2y sub def
    /b1 v2x v1x sub def
    /c1 a1 v1x mul b1 v1y mul add def
    /a2 e1y e2y sub def
    /b2 e2x e1x sub def
    /c2 a2 e2x mul b2 e2y mul add def
    /det a1 b2 mul a2 b1 mul sub def
    0 det ne {
      c1 b2 mul b1 c2 mul sub det div % put x on stack
      a1 c2 mul a2 c1 mul sub det div % put y on stack
    } {
      0 0 % det == 0 x=0, y=0 to avoid stack underflow
    } ifelse
  } def

/make_first_page_header {
  /r_margin exch def
  /l_margin exch def
  /comment3 exch def
  /comment2 exch def
  /comment1 exch def
  /diagram exch def
  /design exch def
  /title exch def
  gsave % begin first page header
  /Helvetica-Bold findfont 24 scalefont setfont
  0 title width_height /title_h exch def /title_w exch def
  /y 297 mm title_h sub 15 mm sub def
  210 mm title_w sub 2 div y moveto
  title show
  /Times-Roman findfont 12 scalefont setfont
  0 diagram width_height /diagram_h exch def /diagram_w exch def
  /y y 1 cm sub diagram_h sub def
  l_margin y moveto design show
  210 mm r_margin sub diagram_w sub y moveto
  diagram show
  /Times-Roman findfont 9 scalefont setfont
  /comment_hspace 0 (B) width_height exch pop def
  /y y 4 mm sub comment_hspace sub def
  /l_margin l_margin 5 mm add def
  l_margin y moveto comment1 show
  /y y 1 mm sub comment_hspace sub def
  l_margin y moveto comment2 show
  /y y 1 mm sub comment_hspace sub def
  l_margin y moveto comment3 show
  grestore % end first page header
} def

/push_arrow { % x y alpha distance angle scale push_arrow - 
  /sc exch def
  /angle exch def
  /d exch def
  /alpha exch def
  /y exch def
  /x exch def
  gsave
    0 0 0 setrgbcolor
    0 setlinewidth % set line thickness 
    [] 0 setdash
    x y translate
    alpha angle sub rotate
    1 sc div 1 sc div scale
    d mm 0 translate 0 0 moveto
    6 0.8660254 mul mm 6 0.5 mul mm lineto
    6 0.6 mul mm 0 lineto
    6 0.8660254 mul mm 6 0.5 mul neg mm lineto
    closepath fill
  grestore
} def

/open_arrow { % x y alpha scale open_arrow - 
  /sc exch def
  /alpha exch def
  /y exch def
  /x exch def
  gsave
    1 setlinewidth % set line thickness 
    [] 0 setdash
    x y translate
    alpha rotate
    1 sc div 1 sc div scale
    0 2 mm moveto
    2.5 mm 1.75 mm rlineto 0 -2 mm rlineto 
    6 mm 1 mm rlineto -1.5 mm -2.75 mm rlineto
    1.5 mm -2.75 mm rlineto -6 mm 1 mm rlineto
    0 -2 mm rlineto -2.5 mm 1.75 mm rlineto 
    closepath
    gsave
      1 1 1 setrgbcolor
      fill
    grestore
    0 0 0 setrgbcolor
    stroke
  grestore
} def

/repeat_arrow { % x y nb s alpha distance angle scale push_arrow - 
  /sc exch def
  /angle exch def
  /d exch def
  /alpha exch def
  /s exch def
  /nb exch def
  /y exch def
  /x exch def
  gsave
    /Helvetica-Bold findfont 12 scalefont setfont
    angle s width_height 2 mm add /H exch def 2 mm add /W exch def
    0 0 0 setrgbcolor
    1 setlinewidth % set line thickness 
    [] 0 setdash
    x y translate
    alpha angle sub rotate
    1 sc div 1 sc div scale
    gsave
      d mm 0 translate 0 0 moveto
      2.5 mm 2 mm lineto 0 0 moveto
      2.5 mm -2 mm lineto 0 0 moveto
      nb 1 sub 1.5 mul 8 add mm 0 lineto
      5 1.5 nb 1 sub 1.5 mul 5 add {
	mm 2.5 mm moveto 0 -5 mm rlineto
      } for stroke
    grestore
    s () ne {
      nb 1 sub 1.5 mul 8 add mm 0 translate
      alpha neg rotate  0 0  moveto
      alpha 270 ge alpha 360 lt and  { % 270 <= a < 360
	0 H neg moveto 
      } {
	alpha 90 ge alpha 180 lt and  { % 90 <= a < 180
	  W neg 0 moveto
	} { % 180 <= a < 270
	  alpha 180 ge alpha 270 lt and {
	    W neg H neg moveto
	  } if
	} ifelse
      } ifelse  
      gsave 
	1 mm 1 mm rmoveto
	s show
      grestore
      0 H rlineto W 0 rlineto 0 H neg rlineto closepath stroke
    } if
  grestore
} def

%%Page: 1 1
(test)
(Design: ??? - Copyright 1999)
(Diagrams: J. Gout \(helped by X. Fouchet\) - Copyright 2001)
()
()
()20 mm 20 mm make_first_page_header
gsave % begin step
52 mm 200 mm translate
gsave % begin figure
/Times-Roman findfont % Get basic font
12 scalefont          % 12 points font
setfont               % Make it the current font

-0 cm -0 cm translate % recentering the step
1 setlinewidth % set border width
[] 0 setdash   % set solid line for border
-2 cm 2 cm moveto 
2 cm 2 cm lineto stroke 
1 setlinewidth % set border width
[] 0 setdash   % set solid line for border
2 cm 2 cm moveto 
2 cm -2 cm lineto stroke 
1 setlinewidth % set border width
[] 0 setdash   % set solid line for border
2 cm -2 cm moveto 
-2 cm -2 cm lineto stroke 
1 setlinewidth % set border width
[] 0 setdash   % set solid line for border
-2 cm -2 cm moveto 
-2 cm 2 cm lineto stroke 
1 setlinewidth % set valley width
[5 1 div] 0 setdash  % set dashed line for valley fold
0 cm 2 cm moveto 
0 cm -0.763932 cm lineto stroke 
1 setlinewidth % set valley width
[5 1 div] 0 setdash  % set dashed line for valley fold
2 cm -2 cm moveto 
0 cm -0.763932 cm lineto stroke 
1 setlinewidth % set valley width
[5 1 div] 0 setdash  % set dashed line for valley fold
-2 cm -2 cm moveto 
0 cm -0.763932 cm lineto stroke 
1 setlinewidth % set mountain width
[5 1 div 3 1 div 1 1 div 3 1 div 1 1 div 3 1 div] 0 setdash  % set dotted-dashed line for mountain fold
2 cm 0.236068 cm moveto 
0 cm -0.763932 cm lineto stroke 
grestore % end figure
/Times-Roman findfont % Get basic font
11 scalefont          % 11 points font
setfont               % Make it the current font

/hline {0 (Mj) width_height exch pop} def
64 mm -0.4 mul -32 mm interline hline add sub moveto
(1-) show
64 mm -0.3 mul -32 mm interline hline add 1 mul sub moveto
([bc, o] = \\rabbit_ear\(c, ab, d, [b, c]\);) show
% begin rotate
32 mm 32 mm (22.5) true rotate_step
grestore % end step
gsave % begin step
157 mm 200 mm translate
gsave % begin figure
22.5 rotate
/Times-Roman findfont % Get basic font
12 scalefont          % 12 points font
setfont               % Make it the current font

-0 cm -0 cm translate % recentering the step
1 setlinewidth % set border width
[] 0 setdash   % set solid line for border
-2 cm 2 cm moveto 
2 cm 2 cm lineto stroke 
1 setlinewidth % set border width
[] 0 setdash   % set solid line for border
2 cm 2 cm moveto 
2 cm -2 cm lineto stroke 
1 setlinewidth % set border width
[] 0 setdash   % set solid line for border
2 cm -2 cm moveto 
-2 cm -2 cm lineto stroke 
1 setlinewidth % set border width
[] 0 setdash   % set solid line for border
-2 cm -2 cm moveto 
-2 cm 2 cm lineto stroke 
0 setlinewidth % set fold width
[] 0 setdash  % set solid line for existing fold
0 cm 1.8618 cm moveto 
0 cm -0.625735 cm lineto stroke 
0 setlinewidth % set fold width
[] 0 setdash  % set solid line for existing fold
1.9 cm -1.9382 cm moveto 
0.1 cm -0.825735 cm lineto stroke 
0 setlinewidth % set fold width
[] 0 setdash  % set solid line for existing fold
-1.9 cm -1.9382 cm moveto 
-0.1 cm -0.825735 cm lineto stroke 
0 setlinewidth % set fold width
[] 0 setdash  % set solid line for existing fold
1.9 cm 0.186068 cm moveto 
0.1 cm -0.713932 cm lineto stroke 
2 cm 0.236068 cm 
gsave 0 0 0 setrgbcolor % black
1 22.5 (pipo) 31 draw_symbol grestore
grestore % end figure
/Times-Roman findfont % Get basic font
11 scalefont          % 11 points font
setfont               % Make it the current font

/hline {0 (Mj) width_height exch pop} def
64 mm -0.4 mul -32 mm interline hline add sub moveto
(2-) show
64 mm -0.3 mul -32 mm interline hline add 1 mul sub moveto
() show
grestore % end step
gsave % begin page footer
3 setlinewidth [] 0 setdash
20 mm 20 mm moveto 190 mm 20 mm lineto stroke
/Times-Roman findfont 12 scalefont setfont
210 mm 0 (1) width_height
3 1 roll sub 2 div exch 20 mm exch sub 3 mm sub moveto
(1) show
grestore % end page footer
showpage
