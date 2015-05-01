// Name: parsical/parseerror.hpp
//
// Description:
//   This describes an error specifically for parsing and parser creation.

#ifndef _PARSICAL_PARSE_ERROR_HPP_
#define _PARSICAL_PARSE_ERROR_HPP_

//////////////
// Includes //
#include <exception>
#include <string>

//////////
// Code //

namespace parsical {
    // A specific error for parsing such that one can match against errors
    // thrown specifically by this library and not general runtime_errors.
    class ParseError : public std::exception {
    private:
        std::string str;
    public:
        // Creating a specific parse error.
        ParseError(std::string);

        // Creating a generic parse error.
        ParseError();

        // Getting some information out of this beast.
        virtual const char* what() const throw() override;
    };
}

#endif
