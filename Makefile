CXX = g++
LEX = flex
YACC = bison
TAR = tar
CXXFLAGS = -g -DDEBUG -Wall -W
#CXXFLAGS = -O6
CPPFLAGS =  -I$(SRC_DIR)
LDFLAGS = 
VERSION = 2.1

SRC_FILES = vertex.cpp edge.cpp vect.cpp step.cpp date.cpp \
	diag_header.cpp read_instruction.cpp ps_output.cpp \
	arrow.cpp color.cpp face.cpp push_arrow.cpp \
	open_arrow.cpp repeat_arrow.cpp ps_prologue.cpp \
	main.cpp

INCLUDES = vertex.h edge.h vect.h step.h date.h diag_header.h \
	read_instruction.h ps_output.h arrow.h color.h face.h \
	global_def.h macro.h context.h unistd.h ps_prologue.h \
	arrow_symbol.h push_arrow.h open_arrow.h repeat_arrow.h \
	ps_prologue.h

DOO_DIR = `basename $(PWD)`
SRC_DIR = src
OBJ_DIR = OBJECTS
BIN_DIR = bin
DIA_DIR = diagrams

SRC = $(SRC_FILES:%=$(SRC_DIR)/%)
DEPSFILE = $(OBJ_DIR)/DEPENDENCIES
PRG = $(BIN_DIR)/doodle

# Generated files (by yacc and lex)
YACC_SRC = $(OBJ_DIR)/parser.tab.c
LEX_SRC = $(OBJ_DIR)/lex.yy.c
PARSER_SRC = $(LEX_SRC) $(YACC_SRC)
OBJ = $(PARSER_SRC:%.c=%.o) $(SRC_FILES:%.cpp=$(OBJ_DIR)/%.o) 

TAR_FILES = $(SRC) $(SRC_DIR)/parser.l $(SRC_DIR)/parser.y \
	$(INCLUDES:%=$(SRC_DIR)/%) $(DIA_DIR) Makefile TODO \
	Doodle.dsp Doodle.dsw

all : $(DEPSFILE) $(PRG)

clean : 
	@printf "Suppressing all produced files\n"
	@\rm -f $(OBJ_DIR)/* $(PRG) $(DEPSFILE) *~

deps : $(OBJ_DIR)
	@printf "Rebuilding the dependences file: %s\n" $(DEPSFILE)
	@\rm -f $(DEPSFILE)
	@touch $(DEPSFILE)
	@g++ -M $(CPPFLAGS) $(SRC) > $(DEPSFILE)

tar : 
	@\rm -f $(DIA_DIR)/*.ps $(DIA_DIR)/*~ $(DIA_DIR)/*.eps
	@printf "Make a tar distribution of version %s ..." $(VERSION)
	@$(TAR) zcf doodle-$(VERSION).tgz -C .. $(TAR_FILES:%=$(DOO_DIR)/%)
	@printf " Done\n"
	@printf "  Result : %s\n" doodle-$(VERSION).tgz

solaris:  $(OBJ_DIR) $(OBJ) $(BIN_DIR)
	@printf "Linking binary %s\n" $@
	@$(CXX) $(LDFLAGS) -o $(PRG) $(OBJ)

#------------------------------------------------------------

include $(DEPSFILE) 

$(PRG) : $(PARSER_SRC) $(OBJ_DIR) $(OBJ) $(BIN_DIR)
	@printf "Linking binary %s\n" $@
	@$(CXX) $(LDFLAGS) -o $(PRG) $(OBJ)

$(OBJ_DIR) : 
	@mkdir $(OBJ_DIR)

$(BIN_DIR) : 
	@mkdir $(BIN_DIR)

$(LEX_SRC): $(SRC_DIR)/parser.l
	@printf "Making lexer file\n"
	@$(LEX) -o$(LEX_SRC) $<

$(YACC_SRC) $(YACC_SRC:%.c=%.h): $(SRC_DIR)/parser.y
	@printf "Making parser files\n"
	@$(YACC) -t -v -b parser -d -o $(YACC_SRC) $(SRC_DIR)/parser.y

$(OBJ_DIR)/%.o : $(OBJ_DIR)/%.c
	@printf "Compiling %s :\n" $<
	@$(CXX) $(CXXFLAGS)  $(CPPFLAGS) -o $@ -c $<

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@printf "Compiling %s :\n" $<
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

$(DEPSFILE):
	@printf "Rebuilding the dependence file: %s\n" $(DEPSFILE)
	@touch $(DEPSFILE)
#	@mkdep -cg++  $(CPPFLAGS) -d$(OBJ_DIR) -k -o$(DEPSFILE) $(SRC) $(PARSER_SRC)
	@g++ -M $(CPPFLAGS) $(SRC) > $(DEPSFILE)
