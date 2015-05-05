// Name: parsical/string.hpp
//
// Description:
//   A a bunch of functions for parsing specifically on strings.

#ifndef _PARSICAL_STRING_HPP_
#define _PARSICAL_STRING_HPP_

//////////////
// Includes //
#include <functional>
#include <string>

#include "parsestream.hpp"
#include "parseerror.hpp"
#include "general.hpp"

//////////
// Code //

namespace parsical {
    namespace str {
        // Attempting to parse a specific string. It should be noted that it
        // will consume input even if the string itself is not matched. The
        // amount of consumed input is equivalent to that of the portion of the
        // matched string. A string that matches on 3 characters, but not the
        // 4th will consume 3 characters in the ParseStream.
        std::string string(parsical::ParseStream<char>&, std::string) throw(ParseError);

        // A version of takeWhile that returns a std::string instead of a vector
        // of characters.
        std::string takeWhile(parsical::ParseStream<char>&, std::function<bool(char)>);

        // A version of takeUntil that returns a std::string instead of a vector
        // of characters.
        std::string takeUntil(parsical::ParseStream<char>&, std::function<bool(char)>);

        // Attempting to parse a bool out of a ParseStream.
        bool parseBool(parsical::ParseStream<char>&) throw(ParseError);

        // A set of basic functions to infer properties about specific characters.
        bool isWhitespace(char);
        bool isNumber(char);
        bool isUppercase(char);
        bool isLowercase(char);
        bool isAlpha(char);
        bool isAlphaNum(char);
    }
}

#endif
