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
// FileParser

// Attempting to back up a chunk.
void parsical::FileParser::backupChunk() {
    for (int i = 0; i < lastChunkSize; i++)
        in.unget();

    priorChunks -= 2;
    currChunkSize = maxChunkSize;

    loadNextChunk();
}

// Attempting to load the next chunk.
void parsical::FileParser::loadNextChunk() {
    in.get(chunk, maxChunkSize + 1, '\0');

    lastChunkSize = currChunkSize;
    currChunkSize = in.gcount();

    priorChunks++;
}

// Creating a FileParser from a path to a file and an alternate chunk
// size.
parsical::FileParser::FileParser(std::string path, int maxChunkSize) throw(std::runtime_error) :
        maxChunkSize(maxChunkSize),
        in(path) {
    currChunkSize = -1;
    priorChunks = -1;
    chunk = new char[maxChunkSize + 1];
    p = 0;

    loadNextChunk();
}

// Creating a FileParser from a path to a file.
parsical::FileParser::FileParser(std::string path) throw(std::runtime_error) :
        parsical::FileParser(path, parsical::FileParser::DEFAULT_CHUNK_SIZE) { }

// Closing up a file.
parsical::FileParser::~FileParser() {
    in.close();
    delete chunk;
}

// Checking whether this ParseStream has reached its end.
bool parsical::FileParser::eof() const noexcept {
    return (p % maxChunkSize == 0 || p % maxChunkSize == currChunkSize) && p != 0 && in.eof();
}

// Peeking at the next value without consuming iint parsical::FileParser::peek() const throw(parsical::ParseError) {
char parsical::FileParser::peek() const throw(parsical::ParseError) {
    if (eof())
        throw parsical::ParseError("Cannot peek after the input stream has been exhausted.");
    return chunk[p % maxChunkSize];
}

// Getting the current position in this ParseStream.
int parsical::FileParser::pos() const noexcept { return p; }

// Consuming and returning a value.
char parsical::FileParser::get() throw(parsical::ParseError) {
    if (eof())
        throw parsical::ParseError("Cannot get after the input stream has been exhausted.");

    if (p != 0 && p % maxChunkSize == 0)
        loadNextChunk();

    return chunk[p++ % maxChunkSize];
}

// Stepping back some interval.
void parsical::FileParser::stepBack(int n) throw(parsical::ParseError) {
    if (n > p)
        throw parsical::ParseError("Stepping back so far would make the current position negative.");

    while (p - n < priorChunks * maxChunkSize)
        backupChunk();

    p -= n;
    if (p <= 0)
        lastChunkSize = -1;
}

// Un-getting a single character. It ought to be equivalent to
// stepBack(1)
void parsical::FileParser::unget() throw(parsical::ParseError) {
    if (pos() <= 0)
        throw parsical::ParseError("Cannot unget after ");
    p -= 1;
}
