#include "parsestream.hpp"

// Constructing a StringParser from a given string.
parsical::StringParser::StringParser(std::string str) :
        str(str),
        p(0) { }

// Checking whether this ParseStream has reached its end.
bool parsical::StringParser::eof() const noexcept {
    return pos() >= str.size();
}

// Peeking at the next value without consuming it.
char parsical::StringParser::peek() const throw(parsical::ParseError) {
    if (eof())
        throw ParseError("Cannot peek after EOF has been reached.");
    return str.at(pos());
}

// Getting the current position in this ParseStream.
int parsical::StringParser::pos() const noexcept { return p; }

// Consuming and returning a value.
char parsical::StringParser::get() throw(parsical::ParseError) {
    if (eof())
        throw ParseError("Cannot get after EOF has been reached.");
    return str.at(p++);
}

// Stepping back some interval.
void parsical::StringParser::stepBack(int n) throw(parsical::ParseError) {
    if (n > pos())
        throw ParseError("Stepping back so far would make the current position negative.");
    p -= n;
}
