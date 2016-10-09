#include "strings.h"
#include <cstdlib>
#include <sstream>

using namespace std;

namespace reactor {
namespace base {
namespace strings {


std::string lower(const std::string &s) {
	std::string l(s);
	for (size_t i=0, sz=l.size(); i < sz; i++) {
		l[i] = static_cast<char>(tolower(l[i]));
	}
	return l;
}

std::string upper(const std::string &s) {
	std::string u(s);
	for (size_t i=0, sz=u.size(); i < sz; i++) {
		u[i] = static_cast<char>(toupper(u[i]));
	}
	return u;
}

std::string lstrip(const std::string &s) {
	size_t i = 0;
	size_t sz = s.size();
	while (i < sz && (s[i]==' ' || s[i]=='\t')) { i++; }
	return s.substr(i);
}

std::string rstrip(const std::string &s) {
	size_t i = s.size();
	while (i > 0 && (s[i-1]==' ' || s[i-1]=='\t')) { i--; }
	return s.substr(0, i);
}

std::string strip(const std::string &s) {
	return rstrip(lstrip(s));
}

void lstrip(std::string *s) {
	if (s) {
		*s = lstrip(*s);
	}
}

void rstrip(std::string *s) {
	if (s) {
		*s = rstrip(*s);
	}
}

void strip(std::string *s) {
	if (s) {
		*s = strip(*s);
	}
}

StringList split(const std::string &s, char sp) {
	StringList words;
	stringstream ss(s);
	string word;
	while (getline(ss, word, sp)) {
		if (word.empty())
			continue;
		lstrip(word);
		rstrip(word);
		words.push_back(word);
	}
	return words;
}

int to_int(const string &s, bool *ok) {
	string tmp = rstrip(s);
	char *end;
	int n = static_cast<int>(strtol(tmp.c_str(), &end, 0));
	if (ok) {
		*ok = (*end != '\0') ? false : 
				tmp.empty() ? false : true;;
	}
	return n;
}

unsigned int to_uint(const string &s, bool *ok) {
	string tmp = rstrip(s);
	char *end;
	unsigned int n = static_cast<unsigned int>(strtoul(tmp.c_str(), &end, 0));
	if (ok) {
		*ok = (*end != '\0') ? false :
				tmp.empty() ? false : 
				tmp[0]=='-' ? false : true;
	}
	return n;
}

float to_float(const string &s, bool *ok) {
	string tmp = rstrip(s);
	char *end;
	float f = strtof(tmp.c_str(), &end);
	if (ok) {
		*ok = (*end != '\0') ? false: 
				tmp.empty() ? false : true;
	}
	return f;
}

double to_double(const string &s, bool *ok) {
	string tmp = rstrip(s);
	char *end;
	float f = static_cast<float>(strtod(tmp.c_str(), &end));
	if (ok) {
		*ok = (*end != '\0') ? false: 
				tmp.empty() ? false : true;
	}
	return f;
}

} //namespace strings
} //namespace base
} //namespace reactor


