%----------------------------------------
%
% Test for Didier by Jerome Gout,  V. 1.0
%
%----------------------------------------
\define \def { \define }
\def \titre(t) { \title(t) }
\def \rgb(r,g,b) { r,g,b }
\define \moitie(a,b) { \middle(a,b) }

\diagram_header {
  \diagram_date(\today);
  \design_date(2001); 
  \diagramer("Jerome Gout");
  \designer("???");
  \titre("Bang !");
  \color_front(white);
  \color_back(\rgb(0, 100, 100));
  \visible_area_height(70);
}

\step {
  \vertical_rectangle(a,b,c,d, A);
  
  ab = \moitie(a,b);
  cd = \middle(c,d);
  da = \middle(d,a);
  bc = \middle(b,c);

  \valley_fold(ab, cd);
  \simple_arrow(da, bc, none, valley, left);
  \caption("Plier la feuille en deux");
}

\step {
  \cut([a,b], ab);
  \cut([c,d], cd);
  \move(a, b);
  \move(d, c);
  \shift(c, -1, 1);
  \shift(b, -1, 1);
  \hide([cd, c]);
  \border(c, b, [ab, a]);
  \border(ab, cd);
  \fill(back, ab, a, d, cd);

  ab_a = \middle(ab, a);
  cd_d = \middle(cd, d);
  \valley_fold(ab_a, cd_d);
  \simple_arrow(bc, [ab_a, cd_d], none, valley, right);

  \caption("Plier la permiere couche en deux");
}

\step {
  \unfill(ab, a, d, cd);
  \cut([ab, a], ab_a);
  \cut([cd, d], cd_d);
  \move(a, ab);
  \move(d, cd);
  \border(ab_a, cd_d);
  \border(b,c);
  \border(cd, c, [ab_a, cd_d]);
  \shift(a, 0, -1);
  \shift(d, 0, -1);
  \shift(ab, 1, 0);
  \shift(cd, 1, 0);
  \hide([cd , cd_d]);
  \border(cd, ab, [ab_a, a]);
  \fill(back, ab, ab_a, cd_d, cd);
  \fill(front, a, ab_a, cd_d, d);
  \shift(bc, 0, 1);
  \shift(_bc, 0, -1);
  \mountain_fold(bc, _bc, 4, 0);
  \return_arrow([bc, _bc], [a, b], none, mountain, right);

  \caption("Plier en arriere (pli montagne) le modele en deux");
}
