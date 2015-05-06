#include "parsestream.hpp"

////
// StringParser

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
        throw parsical::ParseError("Cannot peek after EOF has been reached.");
    return str.at(pos());
}

// Getting the current position in this ParseStream.
int parsical::StringParser::pos() const noexcept { return p; }

// Consuming and returning a value.
char parsical::StringParser::get() throw(parsical::ParseError) {
    if (eof())
        throw parsical::ParseError("Cannot get after EOF has been reached.");
    return str.at(p++);
}

// Stepping back some interval.
void parsical::StringParser::stepBack(int n) throw(parsical::ParseError) {
    if (n > pos())
        throw parsical::ParseError("Stepping back so far would make the current position negative.");
    p -= n;
}

////
// IStreamParser

// Creating an IStreamParser from a r-value reference istream.
parsical::IStreamParser::IStreamParser(std::istream&& in) throw(std::runtime_error) :
        in(in) {
    if (!in.good())
        throw std::runtime_error("Input stream is not good.");

    // TODO: I think I might be running into some problems where the
    // std::istream&& is losing scope and therefore being automatically removed
    // before I want it to be. Therefore when I call it outside of this
    // constructor (and the rvalue reference is gone), it segfaults.

    next = '\0';
    p = -1;
    get();
}

// Creating an IStreamParser from an l-value reference istream.
parsical::IStreamParser::IStreamParser(std::istream& in) throw(std::runtime_error) :
        in(in) {
    if (!in.good())
        throw std::runtime_error("Input stream is not good.");

    next = '\0';
    p = -1;
    get();
}

// Creating an IStreamParser from a path to a file on the filesystem.
parsical::IStreamParser::IStreamParser(std::string path) throw(std::runtime_error) :
        parsical::IStreamParser(std::ifstream(path)) { }

// Checking whether this ParseStream has reached its end.
bool parsical::IStreamParser::eof() const noexcept { return in.eof(); }

// Peeking at the next value without consuming it.
char parsical::IStreamParser::peek() const throw(parsical::ParseError) {
    if (eof())
        throw ParseError();
    return next;
}

// Getting the current position in this ParseStream.
int parsical::IStreamParser::pos() const noexcept { return p; }

// Consuming and returning a value.
char parsical::IStreamParser::get() throw(parsical::ParseError) {
    if (next != '\0')
        gotten.push(next);
    in.get(next);
    p++;

    return gotten.top();
}

// Stepping back some interval.
void parsical::IStreamParser::stepBack(int n) throw(parsical::ParseError) {
    if (n > pos())
        throw parsical::ParseError("Stepping back so far would make the current position negative.");
    for (int i = 0; i < n; i++)
        unget();
}

// Un-getting a single character. It ought to be equivalent to
// stepBack(1)
void parsical::IStreamParser::unget() throw(parsical::ParseError) {
    if (p <= 0)
        throw ParseError("Ungetting would make the current position negative.");

    in.putback(next);
    next = gotten.top();
    gotten.pop();
    p--;
}
