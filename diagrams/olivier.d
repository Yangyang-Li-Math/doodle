%----------------------------------------
%
% Test for Olivier by Jerome Gout,  V. 1.0
%
%----------------------------------------

\diagram_header {
  \diagram_date(\today);
  \design_date(2000); 
  \diagramer("Jerome Gout");
  \designer("Jerome Gout");
  \title("Probleme ouvert pour Olivier");
  \color_front(white);
  \color_back(0, 100, 100);
  \visible_area_height(50);
}

\step {
  \square(a,b,c,d);
  ab2 = \middle(a,b);
  dc2 = \middle(d,c);
  ab4 = \middle(ab2, b);
  dc4 = \middle(dc2,c);

  \fold(ab2, dc2, 90, 0);
  \fold(ab4, dc4, 90, 0);
 
  \caption("Tu marques le quart d'un carre");
}
 
\step {

  ab1 = \point_to_line(d, dc4, [b,c], [a,b], second);

  \valley_fold(ab1, dc4);
  
  X = \symmetry(d, [ab1, dc4]);
  \simple_arrow(d, X, none, valley, left);
  \caption("Tu amenes le coin inferieur gauche sur le");
  \caption("cote droit du carre");
}

\step {
  \cut([a,b], ab1);
  \cut([d, c], dc4);
  \move(d, X);
  \move(a, [ab1, dc4]);
  \border(ab1, b, [a, d]);
  \border(ab1, dc4);
  \hide(ab2, ab4);
  \fill(back, a, ab1, dc4, d);
  \caption("Ce qui te donne ca ....");

}
