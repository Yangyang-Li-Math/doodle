/*  This looks like c, but it's really -*-c++-*-
 * 
 *  Copyright (C) 2000 Doodle
 *  Jerome GOUT - Wed Apr  5 2000
 */


#ifndef READ_INSTRUCTION_H
#define READ_INSTRUCTION_H

#include <string>
#include <vector>
#include "edge.h"

std::string expand_ref(const std::string& s);
void read_new_step();
void read_diamond(const std::string&, const std::string&, const std::string&,
		  const std::string&);
void read_square(const std::string&, const std::string&, const std::string&,
		 const std::string&);
void read_fraction(const std::string&, const std::string&, const std::string&, int, int);
void read_caption(const std::string& c);
void read_index(const std::string& i);
void read_valley_mountain_fold(const std::string&, const std::string&, bool, int,
			       int );
void read_point_to_point(const std::string&, const std::string&, const std::string&,
			 const std::string&, const std::string&, const std::string&,
			 const std::string&, const std::string&); 
void read_point_to_line(const std::string&, const std::string&, const std::string&,
			const std::string&, const std::string&, const std::string&,
			const std::string&, const std::string&); 
void read_line_to_line(const std::string&, const std::string&, const std::string&,
		       const std::string&, const std::string&, const std::string&,
		       const std::string&, const std::string&, const std::string&,
		       const std::string&); 
void read_line_to_line(const std::string&, const std::string&, const std::string&,
		   const std::string&, const std::string&, const std::string&); 
void read_simple_arrow(const std::string&, const std::string&, const std::string&, const std::string&,
		const std::string&, int);
void read_simple_arrow(const std::string&, const std::string&, const std::string&, const std::string&,
		const std::string&, const std::string&, int);
void read_return_arrow(const std::string&, const std::string&, const std::string&, const std::string&,
		       const std::string&, const std::string&, const std::string&, int);
void read_turn_over(bool);
void read_rotate(double );
void read_hide_show(const std::string&, const std::string&, bool);
void read_hide_show(const std::vector<std::string>&, bool);
void read_border(const std::string&, const std::string&);
void read_fold(const std::string&, const std::string&);
void read_intersection(const std::string&, const std::string&, const std::string&,
		       const std::string&, const std::string&);
void read_symmetry(const std::string&, const std::string&, const std::string&,
		  const std::string&);
void read_cut(const std::string&, const std::string&, const std::string&);
void read_debug_point();
void read_debug_point(const std::vector<std::string>&);
void read_debug_line();
void read_debug_line(const std::string&, const std::string&);
void read_debug_line(const std::vector<std::string>&);
void read_xray_fold(const std::string&, const std::string&);
void read_fold_percent(const std::string&, const std::string&, edge_type, int, int);
void read_fold_limit(const std::string&, const std::string&, edge_type t,
		     const std::string&, const std::string&,
		     const std::string&, const std::string&);
void read_fold_both(const std::string&, const std::string&, edge_type, int,
		    const std::string&, const std::string&, bool);
void read_visible_height(int);
void read_visible_width (int);
void read_visible_center(const std::string&);
void read_visible_center_inter(const std::string&);
void read_scale(int);
color* read_color(const std::string&);
void read_fill(const color&, const std::vector<std::string>&);
void read_unfill(const std::vector<std::string>&);
void read_unfill_all();
void read_vertical_rectangle(const std::string&, const std::string&, const std::string&,
		       const std::string&, const std::string&);
void read_vertical_rectangle(const std::string&, const std::string&, const std::string&,
		       const std::string&, int);
void read_horizontal_rectangle(const std::string&, const std::string&, const std::string&,
		       const std::string&, const std::string&);
void read_horizontal_rectangle(const std::string&, const std::string&, const std::string&,
		       const std::string&, int);
void read_clip(bool, int, int);
void read_clip(bool);
void read_perpendicular(const std::string&, const std::string&, const std::string&, const std::string&);
void read_perpendicular(const std::string&, const std::string&, const std::string&,
		   const std::string&, const std::string&, const std::string&);
void read_parallel(const std::string&, const std::string&, const std::string&,
		   const std::string&, const std::string&, const std::string&);
void read_move(const std::string&, const std::string&);
void read_move(const std::string&, const std::string&, const std::string&);
void read_inter_cut(const std::string&, const std::string&, const std::string&,
		    const std::string&, const std::string&);
void read_space_fold(const std::string&, const std::string&, int, int);
void read_eps(const std::string&, int, int, int, int);
void read_reset();
void read_shift(const std::string&, double, double);
void read_text(const std::string&, const std::string&);
void read_unshift_all();
void read_unshift(std::vector<std::string>&);
void read_duplicate(const std::string&, const std::string&);
void read_rabbit_ear(const std::string&, const std::string&, const std::string&,
		     const std::string&, const std::string&, const std::string&,
		     const std::string&, bool);
void read_push_arrow(const std::string&, int, int);
void read_push_arrow(const std::string&);
void read_push_arrow(const std::string&, const std::string&, const std::string&, int);
void read_open_arrow(const std::string&, const std::string&, const std::string&);
void read_repeat_arrow(const std::string&, unsigned int, const std::string&, const std::string&, int, int);
void read_repeat_arrow(const std::string&, unsigned int, const std::string&, const std::string&);
void read_label(const std::string&);
#endif  /* ! READ_INSTRUCTION_H */


