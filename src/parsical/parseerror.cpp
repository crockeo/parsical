#include "parseerror.hpp"

// Creating a specific parse error.
parsical::ParseError::ParseError(std::string str) :
        str("Parse error: " +  str) { }

// Creating a generic parse error.
parsical::ParseError::ParseError() :
        parsical::ParseError("Generic parse error.") { }

// Getting some information out of this beast.
const char* parsical::ParseError::what() const throw() { return str.c_str(); }
