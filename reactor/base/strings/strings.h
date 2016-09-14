#ifndef SDUZH_BASE_STRINGS_H
#define SDUZH_BASE_STRINGS_H

#include <string>
#include <vector>

namespace sduzh {

namespace base {

namespace strings {

typedef std::vector<std::string> StringList;

std::string lstrip(const std::string &s); 
std::string rstrip(const std::string &s); 
std::string strip(const std::string &s);

void lstrip(std::string *s); 
void rstrip(std::string *s);
void strip(std::string *s);

int to_int(const std::string &s, bool *ok);
unsigned int to_uint(const std::string &s, bool *ok);

float to_float(const std::string &s, bool *ok);
double to_double(const std::string &s, bool *ok);

StringList split(const std::string &s, char sep=' ');

std::string lower(const std::string &s);
std::string upper(const std::string &s);

/*
bool startswith(const std::string &s);
bool endswith(const std::string &s);

bool isalnum(const std::string &s);
bool isalpha(const std::string &s);
bool isdigit(const std::string &s);
bool islower(const std::string &s);
bool isspace(const std::string &s);
bool isupper(const std::string &s);

std::string replace(const std::string &s);
void replace(std::string *s);
*/

} //namespace strings
} //namespace base
} //namespace sduzh

#endif

