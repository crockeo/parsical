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
