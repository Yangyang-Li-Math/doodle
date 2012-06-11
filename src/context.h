/* 
 *  Class context
 *  Copyright (C) 2000 Doodle
 *  Vincent Osele - 04 dec. 2000
 */


#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>

struct yy_buffer_state;

class context {
public:
	context(const std::string& n, int l, yy_buffer_state* b)
		: Name(n)
		, Line(l)
		, Buffer(b)
	{ }
	virtual ~context() { }

	int getLine() { return Line; }
	const std::string& getName() { return Name; }
	yy_buffer_state* getBuffer() { return Buffer; }

private:
  std::string Name;
  int Line;
  yy_buffer_state* Buffer;
};

#endif  /* ! CONTEXT_H */
