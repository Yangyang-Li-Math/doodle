%----------------------------------------
%
%  Tangram by Jerome Gout,  V. 1.0
%
%----------------------------------------

\diagram_header {
  \diagram_date(2000);
  \design_date(03, 02, 2000); 
  \diagramer("Jerome Gout");
  \designer("Jerome Gout");
  \title("Tangram parallelogram");
}

\step {
  \square(a,b,c,d);
  ab2 = \middle(a,b);
  cd2 = \middle(c,d);
  \valley_fold(ab2, cd2, 0, 80);
  bc2 = \middle(b,c);
  \simple_arrow(bc2, [ab2,cd2], unfold, valley, left);
}

\step {
  [bc1,ad3] = \point_to_point(c, ab2, [b,c],[a,d]);
  \cut([b,c], bc1);
  \cut([a,d], ad3);
  \valley_fold(bc1, ad3);
  \simple_arrow(c, ab2, none , valley, right);
}

\step {
  \hide(c,d, cd2);
  sd = \symmetry(d,[bc1,ad3]);
  ad1 = \intersection(sd,ab2,a,d);
  \cut([a,ad3], ad1);
  \border(bc1,ad3);
  \border(ad3, sd);
  \border(sd, ab2);
  \border(ab2, bc1);
  \hide([ad1, ad3]);  
  \hide([bc1, c]);
  \turn_over_vertical;
}

\step {
 \cut([sd, ab2], ad1);
 \hide([ab2,bc1]);
 \hide([ab2,ad1]);
 \show([ab2,cd2]);
 \show([ad1, ad3]);
 [bbc1, aad1] = \point_to_point(a, ad1, [b, bc1], [a, ad1]);
 \valley_fold(bbc1, aad1);
 \simple_arrow(a, ad1,unfold, valley, right);
}

\step {
  [bbbc1, aaad1] = \line_to_line([bbc1, aad1], [a, b], [a,d], [b,c]);
  \valley_fold(bbbc1, aaad1);
  \simple_arrow(ab2, [bbbc1, aaad1], unfold, valley, right);
}

\step {
 ab3 = \point_to_line(a, aad1, [aad1,bbc1], [a, b]);
 \valley_fold(ab3, aad1);
 \simple_arrow(a, [ab3, aad1], unfold, valley, right);
 \turn_over_vertical;
}

\step {
  \show(ab2);
  \hide(cd2);
  \hide([ad3, ad1]);
  bbc1_add1_int2 = \intersection(bbc1, aad1, bc1, ab2);
  bbc1_aad1_int1 = \intersection(bbc1, aad1, sd, ab2);
  \cut([aad1, bbc1], bbc1_aad1_int1);
  \cut([bbc1_aad1_int1, bbc1], bbc1_add1_int2);
  \hide([bbc1_aad1_int1, bbc1_add1_int2]);
  \simple_arrow(bbc1_aad1_int1, [bc1,ad3], none , unfold, right);
}

\step {
  \hide(sd, bc1);
  \hide([ad1, ab2]);
  \hide(bbc1_aad1_int1, bbc1_add1_int2);
  \show(d,c);
  \show([ad3, ad1]);
  \show([ab2, cd2]);
  \show([bc1, b]);
  \fold(aad1, bbc1);
  \fold(ad3, bc1);

  [aad2, bbc2] = \line_to_line([d,c], [aad1, bbc1], [a,d], [b,c]); 
  \valley_fold(aad2, bbc2);
  \simple_arrow(cd2, [aad2, bbc2], unfold, valley, right);
}

\step {

}
