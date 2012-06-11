%------------------------------------------------------------
%
%  Star Box by Jerome Gout
%
%------------------------------------------------------------

\diagram_header {
  \title("Starbox");
  \designer("Dino Andreozzi");
  \diagramer("Jerome Gout");
  \diagram_date(2000);
  \design_date(1, 1, 1997);
}

\step {
  \diamond(a, b, c, d);
  \valley_fold(a,c);
  \simple_arrow(d, b, unfold, valley, right);
  \valley_fold(d,b);
  \simple_arrow(a, c, unfold, valley, right);
  \rotate 45;
}

\step {
  ab2 = \middle(a,b);
  bc2 = \middle(b,c);
  cd2 = \middle(c,d);
  da2 = \middle(d,a);
  \valley_fold(ab2, cd2);
  \simple_arrow(bc2, da2, unfold, valley, right);
  \valley_fold(bc2, da2);
  \simple_arrow(ab2, cd2, unfold, valley, right);
}

\step {
  o = \middle(a,c);

  ab1 = \point_to_line(ab2, o, [a,o], [a,b],first);
  \valley_fold(ab1, o);
  \simple_arrow(ab2, [ab1, o], unfold, valley, left);
  ab3 = \point_to_line(ab2, o, [b,o], [a,b], second);
  \valley_fold(ab3, o);
  \simple_arrow(ab2, [ab3, o], unfold, valley, right);
 
  bc1 = \point_to_line(bc2, o, [b,o], [b,c], first);
  \valley_fold(bc1, o);
  \simple_arrow(bc2, [bc1, o], unfold, valley, left);
  bc3 = \point_to_line(bc2, o, [c,o], [b,c], second);
  \valley_fold(bc3, o);
  \simple_arrow(bc2, [bc3, o], unfold, valley, right);

  cd1 = \point_to_line(cd2, o, [c,o], [c,d], first);
  \valley_fold(cd1, o);
  \simple_arrow(cd2, [cd1, o], unfold, valley, left);
  cd3 = \point_to_line(cd2, o, [d,o], [c,d], second);
  \valley_fold(cd3, o);
  \simple_arrow(cd2, [cd3, o], unfold, valley, right);

  da1 = \point_to_line(da2, o, [d,o], [d,a], first);
  \valley_fold(da1, o);
  \simple_arrow(da2, [da1, o], unfold, valley, left);
  da3 = \point_to_line(da2, o, [a,o], [d,a], second);
  \valley_fold(da3, o);
  \simple_arrow(da2, [da3, o], unfold, valley, right);
}

\step {
  \valley_fold(ab3, bc1);
  \simple_arrow(b, [ab3, bc1], none, valley, right);
  sb = \symmetry(b, [ab3, bc1]);

  \valley_fold(bc3, cd1);
  \simple_arrow(c, [bc3, cd1], none, valley, right);
  sc = \symmetry(c, [bc3, cd1]); 

  \valley_fold(cd3, da1);
  \simple_arrow(d, [cd3, da1], none, valley, right);
  sd = \symmetry(d, [cd3, da1]);
 
  \valley_fold(da3, ab1);
  \simple_arrow(a, [da3, ab1], none, valley, right);
  sa = \symmetry(a, [da3, ab1]);
}

\step {
  \cut([a,b], ab1);
  \cut([ab1, b], ab3);

  \cut([b,c], bc1);
  \cut([bc1,c], bc3);

  \cut([c, d], cd1);
  \cut([cd1, d], cd3);

  \cut([d,a], da1);
  \cut([da1, a], da3);

  \hide(a,b,c,d);
  % troncate the corners
  \border(ab3,bc1);
  \border(bc3,cd1);
  \border(cd3,da1);
  \border(da3,ab1);

  % Make the corners folded
  \border(sa, ab1);
  \border(sa, da3);

  \border(sb, ab3);
  \border(sb, bc1);

  \border(sc, bc3);
  \border(sc, cd1);

  \border(sd, cd3);
  \border(sd, da1);

  % redraw the two diagonals

  \fold(sa, sc);
  \fold(sb, sd);

  \turn_over_vertical;
}

\step {
  sa2 = \middle(da3, ab1);
  sb2 = \middle(ab3, bc1);
  sc2 = \middle(bc3, cd1);
  sd2 = \middle(cd3, da1);

  \hide(sa, sb, sc, sd);
  % redraw diagonals
  \fold(sa2, sc2);
  \fold(sb2, sd2);

  [ab23, sc3] = \point_to_point(bc1, o, [ab1, ab3], [bc3, cd1]);
  \valley_fold(ab23, sc3);
  \simple_arrow(bc1, o, unfold, valley, left);

  [sa1, bc12] = \point_to_point(ab3, o, [ab1, da3], [bc1, bc3]);
  \valley_fold(sa1, bc12);
  \simple_arrow(ab3, o, unfold, valley, left);

  [da32, sb3] = \point_to_point(ab1, o, [da1, da3], [ab3, bc1]);
  \valley_fold(da32, sb3);
  \simple_arrow(ab1, o, unfold, valley, left);

  [ab12, sd1] = \point_to_point(da3, o, [ab1, ab3], [da1, cd3]);
  \valley_fold(ab12, sd1);
  \simple_arrow(da3, o, unfold, valley, left);

  [sa3, cd23] = \point_to_point(da1, o, [ab1, da3], [cd3, cd1]);
  \valley_fold(sa3, cd23);
  \simple_arrow(da1, o, unfold, valley, left);

  [da21, sc1] = \point_to_point(cd3, o, [da1, da3], [bc3, cd1]);
  \valley_fold(da21, sc1);
  \simple_arrow(cd3, o, unfold, valley, left);

  [sd3, bc23] = \point_to_point(cd1, o, [da1, cd3], [bc1, bc3]);
  \valley_fold(sd3, bc23);
  \simple_arrow(cd1, o, unfold, valley, left);

  [sb1, cd12] = \point_to_point(bc3, o, [ab3, bc1], [cd3, cd1]);
  \valley_fold(sb1, cd12);
  \simple_arrow(bc3, o, unfold, valley, left);
}

\step {

}
