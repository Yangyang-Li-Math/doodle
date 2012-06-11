%----------------------------------------
%
%   TEST by Jerome Gout,  V. 1.0
%
%----------------------------------------
%\include("macro_date.d");

\define \my_date(annee,mois,jour) {
	mois,jour,annee
}

\define \titre(t) {\title(t)}

%\define \rgb(r, g, b) { 
%   r,g,b
%}

\diagram_header {
  \diagram_date(1999,11,23);
%  \diagram_date(\my_date(1999,11,23));
  \design_date(11, 01, 1999); 
  \diagramer("Jerome Gout et Xavier Fouchet");
  \designer("???");
  \titre("test");
  \color_front(white);
  \color_back(100, 100, 0);
}

\step {
  \diamond(a,b,c,d);
  \valley_fold(d, b);
  \simple_arrow(c,a, none, valley, right);
} 

\step {
  \move(c, a);
  \border(d,b);
  \border(d,c, [a, b]);
  \shift(c, 2, 0);

  o = \middle(b,d);

  \valley_fold(a,o);

  \simple_arrow(d, b, unfold, valley, right);
  \hide(a);
  \debug;
}
