\diagram_header { 
  \designer("Valerie Vann");
  \title("Magic Rose Cube");
  \diagramer("Jerome Gout");
  \diagram_date(2000);
  \comment("This diagram has been made for a personnal usage only. Do not distribute this diagram as Valerie's request.");
  \comment("Use 6 square, 3 for leaves (green) and 3 for rose petals (rose, yellow or red).");
  \comment("Difficulty : Simple - for a 10cm initial squares you'll get a ~4cm cube - Color behind");
  \bottom_margin(15);
  \color_front(white);
  \color_back(0, 81, 0);
}

\visible_area_height(60);
%\scale(80);

\step {
  \caption("Begin with leaves.");
  \caption("Just pinch half");

  \square(a,b,c,d);

  [ab2, cd2] = \line_to_line([b,c], [a,d], [a,b], [c,d]);
  o = \middle(ab2,cd2);
  [bc2, ad2] = \line_to_line([a,b], [c,d], [b,c], [a,d]);
  \valley_fold(ab2, o, 0, 75);
  \valley_fold(cd2, o, 0, 75);
  \simple_arrow(bc2, ad2, unfold, valley, right);
}

\step {
  % to avoid the implicit extremities computation
  % (only 5% by default are left)
  \space_fold([ab2, o], 5, 75);
  \space_fold([cd2, o], 5, 75);
  \caption("Fold unfold vertical quarters.");
  [ab1, cd1] = \line_to_line([a,d], [ab2, o], [a,b], [c,d]);
  [ab3, cd3] = \line_to_line([b,c], [ab2, o], [a,b], [c,d]);

  \valley_fold(ab1, cd1);
  \simple_arrow(ad2, [ab1, cd1], unfold, valley, left);
  \valley_fold(ab3, cd3);
  \simple_arrow(bc2, [ab3, cd3], unfold, valley, right);
}

\step {
 \caption("Fold unfold right top bissector");
 bc1 = \line_to_line(ab1, cd1, b,  [b,c]);
 \valley_fold(bc1, ab1);
 \simple_arrow(b, [bc1, ab1], unfold, valley, right);
}

\step {
 \caption("Fold left bottom bissector");

 ad1 = \line_to_line(cd3,d, ab3, [a,d]);
 \valley_fold(ad1, cd3);
 \simple_arrow(d, [ad1, cd3], none, valley, left);
 i2 = \inter_cut([ab1, cd1], [ad1, cd3]);
\debug;
}

\step {
 \caption("Fold corner using horizontal crease.");
 \cut([c,d], cd3);
 \cut([d,a], ad1);
 \move(d, [ad1, cd3]);
 \move(cd1, [ad1, cd3]);
 \fold(i2, ab1, [ad1, d], 5);
% \space_fold([o, cd2], 0, 5); % uncomment to avoid warning
 i3 = \inter_cut([o, cd2], [ad1, cd3]);
 \hide([o, i3]);
 \space_fold([i3, cd2], 65, 5);
 \move(cd2, [ad1, cd3]);
 % i4 and i5 are and the bissector made in step 3
 i4 = \inter_cut([ab1, bc1], [ad1, d]);
 % i5 is equal to cd1 but on the top layer
 i5 = \inter_cut([i4, bc1], [cd3, d]);
 \hide([i4, i5]);
 \cut([d, cd3], cd1);
 \border(ad1, cd3);
 
 \fill(back, d, ad1, cd3);

 \valley_fold(i4, cd1);
 \simple_arrow(d, o, none, valley, right);
\debug;
}

\step {
 \caption("Swivel fold");
 \unfill(d, ad1, cd3);
 \fill(back, ad1, i4, o, cd1, cd3);
 % ad3 is equal to i4 but on the top layer
% ad3 = \inter_cut([ad1, d], [i4, i5]);
 \cut([ad1, d], i4);
% \border(ad3, cd1);
 \border(i4, cd1);
 \fill(front, i4, d, cd1);
 \move(d, o);

 \valley_fold(i2, cd1, -20, -30);

 i1 = \inter_cut([ab1, i2], [ad1, i4]);
 \valley_fold(ab1, i1);
 \xray_fold(i1,i2);
 \mountain_fold(i2, i4);
 \simple_arrow(ad1, i4, none, valley, left);
 \simple_arrow(i4, i3, none, valley, right);
 \cut([ad1, cd3], i2);
 \cut([a, b], ab1);
 \cut([ad1, i4], i1);
\debug;
}

\step {
 \unfill(ad1, i4, o, cd1, cd3);
 \unfill(i4, d, cd1);
 \move(a, ab2);
 \move(ad1, i4);
 \move(i4, i3); 
 \move(i1, d);
 \hide([ab1, i4]);
 \hide([i1, ab1]);
 \border(ab1, i2);
 \fold(i2, ad1);
 \fold(ad1, i5, 0, 0);
 \cut([i2, cd1], d);
 \hide([i2, d]);
 \hide([i1, i2]);
 \fill(back, ab1, a, i4, i3, i2);
 \fill(front, d, cd1, i4);
 \fill(back, i5, cd3, i4);

 \valley_fold(cd3, bc1, -20, -20);
 \simple_arrow(c, cd2, none, valley, right);
 \caption("Valley fold the right bottom corner");
\debug;
}

\step {
  \cut([c, b], bc1);
  \move(c, cd2);
  \border(cd3, bc1);
  \fill(back, c, cd3, bc1);
  
  \fold(c, bc1); % to avoid that the border line hides valley line
  \valley_fold(i3, bc1, -20, -20);
  \simple_arrow(cd3, i5, none, valley, right);
  \caption("Fold up the lower top.");
  \caption("Fold all the layer.");
  \cut([i2, cd3], i3);
}

\scale(120);

\step {
 \unfill(c, cd3, bc1);
 \move(cd3, i5);
 \border(i3, c);
 \border(c, bc1);
 \fill(back, i5, i3, c);
 \fill(back, i5, c, bc1);

 \xray_fold(ab1, ad1);
 \valley_fold(ad1, i5);
 \mountain_fold(ad1, i5);
 bc3 = \symmetry(i3, [ad1, i5]);
 \valley_fold(i5, bc3);
 \valley_fold(c, cd3);
 \fold(cd2, cd3);
 \simple_arrow(ab3, i2, none, valley, right);
% \simple_arrow(bc3, ad1, none, valley, left);
 \simple_arrow(bc1, i3, none, valley, left);
 \caption("All folds exist expect the right one.");
 \caption("White upper right part should go behind left green layer.");
 \caption("You can open slightly the green upper left layer.");
 \debug;
}

\step {
 \cut([b, bc1], bc3);
 \cut([bc1, i3], c);
 \move(bc1, i3);
 \move(ab3, i2);
 \move(b, a);
 \move(b, [ab1, ad1]);
 \move(ab2, i1);
 \move(bc3, ad1);
 \hide([ab1, ad1]);
 \hide([b, ad1]);
 \hide([ab3, i5]);
 \hide([i1, i5]);

 \debug;
}
