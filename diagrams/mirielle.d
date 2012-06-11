\diagram_header {
  \designer("Mireille Corobu");
  \title("Petit bateau");
  \diagramer("Jerome Gout");
  \diagram_date(2001);
  \comment("Ce diagramme a ete fait en suivant les instructions donnees par Mirielle sur la liste Origami Fr");
  \comment("");
  \comment("Difficulte : Simple - Prendre une feuille A4");
  \bottom_margin(15);
} 


\step {
  \vertical_rectangle(A,B,C,D, A);
  [E, F] =  \line_to_line([A,B], [D, C], [A, D], [B, C]);
  i1 = \middle(A,B); % pour la fleche
  \valley_fold(E, F);
  \simple_arrow(i1, [E, F], none, valley, right);
  \caption("Plier selon la petite mediande.");
}

\step {
  \cut([A, D], E);
  \cut([B, C], F);
  \border(E, F);
  \move(A, D);
  \move(B, C);
  \hide(A,B);
  I = \middle(E, F);
  J = \middle(D, C);
  i2 = \middle(F, C); % pour la fleche
  \valley_fold(I,J);
  \simple_arrow(i2, [I,J], unfold, valley, right);
  \caption("Marquer la petite diagonale");
  \caption("du nouveau rectangle.");
}

\step{
  \cut([E, F], I);
  K = \line_to_line(I, E, J, [E, D]);
  L = \line_to_line(I, F, J, [F, C]);
  \valley_fold(I, K);
  i3 = \symmetry(E, [I, K]);
  \simple_arrow(E, i3, none, valley, left);
  \valley_fold(I, L);
  i4 = \symmetry(F, [I, L]);  
  \simple_arrow(F, i4, none, valley, right);
  \caption("Ramener les deux coins");
  \caption("superieurs au centre");
}

\step{
  \move(E, [I, K]);
  \move(F, [I, L]);
  \cut([E,D], K); \cut([E, A], K);
  \cut([F,C], L); \cut([F, B], L);
  \border(I, K);
  \border(I, L);
  \hide(E,F);
  \valley_fold(K, L);
  \simple_arrow(J, [K, L], none, valley, left);
  \caption("Remonter une seule couche");
  \caption("sur le triangle superieur.");   
}

\scale(120);

\step {
  \move(A, [K, L]);
  \move(B, [K, L]);
  i5 = \inter_cut([I, K], [A, B]);
  i6 = \inter_cut([I, L], [A, B]);
  \show(A, B);
  \hide([i5, K]);
  \hide([i6, L]);
  \border(K, L);

  % recentrage
  center = \intersection([A, B], [I, J]);
  \visible_area_center(center);

  \mountain_fold(i5, K);
  \return_arrow([i5, K], [A, K], none, mountain, left);
  \mountain_fold(i6, L);
  \return_arrow([i6, L], [B, L], none, mountain, right);
  \caption("Rabattre le deux coins superieurs derriere.");
}

\visible_area_center(center);

\step {
  \move(A, [i5, K]);
  \move(B, [i6, L]);
  \cut([A, B], i5);
  \cut([i5, B], i6);
  \hide(A, B);
  \border(i5, K);
  \border(i6, L);
  
  \mountain_fold(K, L);
  \return_arrow([K, L], [D, C], none , mountain, right);
  \caption("Faire passer par derriere");
  \caption("le rectangle inferieur.");
  \caption("Retourner le modele.");
  \turn_over_vertical;
}

\step{
  i7 = \inter_cut([I, J], [K, L]);
  \move(C, [K, L]);
  \move(D, [K, L]);
  \border(K, L);
  \hide([i7, J]);
  \hide([i5, K]);
  \hide([i6, L]);

  \simple_arrow(K, L, none, valley, left);
  \mountain_fold(I, i7);
  \caption("Faire se rejoindre les coins opposes.");
  \caption("Le modele s'ouvre selon les");
  \caption("hauteurs du triangle principal.");
}

\step {

}

