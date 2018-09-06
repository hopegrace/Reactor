#include "reactor/base/strings/strings.h"
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
	std::string result(s);
	lstrip(&result);
	return result;
}

std::string lstrip(const std::string &s, char c) {
	std::string result(s);
	lstrip(&result, c);
	return result;
}

std::string rstrip(const std::string &s) {
	std::string result(s);
	rstrip(&result);
	return result;
}

std::string rstrip(const std::string &s, char c) {
	std::string result(s);
	rstrip(&result, c);
	return result;
}

std::string strip(const std::string &s) {
	return rstrip(lstrip(s));
}

std::string strip(const std::string &s, char c) {
	return rstrip(lstrip(s, c), c);
}

void lstrip(std::string *s) {
	size_t i = 0;
	while (i < s->size() && ((*s)[i] == ' ' || (*s)[i] == '\t')) {
		i++;
	}

	s->erase(0, i);
}

void lstrip(std::string *s, char c) {
	size_t i = 0;
	while (i < s->size() && (*s)[i] == c) {
		i++;
	}

	s->erase(0, i);
}

void rstrip(std::string *s) {
	if (s->empty())
		return;
	
	int i = static_cast<int>(s->size()) - 1;
	while (i >= 0 && ((*s)[i] == ' ' || (*s)[i] == '\t')) {
		i--;
	}
	s->erase(i+1);
}

void rstrip(std::string *s, char c) {
	if (s->empty())
		return;
	
	int i = static_cast<int>(s->size()) - 1;
	while (i >= 0 && s->at(i) == c) {
		i--;
	}
	s->erase(i+1);
}

void strip(std::string *s) {
	lstrip(s);
	rstrip(s);
}

void strip(std::string *s, char c) {
	lstrip(s, c);
	rstrip(s, c);
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

bool startswith(const std::string &s, const std::string &start) {
	return s.size() >= start.size() && equal(s.begin(), s.begin() + start.size(), start.begin());
}

bool endswith(const std::string &s, const std::string &tail) {
	return s.size() >= tail.size() && equal(s.end() - tail.size(), s.end(), tail.begin());
}

} //namespace strings
} //namespace base
} //namespace reactor


