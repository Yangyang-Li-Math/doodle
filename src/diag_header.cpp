// This looks like c, but it's really -*-c++-*-
// 
// Copyright (c) 2000 Doodle
// Jerome GOUT - Thu Mar 30 2000
// 


#include "diag_header.h"
#include "global_def.h"

diag_header::diag_header() : 
  Design_Date(), Diagram_Date(),
  Designer(), Diagrammer(), Title(),
  Top_Margin(TOP_MARGIN), Bottom_Margin(BOTTOM_MARGIN),
  Left_Margin(LEFT_MARGIN), Right_Margin(RIGHT_MARGIN),
  V_Space(VSPACE), H_Space(HSPACE), Front_Color(White), 
  Back_Color(White)
{}

diag_header::diag_header(const diag_header& Ref):
  Design_Date(Ref.Design_Date), Diagram_Date(Ref.Diagram_Date),
  Designer(Ref.Designer), Diagrammer(Ref.Diagrammer), Title(Ref.Title),
  Top_Margin(Ref.Top_Margin), Bottom_Margin(Ref.Bottom_Margin),
  Left_Margin(Ref.Left_Margin), Right_Margin(Ref.Right_Margin),
  V_Space(Ref.V_Space), H_Space(Ref.H_Space), Front_Color(Ref.Front_Color),
  Back_Color(Ref.Back_Color)
{}

diag_header::~diag_header()
{}

diag_header& diag_header::operator=(const diag_header& Ref)
{
  if (this != &Ref) {
    Design_Date = Ref.Design_Date;
    Diagram_Date = Ref.Diagram_Date;
    Designer = Ref.Designer;
    Diagrammer = Ref.Diagrammer;
    Title = Ref.Title;
    Top_Margin = Ref.Top_Margin;
    Bottom_Margin = Ref.Bottom_Margin;
    Left_Margin = Ref.Left_Margin;
    Right_Margin = Ref.Right_Margin;
    V_Space = Ref.V_Space;
    H_Space = Ref.H_Space;
    Front_Color = Ref.Front_Color;
    Back_Color = Ref.Back_Color;
  }
  return *this;
}

void diag_header::get_front_rgb_values(int& r, int& g, int& b) const
{
  r = Front_Color.get_red();
  g = Front_Color.get_green();
  b = Front_Color.get_blue();
}

void diag_header::get_back_rgb_values(int& r, int& g, int& b) const
{
  r = Back_Color.get_red();
  g = Back_Color.get_green();
  b = Back_Color.get_blue();
}

//------------------------------------------------------------

#ifdef DEBUG
#include <iostream>

void diag_header::gdb_info(const std::string & s) const
{
  std::cout << s << "Title         : " << Title << std::endl;
  std::cout << s << "Designer      : " << Designer << std::endl;
  std::cout << s << "Design date   : "; Design_Date.gdb_info(); 
  std::cout << s << "Diagrammer     : " << Diagrammer << std::endl;
  std::cout << s << "Diagram date  : "; Diagram_Date.gdb_info();
  std::cout << s << "Top margin    : " << Top_Margin << std::endl;
  std::cout << s << "Bottom margin : " << Bottom_Margin << std::endl;
  std::cout << s << "Left margin   : " << Left_Margin << std::endl;
  std::cout << s << "Right margin  : " << Right_Margin << std::endl;
  std::cout << s << "V space       : " << V_Space << std::endl;
  std::cout << s << "H space       : " << H_Space << std::endl;
  std::cout << s << "Front color   : " << Front_Color.get_red() << ", " << Front_Color.get_green()
       << ", " << Front_Color.get_blue() << std::endl;
  std::cout << s << "Back color    : " << Back_Color.get_red() << ", " << Back_Color.get_green()
       << ", " << Back_Color.get_blue() << std::endl;
}
#endif /* ! DEBUG */
