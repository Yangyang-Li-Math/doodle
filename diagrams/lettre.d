\diagram_header { 
  \designer("???");
  \title("Lettre enveloppe");
  \diagramer("Jerome Gout");
  \diagram_date(2000);
  \bottom_margin(15);
  \color_front(white);
  \color_back(0, 81, 0);
  \comment("Difficulte : simple; Papier A4");
}

\step {
  \horizontal_rectangle(a,b,c,d,A);

  ab1 = \line_to_line(c, b, d, [a,b]);
  ab3 = \line_to_line(d, a, c, [a,b]);

  \valley_fold(ab1, c);
  cd1 = \symmetry(b, [ab1, c]);
  \valley_fold(ab3, d);
  cd3 = \symmetry(a, [d, ab3]);
  \simple_arrow(b,cd1, unfold, valley, right);
  \simple_arrow(a,cd3, unfold, valley, left);
  \caption("Marquer les deux bissectrices inferieures");
}

\step {
  ab2 = \middle(a,b);
  cd2 = \middle(c,d);
  i1 = \intersection([ab1, c], [d, ab3]);
  a1 = \parallel([a,b], i1, [d,a]);
  b1 = \parallel([a,b], i1, [b,c]);
  \mountain_fold(a1, b1);
  \return_arrow([a1, b1], [a, b], unfold, mountain, right);
  \caption("Marquer le pli parallele au bord superieur");
  \caption("passant par l'intersection des deux plis");
  \caption("precedents. Faire un pli montagne.");
}

\step {
  \mountain_fold(a1, b1);
  \valley_fold(c, ab1);
  \valley_fold(d, ab3);
  \simple_arrow(b1, cd2, none, valley, left);
  \simple_arrow(a1, cd2, none, valley, right);
  \cut([ab1, c], i1);
  \cut([ab3, d], i1);
  \cut([a1, b1], i1);
  \cut([a,b], ab3);
  \cut([a,ab3], ab1);
  \caption("Utiliser les plis formes pour amener");
  \caption("les deux plis montagnes verticalement");
  \caption("dans un mouvement similaire a la base");
  \caption("preliminaire.");
  \debug;
}

\step {
  % Attention, a utilise a1 pour bouger, il faut donc d'abord bouger
  % a puis a1
  \move(a, [i1, a1]);
  \move(a, [i1, d]);  % composition de symetries
  \move(ab1, [i1, a1]);
  \move(a1, [i1, d]);
  % Attention, b utilise b1 pour bouger, il faut donc d'abord bouger
  % b puis b1
  \move(b, [i1, b1]);
  \move(b, [i1, c]);  % composition de symetries
  \move(ab3, [i1, b1]);
  \move(b1, [i1, c]);
  \border(i1, c);
  \border(i1, d);
  i2 = \inter_cut([i1, b1], [ab1, ab3]);
  \hide([i1, i2]);
  \hide([i1, a1]);
  \border(i2, a1);
  \fill(back, i1, ab1, ab3);
  \fill(back, d, ab1, a);
  \fill(back, c, ab3, b);
  \caption("Resultat de l'etape precedente.");
}

\scale(120); % ca commence a etre petit ...

\step {
  a2 = \line_to_line(a1, d, i1, [i1, d]);
  b2 = \line_to_line(b1, c, i1, [i1, c]);

  \valley_fold(a1, a2);
  \simple_arrow(d, [a1, a2], none, valley, right);
  \valley_fold(b1, b2);
  \simple_arrow(c, [b1, b2], none, valley, left);
  \caption("Remonter les pointes laterales a");
  \caption("l'interieur de la pochette superieure."); 
}

\step {
  \scale(150);
  \unfill(i1, ab1, ab3);
  \unfill(d, ab1, a);
  \unfill(c, ab3, b);
  \hide(a,d,b,c);
  \border(a1, a2);
  \border(b1, b2);
  \border(i1, a2);
  \border(i1, b2);
  \fill(back, i1, a2, a1, b2);
  \caption("La lettre est maintenant fermee.");
  \caption("On obtient un jolie enveloppe.");
}
