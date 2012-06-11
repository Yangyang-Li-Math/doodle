/*  This looks like c, but it's really -*-c++-*-
 * 
 *  Copyright (C) 2000 Doodle
 *  Jerome GOUT - Wed Apr  5 2000
 */


#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H

#include <vector>
#include "color.h"

#define EPSILON 1e-6

#define SQUARE_EDGE 100.0                    //- use to define the square vertices 
#define DIAMOND_EDGE (SQUARE_EDGE*sqrt(2))   //- use to define the diamond vertives

#define PS_BORDER_WIDTH   1  //- line width used for border line into PS files
#define PS_FOLD_WIDTH     0  //- line width used for fold into PS files
#define PS_VALLEY_WIDTH   PS_BORDER_WIDTH
#define PS_MOUNTAIN_WIDTH PS_BORDER_WIDTH
#define PS_XRAY_WIDTH     PS_BORDER_WIDTH

#define PAGE_HEIGHT       297
#define PAGE_WIDTH        210
#define CLIP_WIDTH        65
#define CLIP_HEIGHT       65
#define FIRST_TOP_MARGIN  60
#define TOP_MARGIN        20
#define BOTTOM_MARGIN     20
#define LEFT_MARGIN       20
#define RIGHT_MARGIN      20
#define VSPACE            5
#define HSPACE            5

#define CAPTION_HEIGHT_MM 5

#define SIMPLE_ARROW_ANGLE 60
#define RETURN_ARROW_RATIO 50

//- default space between point and fold top
#define FOLDSPC 5

enum turn_type  {turnNone, turnVertical, turnHorizontal};

extern color White; //- defined in color.cc

#ifdef WIN32
#define M_PI 3.141592
#endif // WIN32

#if WIN32 || (__GNUC__ && __sun__)
#define MAX(a,b) ((a) > (b)? (a) : (b))
#endif // WIN32

//- 50 units means 1 cm in paper
#define TO_PS(x) (x/50.0)

//- to check wether a double is null or not
#define IS_NULL(a) ((a) >= -EPSILON && (a) < EPSILON)

#endif  /* ! GLOBAL_DEF_H */


