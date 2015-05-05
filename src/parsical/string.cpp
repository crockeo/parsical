#include "string.hpp"

//////////////
// Includes //
#include <sstream>

//////////
// Code //

// Attempting to parse a specific string. It should be noted that it will
// consume input even if the string itself is not matched. The amount of
// consumed input is equivalent to that of the portion of the matched
std::string parsical::str::string(parsical::ParseStream<char>& stream, std::string str) throw(parsical::ParseError) {
    for (char c: str) {
        if (stream.peek() != c)
            throw parsical::ParseError("Could not match string.");
        stream.get();
    }

    return str;
}

// A version of takeWhile that returns a std::string instead of a vector
// of characters.
std::string parsical::str::takeWhile(parsical::ParseStream<char>& stream, std::function<bool(char)> fn) {
    std::vector<char> chars = parsical::takeWhile(stream, fn);
    std::ostringstream builder;

    for (char c: chars)
        builder << c;

    return builder.str();
}

// A version of takeUntil that returns a std::string instead of a vector
// of characters.
std::string parsical::str::takeUntil(parsical::ParseStream<char>& stream, std::function<bool(char)> fn) {
    std::vector<char> chars = parsical::takeUntil(stream, fn);
    std::ostringstream builder;

    for (char c: chars)
        builder << c;

    return builder.str();
}

// Attempting to parse a bool out of a ParseStream. Does not consume any input
// upon failure.
bool parsical::str::parseBool(parsical::ParseStream<char>& stream) throw(parsical::ParseError) {
    std::string str;

    std::vector<std::function<std::string(parsical::ParseStream<char>&)>> fns {
        std::bind(parsical::str::string, std::placeholders::_1, "true"),
        std::bind(parsical::str::string, std::placeholders::_1, "false")
    };

    try {
        str = parsical::option<std::string>(stream, fns);
    } catch (parsical::ParseError& e) {
        throw parsical::ParseError("Neither \"true\" nor \"false\" could be matched.");
    }

    if (str == "true")
        return true;
    return false;
}

// A set of basic functions to infer properties about specific characters.
bool parsical::str::isWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
bool parsical::str::isNumber    (char c) { return c >= '0' && c <= '9'; }
bool parsical::str::isUppercase (char c) { return c >= 'A' && c <= 'Z'; }
bool parsical::str::isLowercase (char c) { return c >= 'a' && c <= 'z'; }
bool parsical::str::isAlpha     (char c) { return isUppercase(c) || isLowercase(c); }
bool parsical::str::isAlphaNum  (char c) { return isNumber(c) || isAlpha(c); }
