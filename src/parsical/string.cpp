#include "string.hpp"

//////////////
// Includes //
#include <sstream>
#include <cmath>

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

// Consuming input until either whitespace or the end of file is
// reached. Throws an error if nothing is consumed.
std::string parsical::str::parseString(parsical::ParseStream<char>& stream) throw(parsical::ParseError) {
    std::string str = parsical::str::takeUntil(stream, isWhitespace);
    if (str == "")
        throw parsical::ParseError("Expected some input.");

    return str;
}

// Consuming all whitespace from the current position until the
// whitespace stops.
void parsical::str::consumeWhitespace(parsical::ParseStream<char>& stream) {
    parsical::dropWhile(stream, isWhitespace);
}

// Attempting to parse a bool out of a ParseStream. Does not consume any
// input upon failure.
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

// Attempting to parse a single digit out of a ParseStream. Does not
// consume any input upon failure.
int parsical::str::parseDigit(parsical::ParseStream<char>& stream) throw(parsical::ParseError) {
    if (!parsical::str::isNumber(stream.peek()))
        throw parsical::ParseError("");

    return (int)(stream.get() - '0');
}

// Attempting to parse out an entire int - either positive or negative.
// Does not consume any input upon failure.
int parsical::str::parseInt(parsical::ParseStream<char>& stream) throw(parsical::ParseError) {
    return parsical::tryParse<int>(stream, [](parsical::ParseStream<char>& stream) -> int {
        int sign = 1;
        if (stream.peek() == '-') {
            sign = -1;
            stream.get();
        }

        if (!parsical::str::isNumber(stream.peek()))
            throw parsical::ParseError("Expected a number.");

        int sum = 0;
        while (true) {
            try {
                sum *= 10;
                sum += parsical::str::parseDigit(stream);
            } catch (parsical::ParseError& e) {
                sum /= 10;
                break;
            }
        }

        return sum * sign;
    });
}

// Attempting to parse out an entire float - either positive or
// negative. Does not consume any input upon failure.
float parsical::str::parseFloat(ParseStream<char>& stream) throw(parsical::ParseError) {
    return parsical::tryParse<float>(stream, [](ParseStream<char>& stream) -> float {
        int sign = stream.peek() == '-' ? -1 : 1;
        if (sign == -1)
            stream.get();

        std::vector<char> number = parsical::takeWhile(stream, isNumber);
        std::vector<char> decimal;
        if (!stream.eof() && stream.peek() == '.') {
            stream.get();
            decimal = parsical::takeWhile(stream, isNumber);
        }

        if (!stream.eof() && stream.peek() == 'f')
            stream.get();

        float nAccum = 0.f;
        for (char c: number) {
            nAccum *= 10;
            nAccum += c - '0';
        }

        float dAccum = 0.f;
        for (char c: decimal) {
            dAccum *= 10;
            dAccum += c - '0';
        }

        dAccum /= pow(10.f, decimal.size());

        return sign * (nAccum + dAccum);
    });
}

// A set of basic functions to infer properties about specific characters.
bool parsical::str::isWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
bool parsical::str::isNumber    (char c) { return c >= '0' && c <= '9'; }
bool parsical::str::isUppercase (char c) { return c >= 'A' && c <= 'Z'; }
bool parsical::str::isLowercase (char c) { return c >= 'a' && c <= 'z'; }
bool parsical::str::isAlpha     (char c) { return isUppercase(c) || isLowercase(c); }
bool parsical::str::isAlphaNum  (char c) { return isNumber(c) || isAlpha(c); }
