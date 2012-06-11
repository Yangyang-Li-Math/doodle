/* 
 *  Class macro
 *  Copyright (C) 2000 Doodle
 *  Vincent Osele - 05 dec. 2000
 */


#ifndef MACRO_H
#define MACRO_H

#include <vector>
#include <algorithm>

typedef std::vector<std::string> symbolvect;

class macro {
public:
	macro()
		: _current(0)
	{ }
	macro(const symbolvect& symbols)
		: _symbols(symbols)
		, _current(0)
	{ }
	virtual ~macro() {
		partvect::iterator it = _parts.begin();
		for (; it!=_parts.end(); ++it)
			delete *it;
	}

	void add(const std::string& text) {
		check_current();
		_current->add(text);
	}
	void add(char c) {
		check_current();
		_current->add(c);
	}
	void addsymb(const std::string& symb) {
		symbolvect::iterator it =
			std::find(_symbols.begin(),_symbols.end(),symb);
		if (it==_symbols.end()) {
			check_current();
			_current->add(symb);
			return;
		}
		_current=0;
		int n = std::distance(_symbols.begin(),it);
		_parts.push_back(
			new symb_part(_symbols,n)
			);
	}
	void reset() {
		_symbols.clear();
	}
	void set(const symbolvect& symbols) {
		_symbols=symbols;
	}
	void set(const std::string& symb) {
		_symbols.push_back(symb);
	}
	std::string get() {
		std::string result;
		partvect::iterator it = _parts.begin();
		for (; it!=_parts.end(); ++it)
			result+=(*it)->get();
		return result;
	}

private:
	struct part {
		virtual std::string get() = 0;
	};
	typedef std::vector<part*> partvect;
	struct string_part : public part {
	        virtual ~string_part() {}
		std::string get() { return _s; }
		void add(const std::string& text) {
			_s+=text;
		}
		void add(char c) {
			_s+=c;
		}
		std::string _s;
	};
	struct symb_part : public part {
		symb_part(symbolvect& symbs, int n)
			: _n(n), _symbs(symbs)
		{ }
		std::string get() { return _symbs[_n]; }
		int _n;
		symbolvect& _symbs;
	};
	void check_current() {
		if (_current) return;
		_current = new string_part;
		_parts.push_back(_current);
	}
	symbolvect _symbols;
	partvect _parts;
	string_part* _current;
};

#endif  /* ! MACRO_H */
